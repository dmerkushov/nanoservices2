#include "../../plugins/logging/logging.h"
#include "../../thirdparty/doctest/doctest/doctest.h"
#include "ns_exception.h"

using namespace std;
using namespace nanoservices;

TEST_CASE("ns_exception ctor") {
    ns_exception e("My exception", NS_POSITION_SHAREDPTR);
    log::critical("{}", e);
}