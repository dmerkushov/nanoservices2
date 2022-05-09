#define DOCTEST_CONFIG_IMPLEMENT

#include "core/exception/ns_exception.h"
#include "core/serialization/serialization.h"
#include "plugins/configuration/configuration.h"
#include "plugins/logging/logging.h"
//#include "thirdparty/doctest/doctest/doctest.h"
#include "thirdparty/gabime/spdlog/fmt/bin_to_hex.h"
#include "thirdparty/gabime/spdlog/spdlog.h"

#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace nanoservices;

namespace logger = spdlog;

struct MyInnerClass {
    int32_t int32Field;
    std::map<int32_t, std::string> mapField;
    std::vector<double> vectorField;

    NANOSERVICES2_MAKE_SERIALIZABLE(int32Field, mapField, vectorField);
};

struct MyOuterClass {
    std::map<u_int8_t, MyInnerClass> enclosingField;

    NANOSERVICES2_MAKE_SERIALIZABLE(enclosingField);
};

int main(int argc, char **argv) {
    configuration::initialize(argc, argv);
    logging_initialize();

    // SERVICE CODE HERE

    logging_finalize();
    configuration::finalize();

    return 0;
}
