//
// Created by dmerkushov on 15.06.2021.
//

#ifndef NANOSERVICES2_STRINGUTILS_H
#define NANOSERVICES2_STRINGUTILS_H

#include <memory>
#include <sstream>
#include <string>

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
std::shared_ptr<std::string> str_trim_copy(std::shared_ptr<std::string>) noexcept;

/**
 * Convert string to uppercase. Quite slow (may be rewritten for better performance)
 * @return
 */
std::shared_ptr<std::string> str_toUpper_copy(std::shared_ptr<std::string>) noexcept;

/**
 * @brief For use in static asserts: check whether the operand type is either string or stringstream
 * @tparam ToCheck
 */
template<typename ToCheck>
constexpr bool is_string_or_stringstream =
        std::is_same_v<ToCheck, std::string> || std::is_same_v<ToCheck, std::stringstream>;
;
} // namespace nanoservices

#endif // NANOSERVICES2_STRINGUTILS_H
