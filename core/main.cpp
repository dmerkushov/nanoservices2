#include <iostream>
#include <string>

#include "../plugins/configuration/configuration.h"

#include "../plugins/logging/logging.h"

using namespace std;
using namespace nanoservices;

int main(int argc, char **argv) noexcept {

    conf_initialize(argc, argv);
    log_initialize();

    shared_ptr<string> name = conf_getProperty("name");
    auto logLambda = [name]() {
        string str("Hello, World! And especially ");
        str += *name;
        return str;
    };
    log(logLambda, LogLevel::DEBUG);

    return 0;
}
