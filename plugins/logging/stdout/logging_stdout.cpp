//
// Created by dmerkushov on 3/20/21.
//

#include "../../configuration/configuration.h"
#include "../logging.h"

#include <ctime>
#include <iostream>

using namespace std;
using namespace nanoservices;

static LogLevel _currentLevel = LogLevel::INFO;

const int STRFTIME_BUFFER_SIZE = 24;

void nanoservices::log_initialize() noexcept {
    shared_ptr<string> desiredLogLevel = conf_getProperty("logLevel");
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

void nanoservices::log_setLevel(LogLevel level) noexcept {
    _currentLevel = level;
}

bool nanoservices::log_active(LogLevel level) noexcept {
    return level >= _currentLevel;
}

void nanoservices::log(std::string &message, LogLevel level) noexcept {
    if(!log_active(level)) {
        return;
    }

    time_t rawtime;
    struct tm *timeinfo;
    char curtimebuf[STRFTIME_BUFFER_SIZE];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(curtimebuf, STRFTIME_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", timeinfo);

    // TODO strftime()

    cout << curtimebuf << ' ' << log_getLevelName(level) << ": " << message << endl;
    cout.flush();
}