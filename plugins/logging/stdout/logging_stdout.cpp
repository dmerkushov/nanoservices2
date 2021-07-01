//
// Created by dmerkushov on 3/20/21.
//

#include "../../configuration/configuration.h"
#include "../logging.h"

#include <chrono>
#include <ctime>
#include <iostream>

using namespace std;
using namespace nanoservices;

static LogLevel _currentLevel = LogLevel::INFO;

const int STRFTIME_BUFFER_SIZE = 32;

void nanoservices::log_initialize() noexcept {
    shared_ptr<string> desiredLogLevel = Configuration::getProperty("level");
    if(desiredLogLevel->empty()) {
        return;
    }

    LogLevel logLevel = log_getLevelByName(desiredLogLevel->c_str());
    log_setLevel(logLevel);
}

void nanoservices::log_finalize() noexcept {
    // Do nothing
}

LogLevel nanoservices::log_getLevel() noexcept {
    return _currentLevel;
}

void nanoservices::log_setLevel(const LogLevel level) noexcept {
    _currentLevel = level;
}

bool nanoservices::log_active(const LogLevel level) noexcept {
    return level >= _currentLevel;
}

void nanoservices::log(const string &message, LogLevel level) noexcept {
    if(!log_active(level)) {
        return;
    }

    chrono::time_point tp = chrono::system_clock::now();

    // Put the time information into a struct
    time_t rawtime = chrono::system_clock::to_time_t(tp);
    struct tm timeinfo;
    localtime_r(&rawtime, &timeinfo);

    // Convert the time struct to a string. The time struct contains only seconds-precision information, so we will need
    // a millisecond conversion later
    char curtimebuf[STRFTIME_BUFFER_SIZE]; // Sonar will argue about using a C-style array. We need it to be used in
                                           // strftime() in the next line
    strftime(curtimebuf, STRFTIME_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", &timeinfo);

    // Remove the seconds (and bigger) parts, then only milliseconds of each second will remain in the time point
    chrono::seconds seconds = chrono::duration_cast<chrono::seconds>(tp.time_since_epoch());
    tp -= seconds;

    chrono::microseconds micros = chrono::duration_cast<chrono::microseconds>(tp.time_since_epoch());

    // Sonar will argue on using cout for logging. But this logging implementation is a cout one!
    cout << curtimebuf << '.' << micros.count() << ' ' << log_getLevelName(level) << ": " << message << endl;
    cout.flush();
}