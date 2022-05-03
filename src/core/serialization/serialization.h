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

enum class record_type {
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
const char *get_record_type_name(record_type recordType);

/**
 * @brief Append the record type's name to the output stream
 * @param os
 * @param recordType
 * @return
 */
std::ostream &operator<<(std::ostream &os, const record_type &recordType);

/**
 * @brief A record in the serialization sequence
 */
struct serialization_record {
    /**
     * @brief Type of the record
     */
    record_type type;

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
                           { t.__nanoservices2_serializer_serialize() } -> std::convertible_to<std::shared_ptr<std::vector<std::shared_ptr<serialization_record>>>>;
                           t.__nanoservices2_serializer_deserialize(t.__nanoservices2_serializer_serialize());
                       };

#define __NANOSERVICES2_SERIALIZE_FIELDCLAUSE__(FIELDNAME) \
    result->push_back(nanoservices::serializer::serialize_field(*fieldNamesIter, (FIELDNAME))); \
    ++fieldNamesIter;

#define __NANOSERVICES2_DESERIALIZE_FIELDCLAUSE__(FIELDNAME) \
    nanoservices::serializer::deserialize_field(&(FIELDNAME), *serializerRecordsIter); \
    ++serializerRecordsIter;

// SonarLint will argue: "Each main_instance of a parameter in a function-like macro should be enclosed in parentheses" in the body of this macro.
// But what we need is actually using it without parentheses!
#define NANOSERVICES2_MAKE_SERIALIZABLE(...) \
\
private: \
    mutable std::shared_ptr<std::vector<std::shared_ptr<std::string>>> __nanoservices2_fieldNamesVector; \
\
public: \
    std::shared_ptr<std::vector<std::shared_ptr<serialization_record>>> __nanoservices2_serializer_serialize() { \
        if(!__nanoservices2_fieldNamesVector) { \
            const char *fieldNames = #__VA_ARGS__; \
            __nanoservices2_fieldNamesVector = nanoservices::splitString(std::make_shared<std::string>(fieldNames), ',', true, false); \
        } \
        auto result = make_shared<std::vector<std::shared_ptr<serialization_record>>>(); \
        auto fieldNamesIter = __nanoservices2_fieldNamesVector->begin(); \
        DO_FOREACH(__NANOSERVICES2_SERIALIZE_FIELDCLAUSE__, __VA_ARGS__) \
        return result; \
    } \
\
    void __nanoservices2_serializer_deserialize(std::shared_ptr<std::vector<std::shared_ptr<serialization_record>>> serializerRecords) { \
        auto serializerRecordsIter = serializerRecords->begin(); \
        DO_FOREACH(__NANOSERVICES2_DESERIALIZE_FIELDCLAUSE__, __VA_ARGS__) \
    }

class serializer {
private:
    static std::shared_ptr<std::string> _map_item_key_record_name;
    static std::shared_ptr<std::string> _map_item_value_record_name;
    static std::shared_ptr<std::string> _list_item_record_name;

    static void _init_log();

public:
    template<typename T>
    static std::shared_ptr<serialization_record> serialize_list_field(std::shared_ptr<std::string> fieldName, std::vector<T> &fieldValue) {
        auto record = std::make_shared<serialization_record>();

        record->fieldName = fieldName;
        if constexpr(std::is_same_v<T, std::vector<char>>) {
            record->type = record_type::BYTE_ARRAY;
            record->data = std::make_shared<std::vector<char>>(fieldValue);
        } else {
            record->type = record_type::LIST;
            record->data = std::make_shared<std::vector<std::shared_ptr<serialization_record>>>();

            auto data = std::static_pointer_cast<std::vector<std::shared_ptr<serialization_record>>>(record->data);

            for(auto iter = fieldValue.begin(); iter != fieldValue.end(); ++iter) {
                data->push_back(serialize_field(_list_item_record_name, *iter));
            }
        }

        return record;
    }

    template<typename T>
    static void deserialize_list_field(std::vector<T> *field_value_ptr, std::shared_ptr<serialization_record> serializationRecord) {
        auto recordsVector = std::static_pointer_cast<std::vector<std::shared_ptr<serialization_record>>>(serializationRecord->data);

        for(auto iter = recordsVector->begin(); iter != recordsVector->end(); ++iter) {
            T t;
            deserialize_field(&t, *iter);
            field_value_ptr->push_back(t);
        }
    }

    /**
     * @brief Serialize a map
     * @tparam K type of keys of a map. Must be Serializable
     * @tparam V
     * @param fieldName
     * @param fieldValue
     * @return
     */
    template<typename K, typename V>
    static std::shared_ptr<serialization_record> serialize_map_field(std::shared_ptr<std::string> fieldName, std::map<K, V> &fieldValue) {
        auto record = std::make_shared<serialization_record>();
        record->fieldName = fieldName;

        record->type = record_type::MAP;
        record->data = std::make_shared<std::vector<std::shared_ptr<serialization_record>>>();
        auto data = std::static_pointer_cast<std::vector<std::shared_ptr<serialization_record>>>(record->data);

        for(auto const &[k, v] : fieldValue) {
            // k and v have the 'const K' and 'const V' types, which are not supported by serialize_field(), as a const
            // field cannot be deserialized
            K key = k;
            std::shared_ptr<serialization_record> keyRecord = serialize_field(_map_item_key_record_name, key);
            data->push_back(keyRecord);

            V value = v;
            std::shared_ptr<serialization_record> valueRecord = serialize_field(_map_item_value_record_name, value);
            data->push_back(valueRecord);
        }
        return record;
    }

    template<typename K, typename V>
    static void deserialize_map_field(std::map<K, V> *fieldValuePtr, std::shared_ptr<serialization_record> serializationRecord) {
        auto recordsVector = std::static_pointer_cast<std::vector<std::shared_ptr<serialization_record>>>(serializationRecord->data);

        auto iter = recordsVector->begin();
        while(iter != recordsVector->end()) {
            K k;
            deserialize_field(&k, *iter);

            ++iter;
            if(iter == recordsVector->end()) {
                NS_THROW(fmt::format("Reached the end of the serialization record vector when deserializing map field {}", serializationRecord->fieldName).c_str());
            }

            V v;
            deserialize_field(&v, *iter);

            (*fieldValuePtr)[k] = v;

            ++iter;
        }
    }

    template<typename T>
    static std::shared_ptr<serialization_record> serialize_field(std::shared_ptr<std::string> fieldName, T &fieldValue) {
        _init_log();
        if(log::should_log(log::level::trace)) {
            int demangleStatus = 0;
            char *fieldTypeName = abi::__cxa_demangle(typeid(fieldValue).name(), nullptr, nullptr, &demangleStatus);
            if(demangleStatus == 0) {
                log::trace("Serializing an main_instance of {}. Field name: {}", fieldTypeName, fieldName);
            } else if(demangleStatus == -1) {
                log::trace("Serializing an main_instance of (unknown type: memory allocation failure). Field name: {}", fieldName);
            } else if(demangleStatus == -2) {
                log::trace(
                        "Serializing an main_instance of (unknown type: mangled name \"{}\" is not a valid name under the C++ mangling rules). Field name: {}", typeid(fieldValue).name(), fieldName);
            } else if(demangleStatus == -3) {
                log::trace("Serializing an main_instance of (unknown type: invalid argument). Field name: {}", fieldName);
            } else {
                log::trace("Serializing an main_instance of (unknown type: unknown error). Field name: {}", fieldName);
            }
        }

        auto record = std::make_shared<serialization_record>();
        record->fieldName = fieldName;
        if constexpr(std::is_same_v<T, std::string>) {
            record->type = record_type::STRING;
            record->data = std::make_shared<std::string>(fieldValue);
        } else if constexpr(std::is_same_v<T, int8_t>) {
            record->type = record_type::SIGNED_INT_8;
            record->data = std::make_shared<int8_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, u_int8_t>) {
            record->type = record_type::UNSIGNED_INT_8;
            record->data = std::make_shared<u_int8_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, int16_t>) {
            record->type = record_type::SIGNED_INT_16;
            record->data = std::make_shared<int16_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, u_int16_t>) {
            record->type = record_type::UNSIGNED_INT_16;
            record->data = std::make_shared<u_int16_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, int32_t>) {
            record->type = record_type::SIGNED_INT_32;
            record->data = std::make_shared<int32_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, u_int32_t>) {
            record->type = record_type::UNSIGNED_INT_32;
            record->data = std::make_shared<u_int32_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, int64_t>) {
            record->type = record_type::SIGNED_INT_64;
            record->data = std::make_shared<int64_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, u_int64_t>) {
            record->type = record_type::UNSIGNED_INT_64;
            record->data = std::make_shared<u_int64_t>(fieldValue);
        } else if constexpr(std::is_same_v<T, float>) {
            record->type = record_type::FLOAT_32;
            record->data = std::make_shared<float>(fieldValue);
        } else if constexpr(std::is_same_v<T, double>) {
            record->type = record_type::FLOAT_64;
            record->data = std::make_shared<double>(fieldValue);
        } else if constexpr(nanoservices::Serializable<T>) {
            record->type = record_type::SERIALIZABLE_CLASS;
            record->data = fieldValue.__nanoservices2_serializer_serialize();
        } else if constexpr(nanoservices::is_specialization<T, std::vector>::value) {
            return serialize_list_field(fieldName, fieldValue);
        } else if constexpr(nanoservices::is_specialization<T, std::map>::value) {
            return serialize_map_field(fieldName, fieldValue);
        } else {
            record->type = record_type::NULL_VALUE;
        }
        return record;
    }

    template<typename T>
    static std::shared_ptr<serialization_record> serialize_field(const char *fieldName, T &fieldValue) {
        return serialize_field(std::make_shared<std::string>(fieldName), fieldValue);
    }

    template<typename T>
    static void deserialize_field(T *fieldValuePtr, std::shared_ptr<serialization_record> serializationRecord) {
        _init_log();
        if(log::should_log(log::level::trace)) {
            int demangleStatus = 0;
            size_t length = 0;
            std::shared_ptr<char> fieldTypeName(abi::__cxa_demangle(typeid(fieldValuePtr).name(), nullptr, &length, &demangleStatus));
            if(demangleStatus == 0) {
                log::trace("Deserializing an main_instance of {}. Field name: {}", fieldTypeName.get(), serializationRecord->fieldName);
            } else if(demangleStatus == -1) {
                log::trace("Deserializing an main_instance of (unknown type: memory allocation failure). Field name: {}", serializationRecord->fieldName);
            } else if(demangleStatus == -2) {
                log::trace("Deserializing an main_instance of (unknown type: mangled name \"{}\" is not a valid name under the C++ mangling rules). Field name: {}",
                           typeid(fieldValuePtr).name(),
                           serializationRecord->fieldName);
            } else if(demangleStatus == -3) {
                log::trace("Deserializing an main_instance of (unknown type: invalid argument). Field name: {}", serializationRecord->fieldName);
            } else {
                log::trace("Deserializing an main_instance of (unknown type: unknown error). Field name: {}", serializationRecord->fieldName);
            }
        }

        if constexpr(std::is_same_v<T, std::string> || std::is_same_v<T, int8_t> || std::is_same_v<T, u_int8_t> || std::is_same_v<T, int16_t> || std::is_same_v<T, u_int16_t> ||
                     std::is_same_v<T, int32_t> || std::is_same_v<T, u_int32_t> || std::is_same_v<T, int64_t> || std::is_same_v<T, u_int64_t> || std::is_same_v<T, float> ||
                     std::is_same_v<T, double>) {
            *fieldValuePtr = *(std::static_pointer_cast<T>(serializationRecord->data));
        } else if constexpr(nanoservices::Serializable<T>) {
            fieldValuePtr->__nanoservices2_serializer_deserialize(std::static_pointer_cast<std::vector<std::shared_ptr<nanoservices::serialization_record>>>(serializationRecord->data));
        } else if constexpr(nanoservices::is_specialization<T, std::vector>::value) {
            deserialize_list_field(fieldValuePtr, serializationRecord);
        } else if constexpr(nanoservices::is_specialization<T, std::map>::value) {
            deserialize_map_field(fieldValuePtr, serializationRecord);
        }
    }
};

} // namespace nanoservices

#endif // NANOSERVICES2_SERIALIZATION_H
