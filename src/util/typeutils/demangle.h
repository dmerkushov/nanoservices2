//
// Created by dmerkushov on 08.05.22.
//

#ifndef NANOSERVICES2_DEMANGLE_H
#define NANOSERVICES2_DEMANGLE_H
#include <memory>
#include <string>

namespace nanoservices {

namespace internal {

std::shared_ptr<std::string> demangle_type_name(const char *typeid_name) noexcept;

}
} // namespace nanoservices

#endif // NANOSERVICES2_DEMANGLE_H
