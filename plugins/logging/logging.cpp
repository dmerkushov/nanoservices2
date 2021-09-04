//
// Created by dmerkushov on 3/20/21.
//

#include "logging.h"

#include "../configuration/configuration.h"

#include <mutex>

using namespace std;
using namespace nanoservices;

namespace nanoservices {
log::level::level_enum logging_level_by_name(const char *name) noexcept {
    string nameStr(name);
    auto trimmedAndUppered = str_toUpper_copy(*str_trim_copy(nameStr));

    if(*trimmedAndUppered == "OFF") {
        return log::level::off;
    }
    if((*trimmedAndUppered == "FATAL") || (*trimmedAndUppered == "CRITICAL")) {
        return log::level::critical;
    }
    if((*trimmedAndUppered == "ERROR") || (*trimmedAndUppered == "ERR")) {
        return log::level::err;
    }
    if(*trimmedAndUppered == "WARN") {
        return log::level::warn;
    }
    if(*trimmedAndUppered == "INFO") {
        return log::level::info;
    }
    if(*trimmedAndUppered == "DEBUG") {
        return log::level::debug;
    }
    if((*trimmedAndUppered == "TRACE") || (*trimmedAndUppered == "ALL")) {
        return log::level::trace;
    }

    // In case none
    return logging_level_by_name(LOGGING_LEVELNAME_DEFAULT.c_str());
}
} // namespace nanoservices

const string nanoservices::LOGGING_LEVELNAME_DEFAULT = "info";
const string nanoservices::LOGGING_PATTERN_DEFAULT = "[%Y-%m-%d %H:%M:%S.%F] [thread %t] %^[%l]%$ %v";