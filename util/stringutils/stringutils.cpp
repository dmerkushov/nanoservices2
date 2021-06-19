//
// Created by dmerkushov on 15.06.2021.
//

#include "stringutils.h"

#include <algorithm>
#include <locale>

using namespace nanoservices;
using namespace std;

shared_ptr<string> nanoservices::str_trim_copy(shared_ptr<string> str) noexcept {
    const auto strBegin = str->find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return make_shared<string>(""); // no content

    const auto strEnd = str->find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    string *trimmed = new string(str->substr(strBegin, strRange));

    return shared_ptr<string>(trimmed);
}

shared_ptr<string> nanoservices::str_toUpper_copy(shared_ptr<string> str) noexcept {
    locale locale;
    auto to_upper = [&locale](char ch) { return use_facet<ctype<char> >(locale).toupper(ch); };

    string *uppered = new string(*str);

    std::transform(uppered->begin(), uppered->end(), uppered->begin(), to_upper);

    return shared_ptr<string>(uppered);
}