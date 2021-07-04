//
// Created by dmerkushov on 3/20/21.
//

#include "../../configuration/configuration.h"
#include "../logging.h"

#include <chrono>
#include <ctime>
#include <iostream>

using namespace std;
using namespace chrono;
using namespace nanoservices;

const int STRFTIME_BUFFER_SIZE = 32;

void Logger::initialize() noexcept {
    // Do nothing
}

void Logger::finalize() noexcept {
    // Do nothing
}

void Logger::doLog(const LogRecord &logRecord) const noexcept {
    time_point mutableTP = logRecord.timePoint;

    // Put the time information into a struct
    time_t rawtime = system_clock::to_time_t(mutableTP);
    struct tm timeinfo;
    localtime_r(&rawtime, &timeinfo);

    // Convert the time struct to a string. The time struct contains only seconds-precision information, so we will need
    // a millisecond conversion later
    char curtimebuf[STRFTIME_BUFFER_SIZE]; // Sonar will argue about using a C-style array. We need it to be used in
                                           // strftime() in the next line
    strftime(curtimebuf, STRFTIME_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", &timeinfo);

    // Remove the seconds (and bigger) parts, then only milliseconds of each second will remain in the time point
    seconds sec = duration_cast<seconds>(mutableTP.time_since_epoch());
    mutableTP -= sec;

    microseconds micros = duration_cast<microseconds>(mutableTP.time_since_epoch());

    // Sonar will argue on using cout for logging. But this logging implementation is a cout one!
    cout << curtimebuf << '.' << micros.count() << ' ' << *logRecord.loggerName << ' '
         << Logger::levelName(logRecord.level) << ": " << *logRecord.message << endl;
    cout.flush();
}