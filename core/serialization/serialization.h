//
// Created by dmerkushov on 12.07.2021.
//

#ifndef NANOSERVICES2_SERIALIZATION_H
#define NANOSERVICES2_SERIALIZATION_H

#include "../../plugins/logging/logging.h"
#include "../../thirdparty/swansontec/map-macro/map.h"
#include "../../util/macroutils/classname.h"
#include "../../util/stringutils/stringutils.h"
#include "../../util/templateutils/specialization.h"

#include <memory>
#include <string>
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
    LIST,
    MAP
};

struct SerializerRecord {
    RecordType type;
    std::shared_ptr<std::string> fieldName;
    std::shared_ptr<void> data;
};

// template<typename T>
// concept Serializable = requires {
//     {
//         &T::__nanoservices2_serializer_serialize()
//         } -> std::convertible_to<std::shared_ptr<std::vector<std::shared_ptr<SerializerRecord>>>>;
//     {
//         &T::__nanoservices2_serializer_deserialize(std::shared_ptr<std::vector<std::shared_ptr<SerializerRecord>>>)
//         } -> void;
// };

#define __NANOSERVICES2_SERIALIZE_FIELDCLAUSE__(FIELDNAME) \
    __nanoservices_logger->trace("+"); \
    result->push_back(nanoservices::Serializer::serializeField(*fieldNamesIter, (FIELDNAME))); \
    ++fieldNamesIter; \
    __nanoservices_logger->trace("-");

#define __NANOSERVICES2_DESERIALIZE_FIELDCLAUSE__(FIELDNAME) \
    __nanoservices_logger->trace("+"); \
    nanoservices::Serializer::deserializeField(&(FIELDNAME), *serializerRecordsIter); \
    ++serializerRecordsIter; \
    __nanoservices_logger->trace("-");

#define NANOSERVICES2_MAKE_SERIALIZABLE(...) \
\
private: \
    mutable std::shared_ptr<std::vector<std::shared_ptr<std::string>>> __nanoservices_fieldNamesVector; \
    std::shared_ptr<nanoservices::Logger> __nanoservices_logger = Logger::getLogger(); \
\
public: \
    std::shared_ptr<std::vector<std::shared_ptr<SerializerRecord>>> __nanoservices_serialize() { \
        __nanoservices_logger->debug("+__nanoservices_serialize()"); \
        if(!__nanoservices_fieldNamesVector) { \
            const char *fieldNames = #__VA_ARGS__; \
            __nanoservices_fieldNamesVector = \
                    nanoservices::splitString(std::make_shared<std::string>(fieldNames), ',', true, false); \
        } \
        auto result = make_shared<std::vector<std::shared_ptr<SerializerRecord>>>(); \
        auto fieldNamesIter = __nanoservices_fieldNamesVector->begin(); \
        MAP(__NANOSERVICES2_SERIALIZE_FIELDCLAUSE__, __VA_ARGS__) \
        __nanoservices_logger->debug("-__nanoservices_serialize()"); \
        return result; \
    } \
\
    void __nanoservices_deserialize( \
            std::shared_ptr<std::vector<std::shared_ptr<SerializerRecord>>> serializerRecords) { \
        __nanoservices_logger->debug("+__nanoservices_deserialize()"); \
        auto serializerRecordsIter = serializerRecords->begin(); \
        MAP(__NANOSERVICES2_DESERIALIZE_FIELDCLAUSE__, __VA_ARGS__) \
        __nanoservices_logger->debug("-__nanoservices_deserialize()"); \
    }

class Serializer {
private:
    static std::shared_ptr<std::string> _mapItemKeyRecordName;
    static std::shared_ptr<std::string> _mapItemValueRecordName;
    static std::shared_ptr<std::string> _listItemRecordName;

public:
    template<typename T>
    static std::shared_ptr<SerializerRecord> serializeListField(std::shared_ptr<std::string> fieldName,
                                                                std::vector<T> &fieldValue) {
        auto record = std::make_shared<SerializerRecord>();

        auto __nanoservices_logger = Logger::getLogger();
        __nanoservices_logger->trace("serializeListField(): serializing");
        std::string a(typeid(T).name());
        __nanoservices_logger->trace(a);

        record->fieldName = fieldName;
        if constexpr(std::is_same_v<T, std::vector<char>>) {
            record->type = RecordType::BYTE_ARRAY;
            record->data = std::make_shared<std::vector<char>>(fieldValue);
        } else if constexpr(std::is_same_v<T, std::shared_ptr<std::vector<char>>>) {
            record->type = RecordType::BYTE_ARRAY;
            record->data = fieldValue;
        } else {
            record->type = RecordType::LIST;
            record->data = std::make_shared<std::vector<std::shared_ptr<SerializerRecord>>>();

            auto data = std::static_pointer_cast<std::vector<std::shared_ptr<SerializerRecord>>>(record->data);

            for(auto iter = fieldValue.begin(); iter != fieldValue.end(); ++iter) {
                data->push_back(serializeField(_listItemRecordName, *iter));
            }
        }

        return record;
    }

    /**
     * @brief Serialization of a map
     * @tparam V
     * @param fieldName
     * @param fieldValue
     * @return
     */
    template<typename K, typename V>
    static std::shared_ptr<SerializerRecord> serializeMapField(std::shared_ptr<std::string> fieldName,
                                                               std::map<K, V> &fieldValue) {
        auto record = std::make_shared<SerializerRecord>();
        record->fieldName = fieldName;

        record->type = RecordType::MAP;
        record->data = std::make_shared<std::vector<std::shared_ptr<SerializerRecord>>>();
        auto data = std::static_pointer_cast<std::vector<std::shared_ptr<SerializerRecord>>>(record->data);

        for(typename std::map<K, V>::iterator iter = fieldValue.begin(); iter != fieldValue.end(); ++iter) {
            data->push_back(serializeField(_mapItemKeyRecordName, iter->first));
            data->push_back(serializeField(_mapItemValueRecordName, iter->second));
        }
        return record;
    }

    template<typename T>
    static std::shared_ptr<SerializerRecord> serializeField(std::shared_ptr<std::string> fieldName, T &fieldValue) {
        auto record = std::make_shared<SerializerRecord>();
        record->fieldName = fieldName;
        if constexpr(std::is_same_v<T, std::string>) {
            record->type = RecordType::STRING;
            record->data = std::make_shared<std::string>(fieldValue);
        } else if constexpr(std::is_same_v<T, std::shared_ptr<std::string>>) {
            record->type = RecordType::STRING;
            record->data = fieldValue;
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
    static std::shared_ptr<SerializerRecord> serializeField(const char *fieldName, T &fieldValue) {
        return serializeField(std::make_shared<std::string>(fieldName), fieldValue);
    }

    template<typename T>
    static void deserializeField(T *fieldPtr, std::shared_ptr<SerializerRecord> serializerRecord) {
        if constexpr(std::is_same_v<T, int32_t>) {
            *fieldPtr = *(std::static_pointer_cast<int32_t>(serializerRecord->data));
        }
    }
};

} // namespace nanoservices

#endif // NANOSERVICES2_SERIALIZATION_H
