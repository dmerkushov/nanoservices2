//
// Created by dmerkushov on 08.05.22.
//

#include "demangle.h"

#include "../../core/exception/ns_exception.h"
#include "../../plugins/logging/logging.h"

#include <cxxabi.h>

using namespace std;
using namespace nanoservices;

shared_ptr<string> nanoservices::internal::demangle_type_name(const char *typeidName, bool canThrow) {
    int demangleStatus = 0;
    char *callResult = abi::__cxa_demangle(typeidName, nullptr, nullptr, &demangleStatus);
    shared_ptr<string> result;

    if(demangleStatus == 0) {
        log::trace("Demangling OK: \"{}\" -> \"{}\"", typeidName, callResult);
        result = make_shared<string>(callResult);
    } else {
        auto warnAndThrow = [typeidName, canThrow, &result](const char *format, shared_ptr<string> position) {
            string formatStr(format);
            string msg;
            if(formatStr.find("{}") != string::npos) {
                msg = fmt::format(fmt::runtime(formatStr), typeidName);
            } else {
                msg = formatStr;
            }

            log::warn(msg);

            if(canThrow) {
                throw ns_exception(msg.c_str(), position);
            }

            result = make_shared<string>(typeidName);
        };

        if(demangleStatus == -1) {
            warnAndThrow("Demangling failed: memory allocation failure", NS_POSITION_SHAREDPTR);
        } else if(demangleStatus == -2) {
            warnAndThrow("Demangling failed: \"{}\". Mangled name is not a valid name under the C++ mangling rules", NS_POSITION_SHAREDPTR);
        } else if(demangleStatus == -3) {
            warnAndThrow("Demangling failed: \"{}\". Invalid argument", NS_POSITION_SHAREDPTR);
        } else {
            warnAndThrow("Demangling failed: \"{}\". Unknown error", NS_POSITION_SHAREDPTR);
        }
    }

    return result;
}