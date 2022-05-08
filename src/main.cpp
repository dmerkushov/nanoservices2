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

    // if(configuration::get_property("nanoservices.testing.nstests.run")) {
    //     doctest::Context context;
    //     context.setOption("reporters", "console");
    //     context.applyCommandLine(argc, argv);
    //     int doctest_result = context.run();
    //     if(context.shouldExit()) {
    //         return doctest_result;
    //     }
    // }

    ns_exception e2(make_shared<ns_exception>("Message1", NS_POSITION_SHAREDPTR), make_shared<string>("Message2"), NS_POSITION_SHAREDPTR);

    log::set_level(log::level::trace);

    log::critical("{}", e2);
    log::warn("{}", e2);
    log::error("{}", e2);
    log::info("{}", e2);
    log::debug("{}", e2);

    ns_exception e3("Message3", NS_POSITION_SHAREDPTR);

    function<string()> f = [e3]() {
        return string("EXCEPTION FROM FUNC: ") + e3.what();
    };

    log::trace("{}", f);

    // spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%F] [thread %t] %^[%l]%$ %v");
    //
    // spdlog::info("Welcome to spdlog!");
    //
    // std::array<char, 80> buf;
    // spdlog::info("Binary example: {:a}", spdlog::to_hex(buf));
    // spdlog::info("Another binary example:{:n}", spdlog::to_hex(std::begin(buf), std::begin(buf) + 10));
    //
    // spdlog::error("Some error message with arg: {}", 1);
    //
    // spdlog::warn("Easy padding in numbers like {:08d}", 12);
    // spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    // spdlog::info("Support for floats {:03.2f}", 1.23456);
    // spdlog::info("Positional args are {1} {0}..", "too", "supported");
    // spdlog::info("| {:>30} |", "right aligned");
    //
    // spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    // spdlog::debug("This message should be displayed..");
    //
    // shared_ptr<Logger> logger = Logger::getLogger();
    // logger->info("+main()");
    //
    // MyEnclosingClass myClass1;
    // MyEnclosedClass myEnclosedClass;
    // myClass1.enclosingField[0] = myEnclosedClass;
    // myClass1.enclosingField[0].int32Field = 234;
    // myClass1.enclosingField[0].mapField[32] = "Hallo";
    // myClass1.enclosingField[0].vectorField = {0.0, 3.14};
    // auto serialized = myClass1.__nanoservices2_serializer_serialize();
    // MyEnclosingClass myClass2;
    // myClass2.__nanoservices2_serializer_deserialize(serialized);
    //
    // spdlog::info("Deserialized map: {}", myClass2.enclosingField[0].mapField[32]);
    // spdlog::info("Deserialized vector: {}, {}", myClass2.enclosingField[0].vectorField[0], myClass2.enclosingField[0].vectorField[1]);
    {
        // stringstream msgSS;
        // msgSS << "Deserialized vector: " << myClass2.enclosingField[0].vectorField.at(0) << ", " << myClass2.enclosingField[0].vectorField.at(1);
        // logger->info(msgSS);
    }
    //
    // logger->info("-main()");
    //
    logging_finalize();
    configuration::finalize();

    return 0;
}