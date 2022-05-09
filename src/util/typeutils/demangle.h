//
// Created by dmerkushov on 08.05.22.
//

#ifndef NANOSERVICES2_DEMANGLE_H
#define NANOSERVICES2_DEMANGLE_H
#include <memory>
#include <string>

namespace nanoservices::internal {

/**
 * @brief Demangle a mangled type name according to C++ mangling rules.
 * @param typeidName Value from <code>typeid(smth).name()</code>
 * @param canThrow If <code>true</code>, will log a warning message and throw a <code>ns_exception</code> in case of an error when demangling. If <code>false</code>, will only log a warning message:
 * the return value is equal to the input in this case.
 * @return The demangled type name, or the mangled type name in case of error
 */
std::shared_ptr<std::string> demangle_type_name(const char *typeidName, bool canThrow = false);

} // namespace nanoservices::internal

#endif // NANOSERVICES2_DEMANGLE_H
