#include "core/exception/NsException.h"
#include "plugins/configuration/configuration.h"
#include "plugins/logging/logging.h"

#include <functional>
#include <iostream>
#include <string>

using namespace std;
using namespace nanoservices;

int main(int argc, char **argv) {
    Configuration::initialize(argc, argv);
    Logger::initialize();
    shared_ptr<Logger> logger = Logger::getLogger();

    logger->info("+main()");

    shared_ptr<string> name = Configuration::getProperty("name");

    auto logLambda = [name]() {
        string str("Logging via lambda: Hello, World! And especially ");
        str += *name;
        return str;
    };

    double avgWorkingTime = 0.0;
    int64_t currentWorkingTime;
    int64_t fullWorkingTime = 0;
    int64_t maxWorkingTime = 0;
    int maxWTIter = 0;
    int minWTIter = 0;
    int64_t minWorkingTime = INT64_MAX;

    int iterations = 100000;

    for(int i = 0; i < iterations + 1; i++) {
        chrono::time_point start = chrono::system_clock::now();
        logger->log( // logLambda,
                [name, i]() {
                    stringstream ss;
                    ss << "hello World and espicially " << (name != nullptr ? *name : "you") << " " << i;
                    return ss;
                },
                Logger::LogLevel::FATAL);
        chrono::time_point end = chrono::system_clock::now();

        currentWorkingTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        // Skip the first long-lasting call
        if(i > 0) {
            fullWorkingTime += currentWorkingTime;
            if(currentWorkingTime > maxWorkingTime) {
                maxWorkingTime = currentWorkingTime;
                maxWTIter = i;
            }
            if(currentWorkingTime < minWorkingTime) {
                minWorkingTime = currentWorkingTime;
                minWTIter = i;
            }
        }

        cout << "Logging worked for " << currentWorkingTime << " ns" << endl;
        cout.flush();
    }

    avgWorkingTime = fullWorkingTime * 1.0 / iterations;

    cout << "Average working time " << avgWorkingTime << " ns" << endl;
    cout << "Max " << maxWorkingTime << " on iteration " << maxWTIter << endl;
    cout << "Min " << minWorkingTime << " on iteration " << minWTIter << endl;

    // fullWorkingTime = 0;
    //
    // stringstream logSS;
    // logSS << "Logging via stringstream: Hello, World! And especially " << *name;
    //
    // for(int i = 0; i < iterations + 1; i++) {
    //     chrono::time_point start = chrono::system_clock::now();
    //     logger->debug(logSS);
    //     chrono::time_point end = chrono::system_clock::now();
    //
    //     currentWorkingTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    //
    //     // Skipping the first long-lasting call
    //     if(i > 0) {
    //         fullWorkingTime += currentWorkingTime;
    //     }
    //
    //     cout << "Logging worked for " << currentWorkingTime << " ns" << endl;
    //     cout.flush();
    // }
    //
    // avgWorkingTime = fullWorkingTime * 1.0 / iterations;
    //
    // cout << "Average working time " << avgWorkingTime << " ns" << endl << endl << endl << endl;

    Logger::finalize();
    Configuration::finalize();

    return 0;
}
