#include "plugins/configuration/configuration.h"
#include "plugins/logging/logging.h"

#include <functional>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace nanoservices;

int main(int argc, char **argv) noexcept {
    conf_initialize(argc, argv);
    log_initialize();

    shared_ptr<string> name = conf_getProperty("name");

    function<string()> logLambda = [name]() {
        string str("Logging via lambda: Hello, World! And especially ");
        str += *name;
        return str;
    };
    log(logLambda, LogLevel::DEBUG);

    stringstream logSS;
    logSS << "Logging via stringstream: Hello, World! And especitally " << *name;
    log(logSS, LogLevel::DEBUG);

    return 0;
}
