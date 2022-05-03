#include "../../util/testing/testing.h"
#include "ns_exception.h"

#include <regex>

using namespace std;
using namespace nanoservices;

function<bool()> test_ns_exception_ctor = []() {
    auto e1 = make_shared<ns_exception>("My exception", NS_POSITION_SHAREDPTR);
    auto e2 = make_shared<ns_exception>(e1, make_shared<string>("My exception 2"), NS_POSITION_SHAREDPTR);

    regex re(".*My exception.*");
    return regex_match(*e1->whatstr(), re);
};

NSTEST_ADD(test_ns_exception_ctor)