//
// Created by dmerkushov on 12.07.2021.
//

#ifndef NANOSERVICES2_SERIALIZER_H
#define NANOSERVICES2_SERIALIZER_H

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

struct TypedRecord {
    RecordType type;
    std::string name;
    std::shared_ptr<void> record;
};

template<typename T>
concept Serializable = requires {
    { &T::__nanoservices2_serializer_serialize() } -> std::vector<std::shared_ptr<TypedRecord>>;
    { &T::__nanoservices2_serializer_deserialize(TypedRecord &) } -> void;
};

#ifndef NANOSERVICES2_PREPARE_SERIALIZABLE
#    define NANOSERVICES2_PREPARE_SERIALIZABLE(...) \
\
    public: \
        std::vector<std::shared_ptr<TypedRecord>> __nanoservices2_serializer_serialize() { \
            return nanoservices::serialize("__VA_ARGS__", __VA_ARGS__); \
        } \
        void __nanoservices2_serializer_deserialize(TypedRecord &typedRecord) { \
            // TODO Implement deserialization
}
#endif

template<Serializable S>
class SerializedEntity {
private:
public:
    SerializedEntity(std::shared_ptr<S> entity);

    SerializedEntity(std::shared_ptr<std::vector<byte>> &bytes);

    std::shared_ptr<std::vector<byte>> serialize();

    std::shared_ptr<S> deserialize();
};

class Serializer {
private:
public:
};

} // namespace nanoservices

#endif // NANOSERVICES2_SERIALIZER_H
