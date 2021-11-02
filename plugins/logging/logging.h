//
// Created by dmerkushov on 3/20/21.
//

#ifndef LOGGING_H_
#define LOGGING_H_

#include "../../thirdparty/gabime/spdlog/spdlog.h"
#include "../../util/macroutils/position.h"
#include "../../util/stringutils/stringutils.h"
#include "../../util/templateutils/lambdaToFunction.h"

#include <cassert>
#include <chrono>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <type_traits>

namespace fmt {

///////////////////////////////////////////////////////////////////////////////
////  Formatting of strings returned from functions                        ////
///////////////////////////////////////////////////////////////////////////////

template<>
struct formatter<std::function<std::string()>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx);

    template<typename FormatContext>
    auto format(std::function<std::string()> const &e, FormatContext &ctx);
};

template<typename ParseContext>
constexpr auto formatter<std::function<std::string()>>::parse(ParseContext &ctx) {
    return std::begin(ctx);
}

template<typename FormatContext>
auto formatter<std::function<std::string()>>::format(std::function<std::string()> const &f, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "{}", f());
}

///////////////////////////////////////////////////////////////////////////////
////  Formatting of stringstreams returned from functions                  ////
///////////////////////////////////////////////////////////////////////////////

template<>
struct formatter<std::function<std::stringstream()>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx);

    template<typename FormatContext>
    auto format(std::function<std::stringstream()> const &e, FormatContext &ctx);
};

template<typename ParseContext>
constexpr auto formatter<std::function<std::stringstream()>>::parse(ParseContext &ctx) {
    return std::begin(ctx);
}

template<typename FormatContext>
auto formatter<std::function<std::stringstream()>>::format(std::function<std::stringstream()> const &f, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "{}", f().str());
}

} // namespace fmt

namespace nanoservices {

namespace log = spdlog;

void logging_initialize() noexcept;

void logging_finalize() noexcept;

log::level::level_enum logging_level_by_name(const char *name) noexcept;

/**
 * @brief The name of the default logging level
 * @details Level names are for spdlog. See spdlog documentation for details (available in nanoservices sources at thirdparty/gabime/spdlog/README.md)
 */
extern const std::string LOGGING_LEVELNAME_DEFAULT;

/**
 * @brief The default logging pattern
 * @details Patterns are set for spdlog. See spdlog documentation for details (available in nanoservices sources at thirdparty/gabime/spdlog/README.md)
 */
extern const std::string LOGGING_PATTERN_DEFAULT;

} // namespace nanoservices

#endif // LOGGING_H_
