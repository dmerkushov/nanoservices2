#define DOCTEST_CONFIG_IMPLEMENT

#include "core/exception/ns_exception.h"
#include "core/serialization/serialization.h"
#include "plugins/configuration/configuration.h"
#include "plugins/logging/logging.h"
#include "thirdparty/doctest/doctest/doctest.h"
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

struct MyEnclosedClass {
    int32_t int32Field;
    std::map<int32_t, std::string> mapField;
    std::vector<double> vectorField;

    NANOSERVICES2_MAKE_SERIALIZABLE(int32Field, mapField, vectorField);
};

struct MyEnclosingClass {
    std::map<u_int8_t, MyEnclosedClass> enclosingField;

    NANOSERVICES2_MAKE_SERIALIZABLE(enclosingField);
};

int main(int argc, char **argv) {
    configuration::initialize(argc, argv);
    logging_initialize();

    log::info("nanoservices2: unit tests running");

    doctest::Context context;
    context.setOption("reporters", "console");
    // context.setOption("out", "doctestreport.xml");
    context.applyCommandLine(argc, argv);
    int doctest_result = context.run();

    log::info("nanoservices2: unit tests finished. shouldExist: {}, exit code: {}", context.shouldExit(), doctest_result);

    if(context.shouldExit()) {
        return doctest_result;
    }

    return 0;
}
