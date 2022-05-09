#include "../../core/exception/ns_exception.h"
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
    auto compareVectors = [](vector<string> check, shared_ptr<vector<shared_ptr<string>>> in) {
        if(check.size() != in->size()) {
            return false;
        }
        for(auto i = 0; i < check.size(); i++) {
            if(check[i].compare(*(in->operator[](i))) != 0) {
                return false;
            }
        }
        return true;
    };

    auto str = make_shared<string>("My String  is quite long, and good enough");

    auto doCheck = [&str, &compareVectors](char delimiter, bool trimTokens, bool dropEmptyTokens, vector<string> check) {
        auto split = splitString(str, delimiter, trimTokens, dropEmptyTokens);
        auto checkResult = compareVectors(check, split);
        log::debug("delimiter: '{}' trimTokens: {:<5} dropEmptyTokens: {:<5} doCheck: {:<65} -> {:<65}: {}",
                   delimiter,
                   trimTokens,
                   dropEmptyTokens,
                   fmt::format("{}", check),
                   fmt::format("{}", *split),
                   checkResult ? "OK" : "FAILED");
        return checkResult;
    };

    log::debug("Input string: \"{}\"", str);

    bool result = true;

    result &= doCheck(' ', false, true, {"My", "String", "is", "quite", "long,", "and", "good", "enough"});
    result &= doCheck(' ', false, false, {"My", "String", "", "is", "quite", "long,", "and", "good", "enough"});
    result &= doCheck(',', false, true, {"My String  is quite long", " and good enough"});
    result &= doCheck(',', true, true, {"My String  is quite long", "and good enough"});

    return result;
};

NSTEST_ADD(test_strutils_splitString)