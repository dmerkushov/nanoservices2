//
// Created by dmerkushov on 15.06.2021.
//

#ifndef NANOSERVICES2_STRINGUTILS_H
#define NANOSERVICES2_STRINGUTILS_H

#include "../../thirdparty/gabime/spdlog/fmt/fmt.h"

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace nanoservices {

/**
 * Whitespace chars to be used
 */
static const std::string whitespace(" \t");

/**
 * Trim a string (remove leading and trailing whitespaces: spaces and tabs). Quite slow (may be rewritten for better
 * performance)
 * @return
 */
std::shared_ptr<std::string> str_trim_copy(std::string_view str) noexcept;

/**
 * Convert string to uppercase. Quite slow (may be rewritten for better performance)
 * @return
 */
std::shared_ptr<std::string> str_toUpper_copy(std::string_view str) noexcept;

/**
 * @brief The concept requires that the given type is convertible to std::string
 * @tparam MT
 */
template<typename MT>
concept String = requires { requires std::is_convertible_v<MT, std::string>; };

/**
 * @brief The concept requires that the given type is std::stringstream
 * @tparam MT
 */
template<typename MT>
concept SStream = requires { requires std::is_same_v<MT, std::stringstream>; };

/**
 * @brief The concept requires that the given type is either std::stringstream or convertible to std::string
 * @tparam MT
 * @see nanoservices::String
 * @see nanoservices::SStream
 */
template<typename MT>
concept StringOrSStream = String<MT> || SStream<MT>;

/**
 * @brief The concept requires that the given type denotes a callable entity that produces a std::string when calling
 * without parameters
 * @tparam MT
 */
template<class MT>
concept StringProducer = requires(MT mt) {
                             { mt() } -> String;
                         };

/**
 * @brief The concept requires that the given type denotes a callable entity that produces a std::stringstream when
 * calling without parameters
 * @tparam MT
 */
template<class MT>
concept SStreamProducer = requires(MT mt) {
                              { mt() } -> SStream;
                          };

/**
 * @brief The concept requires that the given type denotes a callable entity that produces either a std::stringstream or
 * anything convertible to std::string when calling without parameters
 * @tparam MT
 */
template<class MT>
concept StringOrSStreamProducer = StringProducer<MT> || SStreamProducer<MT>;

/**
 * @brief Split a string by delimiting char
 * @param in input string
 * @param delimiter
 * @param trimTokens if true, will trim the tokens
 * @param dropEmptyTokens if true, the result will not contain empty tokens (due to delimiters beside each other, or due to
 * delimiters at the start or the end of the input string) in the result. The empty-check is performed after trimming
 * the token.
 * @return A shared pointer to a vector of found tokens
 */
std::shared_ptr<std::vector<std::shared_ptr<std::string>>> splitString(std::shared_ptr<std::string> in, char delimiter = ' ', bool trimTokens = false, bool dropEmptyTokens = false);

} // namespace nanoservices

namespace fmt {

/**
 * @brief fmt library formatter for <code>std::shared_ptr&lt;std::string></code>
 */
template<>
struct formatter<std::shared_ptr<std::string>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx);

    template<typename FormatContext>
    auto format(std::shared_ptr<std::string> const &e, FormatContext &ctx);
};

/**
 * @brief Implementation of the <code>parse</code> method for the fmt library formatter for <code>std::shared_ptr&lt;std::string></code>
 * @tparam ParseContext
 * @param ctx
 * @return
 */
template<typename ParseContext>
constexpr auto formatter<std::shared_ptr<std::string>>::parse(ParseContext &ctx) {
    return std::begin(ctx);
}

/**
 * @brief Implementation of the <code>format</code> method for the fmt library formatter for <code>std::shared_ptr&lt;std::string></code>
 * @tparam ParseContext
 * @param ctx
 * @return
 */
template<typename FormatContext>
auto formatter<std::shared_ptr<std::string>>::format(std::shared_ptr<std::string> const &sps, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "{}", *sps);
}

template<>
struct formatter<std::vector<std::string>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx);

    template<typename FormatContext>
    auto format(std::vector<std::string> const &e, FormatContext &ctx);
};

template<typename ParseContext>
constexpr auto formatter<std::vector<std::string>>::parse(ParseContext &ctx) {
    return std::begin(ctx);
}

template<typename FormatContext>
auto formatter<std::vector<std::string>>::format(std::vector<std::string> const &vec, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "[{}]", vec.empty() ? "" : fmt::format("\"{}\"", fmt::join(vec, "\",\"")));
}

template<>
struct formatter<std::vector<std::shared_ptr<std::string>>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx);

    template<typename FormatContext>
    auto format(std::vector<std::shared_ptr<std::string>> const &e, FormatContext &ctx);
};

template<typename ParseContext>
constexpr auto formatter<std::vector<std::shared_ptr<std::string>>>::parse(ParseContext &ctx) {
    return std::begin(ctx);
}

template<typename FormatContext>
auto formatter<std::vector<std::shared_ptr<std::string>>>::format(std::vector<std::shared_ptr<std::string>> const &vec, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "[{}]", vec.empty() ? "" : fmt::format("\"{}\"", fmt::join(vec, "\",\"")));
}

} // namespace fmt

#endif // NANOSERVICES2_STRINGUTILS_H
