//#include "core/exception/ns_exception.h"
#include "core/serialization/serialization.h"
#include "plugins/configuration/configuration.h"
#include "plugins/logging/logging.h"
#include "thirdparty/gabime/spdlog/spdlog.h"
#include "thirdparty/swansontec/map-macro/do_foreach.h"

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace nanoservices;

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
    Configuration::initialize(argc, argv);
    Logger::initialize();

    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);

    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("| {:>30} |", "right aligned");

    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    spdlog::debug("This message should be displayed..");

    // change log pattern
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

    // Compile time log levels
    // define SPDLOG_ACTIVE_LEVEL to desired level
    SPDLOG_TRACE("Some trace message with param {}", 42);
    SPDLOG_DEBUG("Some debug message");

    shared_ptr<Logger> logger = Logger::getLogger();

    logger->info("+main()");

    MyEnclosingClass myClass1;
    MyEnclosedClass myEnclosedClass;
    myClass1.enclosingField[0] = myEnclosedClass;
    myClass1.enclosingField[0].int32Field = 234;
    myClass1.enclosingField[0].mapField[32] = "Hallo";
    myClass1.enclosingField[0].vectorField = {0.0, 3.14};

    auto serialized = myClass1.__nanoservices2_serializer_serialize();

    MyEnclosingClass myClass2;
    myClass2.__nanoservices2_serializer_deserialize(serialized);

    {
        stringstream msgSS;
        msgSS << "Deserialized int32: " << myClass2.enclosingField[0].int32Field;
        logger->info(msgSS);
    }
    {
        stringstream msgSS;
        msgSS << "Deserialized map: " << myClass2.enclosingField[0].mapField[32];
        logger->info(msgSS);
    }
    {
        stringstream msgSS;
        msgSS << "Deserialized vector: " << myClass2.enclosingField[0].vectorField.at(0) << ", " << myClass2.enclosingField[0].vectorField.at(1);
        logger->info(msgSS);
    }

    logger->info("-main()");

    Logger::finalize();
    Configuration::finalize();

    return 0;
}
