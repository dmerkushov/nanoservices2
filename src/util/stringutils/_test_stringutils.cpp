#include "../../plugins/logging/logging.h"
#include "../testing/testing.h"
#include "stringutils.h"

using namespace std;
using namespace nanoservices;

static const std::function<bool()> test_strutils_fmt_sharedptrstring_formatter = []() {
    auto str = make_shared<string>("Ololo");
    string str2 = fmt::format("{}", str);
    return (str->compare(str2) == 0);
};

NSTEST_ADD(test_strutils_fmt_sharedptrstring_formatter)

static const std::function<bool()> test_strutils_splitString = []() {
    auto str = make_shared<string>("My String  is quite long, and good enough");

    bool result = true;
    log::debug("Input string: \"{}\"", str);

    auto check_against = [](vector<string> check, shared_ptr<vector<shared_ptr<string>>> in) {
        if(check.size() != in->size()) {
            return false;
        }
        for(auto i = 0; i < check.size(); i++) {
            auto member = check[i];
            if(member.compare(*(in->operator[](i))) != 0) {
                return false;
            }
        }
        return true;
    };

    auto check = [&str, &result, &check_against](char delimiter, bool trimTokens, bool dropEmptyTokens, vector<string> check) {
        auto split = splitString(str, delimiter, trimTokens, dropEmptyTokens);
        auto check_result = check_against(check, split);
        log::debug("delimiter: '{}' trimTokens: {:<5} dropEmptyTokens: {:<5} check: {:<65} -> {:<65}: {}",
                   delimiter,
                   trimTokens,
                   dropEmptyTokens,
                   fmt::format("{}", check),
                   fmt::format("{}", *split),
                   check_result ? "OK" : "FAILED");
        result = result & check_result;
    };

    check(' ', false, true, {"My", "String", "is", "quite", "long,", "and", "good", "enough"});
    check(' ', false, false, {"My", "String", "", "is", "quite", "long,", "and", "good", "enough"});
    check(',', false, true, {"My String  is quite long", " and good enough"});
    check(',', true, true, {"My String  is quite long", "and good enough"});

    return result;
};

NSTEST_ADD(test_strutils_splitString)