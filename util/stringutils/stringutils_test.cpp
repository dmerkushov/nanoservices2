#include "stringutils.h"

#include "../../plugins/logging/logging.h"
#include "../../thirdparty/doctest/doctest/doctest.h"

using namespace std;
using namespace nanoservices;

TEST_CASE("test fmt formatter for shared_ptr<string>") {
    shared_ptr<string> str = make_shared<string>("Ololo");
    string str2 = fmt::format("{}", str);
    CHECK(str->compare(str2) == 0);
}