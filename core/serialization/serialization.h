//
// Created by dmerkushov on 12.07.2021.
//

#ifndef NANOSERVICES2_SERIALIZATION_H
#define NANOSERVICES2_SERIALIZATION_H

#include "../../core/exception/ns_exception.h"
#include "../../plugins/logging/logging.h"
#include "../../thirdparty/swansontec/map-macro/do_foreach.h"
#include "../../util/macroutils/classname.h"
#include "../../util/stringutils/stringutils.h"
#include "../../util/templateutils/specialization.h"

#include <cxxabi.h>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace nanoservices {

enum class RecordType {
    NULL_VALUE,
    BYTE_ARRAY,
    STRING,
    SIGNED_INT_8,
    UNSIGNED_INT_8,
    SIGNED_INT_16,
    UNSIGNED_INT_16,
    SIGNED_INT_32,
    UNSIGNED_INT_32,
    SIGNED_INT_64,
    UNSIGNED_INT_64,
    FLOAT_32,
    FLOAT_64,
    SERIALIZABLE_CLASS,
    LIST,
    MAP
};

/**
 * @brief Get the name of the record type
 * @param recordType
 * @return
 */
const char *getRecordTypeName(RecordType recordType);

/**
 * @brief Append the record type's name to the output stream
 * @param os
 * @param recordType
 * @return
 */
std::ostream &operator<<(std::ostream &os, const RecordType &recordType);

/**
 * @brief A record in the serialization sequence
 */
struct SerializationRecord {
    /**
     * @brief Type of the record
     */
    RecordType type;

    /**
     * @brief Field name of the record
     */
    std::shared_ptr<std::string> fieldName;

    /**
     * @brief Data of the record
     */
    std::shared_ptr<void> data;
};

template<typename T>
concept Serializable = requires(T t) {
    { t.__nanoservices2_serializer_serialize() } -> std::convertible_to<std::shared_ptr<std::vector<std::shared_ptr<SerializationRecord>>>>;
    t.__nanoservices2_serializer_deserialize(t.__nanoservices2_serializer_serialize());
};

#define __NANOSERVICES2_SERIALIZE_FIELDCLAUSE__(FIELDNAME) \
    result->push_back(nanoservices::Serializer::serializeField(*fieldNamesIter, (FIELDNAME))); \
    ++fieldNamesIter;

#define __NANOSERVICES2_DESERIALIZE_FIELDCLAUSE__(FIELDNAME) \
    nanoservices::Serializer::deserializeField(&(FIELDNAME), *serializerRecordsIter); \
    ++serializerRecordsIter;

// SonarLint will argue: "Each instance of a parameter in a function-like macro should be enclosed in parentheses" in the body of this macro.
// But what we need is actually using it without parentheses!
#define NANOSERVICES2_MAKE_SERIALIZABLE(...) \
\
private: \
    mutable std::shared_ptr<std::vector<std::shared_ptr<std::string>>> __nanoservices2_fieldNamesVector; \
\
public: \
    std::shared_ptr<std::vector<std::shared_ptr<SerializationRecord>>> __nanoservices2_serializer_serialize() { \
        if(!__nanoservices2_fieldNamesVector) { \
            const char *fieldNames = #__VA_ARGS__; \
            __nanoservices2_fieldNamesVector = nanoservices::splitString(std::make_shared<std::string>(fieldNames), ',', true, false); \
        } \
        auto result = make_shared<std::vector<std::shared_ptr<SerializationRecord>>>(); \
        auto fieldNamesIter = __nanoservices2_fieldNamesVector->begin(); \
        DO_FOREACH(__NANOSERVICES2_SERIALIZE_FIELDCLAUSE__, __VA_ARGS__) \
        return result; \
    } \
\
    void __nanoservices2_serializer_deserialize(std::shared_ptr<std::vector<std::shared_ptr<SerializationRecord>>> serializerRecords) { \
        auto serializerRecordsIter = serializerRecords->begin(); \
        DO_FOREACH(__NANOSERVICES2_DESERIALIZE_FIELDCLAUSE__, __VA_ARGS__) \
    }

class Serializer {
private:
    static std::shared_ptr<std::string> _mapItemKeyRecordName;
    static std::shared_ptr<std::string> _mapItemValueRecordName;
    static std::shared_ptr<std::string> _listItemRecordName;
    static std::shared_ptr<Logger> _logger;

    static void _initLogger();

public:
    template<typename T>
    static std::shared_ptr<SerializationRecord> serializeListField(std::shared_ptr<std::string> fieldName, std::vector<T> &fieldValue) {
        auto record = std::make_shared<SerializationRecord>();

        record->fieldName = fieldName;
        if constexpr(std::is_same_v<T, std::vector<char>>) {
            record->type = RecordType::BYTE_ARRAY;
            record->data = std::make_shared<std::vector<char>>(fieldValue);
        } else {
            record->type = RecordType::LIST;
            record->data = std::make_shared<std::vector<std::shared_ptr<SerializationRecord>>>();

            auto data = std::static_pointer_cast<std::vector<std::shared_ptr<SerializationRecord>>>(record->data);

            for(auto iter = fieldValue.begin(); iter != fieldValue.end(); ++iter) {
                data->push_back(serializeField(_listItemRecordName, *iter));
            }
        }

        return record;
    }

    template<typename T>
    static void deserializeListField(std::vector<T> *fieldValuePtr, std::shared_ptr<SerializationRecord> serializationRecord) {
        auto recordsVector = std::static_pointer_cast<std::vector<std::shared_ptr<SerializationRecord>>>(serializationRecord->data);

        for(auto iter = recordsVector->begin(); iter != recordsVector->end(); ++iter) {
            T t;
            deserializeField(&t, *iter);
            fieldValuePtr->push_back(t);
        }
    }

    /**
     * @brief Serialize a map
     * @tparam K type of keys of a map. Must be serializable
     * @tparam V
     * @param fieldName
     * @param fieldValue
     * @return
     */
    template<typename K, typename V>
    static std::shared_ptr<SerializationRecord> serializeMapField(std::shared_ptr<std::string> fieldName, std::map<K, V> &fieldValue) {
        auto record = std::make_shared<SerializationRecord>();
        record->fieldName = fieldName;

        record->type = RecordType::MAP;
        record->data = std::make_shared<std::vector<std::shared_ptr<SerializationRecord>>>();
        auto data = std::static_pointer_cast<std::vector<std::shared_ptr<SerializationRecord>>>(record->data);

        for(auto const &[k, v] : fieldValue) {
            // k and v have the 'const K' and 'const V' types, which are not supported by serializeField(), as a const
            // field cannot be deserialized
            K key = k;
            std::shared_ptr<SerializationRecord> keyRecord = serializeField(_mapItemKeyRecordName, key);
            data->push_back(keyRecord);

            V value = v;
            std::shared_ptr<SerializationRecord> valueRecord = serializeField(_mapItemValueRecordName, value);
            data->push_back(valueRecord);
        }
        return record;
    }

    template<typename K, typename V>
    static void deserializeMapField(std::map<K, V> *fieldValuePtr, std::shared_ptr<SerializationRecord> serializationRecord) {
        auto recordsVector = std::static_pointer_cast<std::vector<std::shared_ptr<SerializationRecord>>>(serializationRecord->data);

        auto iter = recordsVector->begin();
        while(iter != recordsVector->end()) {
            K k;
            deserializeField(&k, *iter);

            ++iter;
            if(iter == recordsVector->end()) {
                std::stringstream msgSS;
                msgSS << "Reached the end of the serialization record vector when deserializing map field " << serializationRecord->fieldName;
                NS_THROW(msgSS.str().c_str());
            }

            V v;
            deserializeField(&v, *iter);

            (*fieldValuePtr)[k] = v;

            ++iter;
        }
    }

    template<typename T>
    static std::shared_ptr<SerializationRecord> serializeField(std::shared_ptr<std::string> fieldName, T &fieldValue) {
        _initLogger();
        auto msgf = [fieldValue, fieldName]() {
            std::stringstream msgSS;
            int demangleStatus = 0;
            char *fieldTypeName = abi::__cxa_demangle(typeid(fieldValue).name(), 0, 0, &demangleStatus);
            msgSS << "Serializing an instance of " << fieldTypeName << ". Field name: " << *fieldName;
            ::free(fieldTypeName);
            return msgSS;
        };
        _logger->trace(msgf);

        auto record = std::make_shared<SerializationRecord>();
        record->fieldName = fieldName;
        if constexpr(std::is_same_v<T, std::string>) {
            record->type = RecordType::STRING;
            record->data = std::make_shared<std::string>(fieldValue);
        } else if constexpr(std::is_same_v<T, int8_t>) {
            record->type = RecordType::SIGNED_INT_8;
            record->data = std::make_shared<int8_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, u_int8_t>) {
            record->type = RecordType::UNSIGNED_INT_8;
            record->data = std::make_shared<u_int8_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, int16_t>) {
            record->type = RecordType::SIGNED_INT_16;
            record->data = std::make_shared<int16_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, u_int16_t>) {
            record->type = RecordType::UNSIGNED_INT_16;
            record->data = std::make_shared<u_int16_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, int32_t>) {
            record->type = RecordType::SIGNED_INT_32;
            record->data = std::make_shared<int32_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, u_int32_t>) {
            record->type = RecordType::UNSIGNED_INT_32;
            record->data = std::make_shared<u_int32_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, int64_t>) {
            record->type = RecordType::SIGNED_INT_64;
            record->data = std::make_shared<int64_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, u_int64_t>) {
            record->type = RecordType::UNSIGNED_INT_64;
            record->data = std::make_shared<u_int64_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, float>) {
            record->type = RecordType::FLOAT_32;
            record->data = std::make_shared<float>(fieldValue);
        } else if constexpr(std::is_same_v<T, double>) {
            record->type = RecordType::FLOAT_64;
            record->data = std::make_shared<double>(fieldValue);
        } else if constexpr(nanoservices::Serializable<T>) {
            record->type = RecordType::SERIALIZABLE_CLASS;
            record->data = fieldValue.__nanoservices2_serializer_serialize();
        } else if constexpr(nanoservices::is_specialization<T, std::vector>::value) {
            return serializeListField(fieldName, fieldValue);
        } else if constexpr(nanoservices::is_specialization<T, std::map>::value) {
            return serializeMapField(fieldName, fieldValue);
        } else {
            record->type = RecordType::NULL_VALUE;
        }
        return record;
    }

    template<typename T>
    static std::shared_ptr<SerializationRecord> serializeField(const char *fieldName, T &fieldValue) {
        return serializeField(std::make_shared<std::string>(fieldName), fieldValue);
    }

    template<typename T>
    static void deserializeField(T *fieldValuePtr, std::shared_ptr<SerializationRecord> serializationRecord) {
        _initLogger();
        auto msgf = [fieldValuePtr, serializationRecord]() {
            std::stringstream msgSS;
            int demangleStatus = 0;
            char *fieldTypeName = abi::__cxa_demangle(typeid(*fieldValuePtr).name(), 0, 0, &demangleStatus);
            msgSS << "Deserializing an instance of " << fieldTypeName << ". Field name: " << *(serializationRecord->fieldName);
            ::free(fieldTypeName);
            return msgSS;
        };
        _logger->trace(msgf);

        if constexpr(std::is_same_v<T, std::string> || std::is_same_v<T, int8_t> || std::is_same_v<T, u_int8_t> || std::is_same_v<T, int16_t> || std::is_same_v<T, u_int16_t> ||
                     std::is_same_v<T, int32_t> || std::is_same_v<T, u_int32_t> || std::is_same_v<T, int64_t> || std::is_same_v<T, u_int64_t> || std::is_same_v<T, float> ||
                     std::is_same_v<T, double>) {
            *fieldValuePtr = *(std::static_pointer_cast<T>(serializationRecord->data));
        } else if constexpr(nanoservices::Serializable<T>) {
            fieldValuePtr->__nanoservices2_serializer_deserialize(std::static_pointer_cast<std::vector<std::shared_ptr<nanoservices::SerializationRecord>>>(serializationRecord->data));
        } else if constexpr(nanoservices::is_specialization<T, std::vector>::value) {
            deserializeListField(fieldValuePtr, serializationRecord);
        } else if constexpr(nanoservices::is_specialization<T, std::map>::value) {
            deserializeMapField(fieldValuePtr, serializationRecord);
        }
    }
};

} // namespace nanoservices

#endif // NANOSERVICES2_SERIALIZATION_H