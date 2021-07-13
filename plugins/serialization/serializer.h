//
// Created by dmerkushov on 12.07.2021.
//

#ifndef NANOSERVICES2_MARSHALLER_H
#define NANOSERVICES2_MARSHALLER_H

#include <memory>
#include <vector>

namespace nanoservices {

template<typename T>
concept Serializable = requires(T t) {
    { t.fieldGetterFunction() } -> std::function<std::vector<byte>(std::string)>;
    { t.fieldSetterFunction() } -> std::function<void(std::string, std::vector<byte>)>;
};

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
