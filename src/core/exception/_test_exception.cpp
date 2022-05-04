#include "../../plugins/logging/logging.h"
#include "../../util/testing/testing.h"
#include "ns_exception.h"

#include <regex>

using namespace std;
using namespace nanoservices;

static const function<bool()> test_ns_exception = []() {
    auto e1 = make_shared<ns_exception>("My exception 1", NS_POSITION_SHAREDPTR);
    auto e2 = make_shared<ns_exception>(e1, make_shared<string>("My exception 2"), NS_POSITION_SHAREDPTR);

    regex re1(".*My exception 1", std::regex::extended);
    regex re2(".*My exception 2\n.*");

    bool result = true;

    if(!regex_match(*(e1->whatstr()), re1)) {
        log::warn("e1->whatstr() : \"{}\" doesn't match regular expression re1", e1->whatstr());
        result = false;
    }
    if(regex_match(*(e1->whatstr()), re2)) {
        log::warn("e1->whatstr() : \"{}\" matches regular expression re1", e1->whatstr());
        result = false;
    }
    if(!regex_match(*(e2->whatstr()), re1)) {
        log::warn("e2->whatstr() : \"{}\" doesn't match regular expression re1", e2->whatstr());
        result = false;
    }
    if(!regex_match(*(e2->whatstr()), re2)) {
        log::warn("e2->whatstr() : \"{}\" doesn't match regular expression re2", e2->whatstr());
        result = false;
    }
    return result;
};

NSTEST_ADD(test_ns_exception)