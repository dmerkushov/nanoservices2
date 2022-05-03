#include "stringutils.h"

#include "../../plugins/logging/logging.h"
#include "../testing/testing.h"

using namespace std;
using namespace nanoservices;

static std::function<bool()> test_strutils_fmt_sharedptrstring_formatter = []() {
    shared_ptr<string> str = make_shared<string>("Ololo");
    string str2 = fmt::format("{}", str);
    return (str->compare(str2) == 0);
};

NSTEST_ADD(test_strutils_fmt_sharedptrstring_formatter)