//
// Created by dmerkushov on 15.06.2021.
//

#ifndef NANOSERVICES2_STRINGUTILS_H
#define NANOSERVICES2_STRINGUTILS_H

#include <memory>
#include <string>

namespace nanoservices {

    /**
     * Whitespace chars to be used
     */
    static const std::string whitespace(" \t");

/**
 * Trim a string (remove leading and trailing whitespaces: spaces and tabs). Quite slow (may be rewritten for better performance)
 * @return
 */
    std::shared_ptr<std::string> str_trim_copy(std::shared_ptr<std::string>) noexcept;

/**
 * Convert string to uppercase. Quite slow (may be rewritten for better performance)
 * @return
 */
    std::shared_ptr<std::string> str_toUpper_copy(std::shared_ptr<std::string>) noexcept;

}

#endif //NANOSERVICES2_STRINGUTILS_H
