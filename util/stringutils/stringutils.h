//
// Created by dmerkushov on 15.06.2021.
//

#ifndef NANOSERVICES2_STRINGUTILS_H
#define NANOSERVICES2_STRINGUTILS_H

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
concept String = requires {
    requires std::is_convertible_v<MT, std::string>;
};

/**
 * @brief The concept requires that the given type is std::stringstream
 * @tparam MT
 */
template<typename MT>
concept SStream = requires {
    requires std::is_same_v<MT, std::stringstream>;
};

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
 * @param dropEmptyTokens if true, will not include empty tokens (due to delimiters beside each other, or due to
 * delimiters at the start or the end of the input string) in the result. The empty-check is performed after trimming
 * the token.
 * @return A shared pointer to a vector of found tokens
 */
std::shared_ptr<std::vector<std::shared_ptr<std::string>>> splitString(std::shared_ptr<std::string> in,
                                                                       char delimiter = ' ',
                                                                       bool trimTokens = false,
                                                                       bool dropEmptyTokens = false);

} // namespace nanoservices

#endif // NANOSERVICES2_STRINGUTILS_H
