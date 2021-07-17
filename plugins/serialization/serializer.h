//
// Created by dmerkushov on 12.07.2021.
//

#ifndef NANOSERVICES2_SERIALIZER_H
#define NANOSERVICES2_SERIALIZER_H

#include "../../thirdparty/swansontec/map-macro/map.h"

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
    FLOAT,
    DOUBLE,
    ENUM_VALUE,
    LIST,
    MAP
};

// TODO Try https://stackoverflow.com/questions/1872220/is-it-possible-to-iterate-over-arguments-in-variadic-macros
// Add something like macroutils.h for these macro tricks

struct SerializerRecord {
    RecordType type;
    std::shared_ptr<void> data;
};

template<typename T>
concept Serializable = requires {
    {
        &T::__nanoservices2_serializer_serialize()
        } -> std::convertible_to<std::shared_ptr<std::vector<std::shared_ptr<SerializerRecord>>>>;
    {
        &T::__nanoservices2_serializer_deserialize(std::shared_ptr<std::vector<std::shared_ptr<SerializerRecord>>>)
        } -> void;
};

#ifndef NANOSERVICES2_MAKE_SERIALIZABLE
#    define NANOSERVICES2_MAKE_SERIALIZABLE(...) \
\
    public: \
        std::shared_ptr<std::vector<std::shared_ptr<SerializerRecord>>> __nanoservices2_serializer_serialize() { \
            return CALL_SERIALIZE(__VA_ARGS__); \
        } \
        template<typename... FieldTypes> \
        void __nanoservices2_serializer_doDeserialize(FieldTypes) void __nanoservices2_serializer_deserialize( \
                std::shared_ptr<std::vector<std::shared_ptr<SerializerRecord>>> serializedVector) { \
            return nanoservices::Serializer::deserialize(serializedVector, __VA_ARGS__); \
        }
#endif
// TODO Implement deserialization

class Serializer {
private:
    template<typename T>
    static std::shared_ptr<SerializerRecord> serializeField(T fieldValue) {
        auto record = std::make_shared<SerializerRecord>();
        if constexpr(std::is_same_v<T, int32_t>) {
            record->type = RecordType::SIGNED_INT_32;
            record->data = std::make_shared<int32_t>(fieldValue);
        }
        return record;
    }

    template<typename T>
    static void deserializeField(T *fieldPtr, std::shared_ptr<SerializerRecord> serializerRecord) {
        if constexpr(std::is_same_v<T, int32_t>) {
            *fieldPtr = *(std::static_pointer_cast<int32_t>(serializerRecord->data));
        }
    }

public:
    /**
     * @brief A recursive template: prepare a vector of SerializerRecord containing the otherFields to serialize
     * @details Fields are put into the vector in reverse order, so they must be read in reverse order too
     * @tparam CurrentFieldType
     * @tparam OtherFieldTypes
     * @param currentField
     * @param otherFields
     * @return
     */
    template<typename CurrentFieldType, typename... OtherFieldTypes>
    static std::shared_ptr<std::vector<std::shared_ptr<SerializerRecord>>> serialize(
            CurrentFieldType currentField, OtherFieldTypes... otherFields) throw() {
        std::shared_ptr<std::vector<std::shared_ptr<SerializerRecord>>> result;
        if constexpr(sizeof...(OtherFieldTypes) == 0) {
            result = std::make_shared<std::vector<std::shared_ptr<SerializerRecord>>>();
        } else {
            result = serialize(otherFields...);
        }
        result->push_back(serializeField(currentField));
        return result;
    }

    template<typename CurrentFieldType, typename... OtherFieldTypes>
    static void deserialize(std::shared_ptr<std::vector<std::shared_ptr<SerializerRecord>>> serializedVector,
                            CurrentFieldType *currentField,
                            OtherFieldTypes *...otherFields) throw() {

        if(serializedVector->size() <= 0) {
            // TODO Throw an exception here
            return;
        }

        std::shared_ptr<SerializerRecord> currentRecord = serializedVector->back();
        deserializeField(currentField, currentRecord);
        serializedVector->pop_back();

        if constexpr(sizeof...(OtherFieldTypes) > 0) {
            deserialize(serializedVector, otherFields...);
        }
    }
};

} // namespace nanoservices

#endif // NANOSERVICES2_SERIALIZER_H
