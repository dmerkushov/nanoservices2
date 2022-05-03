//
// Created by dmerkushov on 15.06.2021.
//

#include "stringutils.h"

#include <algorithm>
#include <locale>
#include <ranges>

using namespace nanoservices;
using namespace std;

shared_ptr<string> nanoservices::str_trim_copy(string_view str) noexcept {
    const auto strBegin = str.find_first_not_of(whitespace);
    if(strBegin == string::npos) {
        return make_shared<string>(""); // no content
    }

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    auto trimmed = make_shared<string>(str.substr(strBegin, strRange));

    return trimmed;
}

shared_ptr<string> nanoservices::str_toUpper_copy(string_view str) noexcept {
    locale locale;

    auto uppered = make_shared<string>(str);

    ranges::transform(uppered->begin(), uppered->end(), uppered->begin(), [&locale](char ch) {
        return use_facet<ctype<char>>(locale).toupper(ch);
    });

    return uppered;
}

shared_ptr<vector<shared_ptr<string>>> nanoservices::splitString(std::shared_ptr<std::string> in, char delimiter, bool trimTokens, bool dropEmptyTokens) {
    stringstream ss(*in);
    string token;
    auto result = make_shared<vector<shared_ptr<string>>>();
    while(getline(ss, token, delimiter)) {
        if(trimTokens) {
            token = *(str_trim_copy(token));
        }
        if(dropEmptyTokens && token.empty()) {
            continue;
        }
        result->push_back(make_shared<string>(token));
    }

    return result;
}