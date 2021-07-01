#include "plugins/configuration/configuration.h"
#include "plugins/logging/logging.h"

#include <functional>
#include <iostream>
#include <string>

using namespace std;
using namespace nanoservices;

int main(int argc, char **argv) noexcept {
    Configuration::initialize(argc, argv);
    log_initialize();

    shared_ptr<string> name = Configuration::getProperty("name");

    function<string()> logLambda = [name]() {
        string str("Logging via lambda: Hello, World! And especially ");
        str += *name;
        return str;
    };

    double avgWorkingTime = 0.0;
    int64_t currentWorkingTime;

    for(int i = 0; i < 30; i++) {
        chrono::time_point start = chrono::system_clock::now();
        log(logLambda, LogLevel::DEBUG);
        chrono::time_point end = chrono::system_clock::now();

        currentWorkingTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        avgWorkingTime = ((avgWorkingTime * i) + currentWorkingTime) / (i + 1);

        cout << "Logging worked for " << currentWorkingTime << " ns" << endl;
        cout.flush();
    }

    cout << "Average working time " << avgWorkingTime << " ns" << endl << endl << endl << endl;

    avgWorkingTime = 0.0;

    stringstream logSS;
    logSS << "Logging via stringstream: Hello, World! And especially " << *name;

    for(int i = 0; i < 30; i++) {
        chrono::time_point start = chrono::system_clock::now();
        log(logSS, LogLevel::DEBUG);
        chrono::time_point end = chrono::system_clock::now();

        currentWorkingTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        avgWorkingTime = ((avgWorkingTime * i) + currentWorkingTime) / (i + 1);

        cout << "Logging worked for " << currentWorkingTime << " ns" << endl;
        cout.flush();
    }

    cout << "Average working time " << avgWorkingTime << " ns" << endl << endl << endl << endl;

    log_finalize();
    Configuration::finalize();

    return 0;
}
