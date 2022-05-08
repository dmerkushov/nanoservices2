//
// Created by dmerkushov on 08.05.22.
//

#include "demangle.h"

#include "../../plugins/logging/logging.h"

#include <cxxabi.h>

using namespace std;
using namespace nanoservices;

shared_ptr<string> nanoservices::internal::demangle_type_name(const char *typeid_name) noexcept {
    int demangleStatus = 0;
    char *callResult = abi::__cxa_demangle(typeid_name, nullptr, nullptr, &demangleStatus);
    shared_ptr<string> result;

    if(demangleStatus == 0) {
        log::trace("Demangling OK: \"{}\" -> \"{}\"", typeid_name, callResult);
        result = make_shared<string>(callResult);
    } else if(demangleStatus == -1) {
        log::warn("Demangling failed: \"{}\". Memory allocation failure", typeid_name);
        result = make_shared<string>(typeid_name);
    } else if(demangleStatus == -2) {
        log::warn("Demangling failed: \"{}\". Mangled name is not a valid name under the C++ mangling rules", typeid_name);
        result = make_shared<string>(typeid_name);
    } else if(demangleStatus == -3) {
        log::warn("Demangling failed: \"{}\". Invalid argument", typeid_name);
        result = make_shared<string>(typeid_name);
    } else {
        log::warn("Demangling failed: \"{}\". Unknown error", typeid_name);
        result = make_shared<string>(typeid_name);
    }

    return result;
}