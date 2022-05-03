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