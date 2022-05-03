#include "../../util/testing/testing.h"
#include "ns_exception.h"

#include <regex>

using namespace std;
using namespace nanoservices;

static function<bool()> test_ns_exception_ctor = []() {
    auto e1 = make_shared<ns_exception>("My exception 1", NS_POSITION_SHAREDPTR);
    auto e2 = make_shared<ns_exception>(e1, make_shared<string>("My exception 2"), NS_POSITION_SHAREDPTR);

    regex re1(".*My exception 1.*");
    regex re2(".*My exception 2.*");
    return
            // Testing whatstr for the first exception
            regex_match(*e1->whatstr(), re1) &&
            !regex_match(*e1->whatstr(), re2)

            // Testing whatstr for the second exception (with a cause)
            && regex_match(*e2->whatstr(), re1) && regex_match(*e2->whatstr(), re2);
};

NSTEST_ADD(test_ns_exception_ctor)