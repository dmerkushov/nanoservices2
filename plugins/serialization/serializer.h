//
// Created by dmerkushov on 12.07.2021.
//

#ifndef NANOSERVICES2_MARSHALLER_H
#define NANOSERVICES2_MARSHALLER_H

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
    RECORD_LIST,
    RECORD_MAP
};

template<typename T>
struct TypedRecord {
    RecordType type;
    std::string name;
    std::shared_ptr<T> record;
};

// TODO Serializable concept

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

#endif // NANOSERVICES2_MARSHALLER_H
