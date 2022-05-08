#include "../../plugins/logging/logging.h"
#include "../../util/testing/testing.h"
#include "ns_exception.h"

#include <regex>

using namespace std;
using namespace nanoservices;

static const function<bool()> test_ns_exception_whatstr = []() {
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

NSTEST_ADD(test_ns_exception_whatstr)

static const function<bool()> test_ns_exception_message = []() {
    const char *msg = "My exception";

    auto e = make_shared<ns_exception>(msg, NS_POSITION_SHAREDPTR);

    bool result = true;
    if(e->message()->compare(msg) != 0) {
        log::warn("e->message(): \"{}\" is not the expected \"{}\"", e->message(), msg);
        result = false;
    }
    return result;
};

NSTEST_ADD(test_ns_exception_message)

static const function<bool()> test_ns_exception_position = []() {
    auto e = make_shared<ns_exception>("My exception", NS_POSITION_SHAREDPTR);
    bool result = true;

    auto position = e->position();
    regex re("[a-zA-Z0-9\\/_.]*:[0-9]*, in <lambda\\(\\)>");
    if(!regex_match(*position, re)) {
        log::warn("position: {} does not match regular expression");
        result = false;
    }
    return result;
};

NSTEST_ADD(test_ns_exception_position)

static const function<bool()> test_ns_exception_cause = []() {
    ns_exception e2 = []() {
        ns_exception e1("My exception", NS_POSITION_SHAREDPTR);
        ns_exception e2(e1, make_shared<string>("Another exception"), NS_POSITION_SHAREDPTR);
        return e2;
    }();

    auto e1ptr = e2.cause(); // If no segfault here, then everything is ok.

    return true;
};

NSTEST_ADD(test_ns_exception_cause)