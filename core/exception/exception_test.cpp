#include "../../plugins/logging/logging.h"
#include "../../thirdparty/doctest/doctest/doctest.h"
#include "ns_exception.h"

#include <regex>

using namespace std;
using namespace nanoservices;

TEST_CASE("ns_exception ctor") {
    log::info("TEST CASE: ns_exception ctor");
    auto e1 = make_shared<ns_exception>("My exception", NS_POSITION_SHAREDPTR);
    auto e2 = make_shared<ns_exception>(e1, make_shared<string>("My exception 2"), NS_POSITION_SHAREDPTR);

    regex re("My exception");
    CHECK(regex_match(*e1->whatstr(), re));
}