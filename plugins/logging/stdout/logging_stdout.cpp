//
// Created by dmerkushov on 3/20/21.
//

#include "../../../thirdparty/HowardHinnant/date/include/date/date.h"
#include "../logging.h"

#include <ctime>
#include <iostream>

using namespace std;
using namespace date;
using namespace nanoservices;

void Logger::initialize() noexcept {
    // Do nothing
}

void Logger::finalize() noexcept {
    // Do nothing
}

void Logger::doLog(const LogRecord &logRecord) const noexcept {
    // Sonar will argue on using cout for logging. But this logging implementation is a cout one!
    cout << logRecord.timePoint << ' ';
    cout << *logRecord.loggerName << ' ' << Logger::levelName(logRecord.level) << ": " << *logRecord.message << endl;
}