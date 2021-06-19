//
// Created by dmerkushov on 3/20/21.
//

#include "logging.h"
#include "../../util/stringutils/stringutils.h"

using namespace std;
using namespace nanoservices;

const LogLevel nanoservices::_defaultLevel = LogLevel::INFO;

const char *nanoservices::log_getLevelName(LogLevel level) noexcept {
    if (level >= LogLevel::OFF) {
        return "OFF";
    }
    if (level >= LogLevel::FATAL) {
        return "FATAL";
    }
    if (level >= LogLevel::ERROR) {
        return "ERROR";
    }
    if (level >= LogLevel::WARN) {
        return "WARN";
    }
    if (level >= LogLevel::INFO) {
        return "INFO";
    }
    if (level >= LogLevel::DEBUG) {
        return "DEBUG";
    }
    if (level >= LogLevel::TRACE) {
        return "TRACE";
    }
    if (level >= LogLevel::ALL) {
        return "ALL";
    }
    return "UNKNOWN";
}

LogLevel nanoservices::log_getLevelByName(std::string &name) noexcept {
    auto trimmedAndUppered = str_toUpper_copy(str_trim_copy(make_shared<string>(name)));

    if (*trimmedAndUppered == "OFF") {
        return LogLevel::OFF;
    }
    if (*trimmedAndUppered == "FATAL") {
        return LogLevel::FATAL;
    }
    if (*trimmedAndUppered == "ERROR") {
        return LogLevel::ERROR;
    }
    if (*trimmedAndUppered == "WARN") {
        return LogLevel::WARN;
    }
    if (*trimmedAndUppered == "INFO") {
        return LogLevel::INFO;
    }
    if (*trimmedAndUppered == "DEBUG") {
        return LogLevel::DEBUG;
    }
    if (*trimmedAndUppered == "TRACE") {
        return LogLevel::TRACE;
    }
    if (*trimmedAndUppered == "ALL") {
        return LogLevel::ALL;
    }

    return _defaultLevel;
}

void nanoservices::log(stringstream &message, LogLevel level) noexcept {
    if (!log_active(level)) {
        return;
    }

    string descriptionStr = message.str();
    log(descriptionStr, level);
}

void nanoservices::log(function<string()> const &logMessageProducer, LogLevel level) noexcept {
    if (!log_active(level)) {
        return;
    }

    string msg = logMessageProducer();
    log(msg, level);
}

void nanoservices::log(function<stringstream()> logMessageProducer, LogLevel level) noexcept {
    if (!log_active(level)) {
        return;
    }

    stringstream msg = logMessageProducer();
    log(msg, level);
}

//void nanoservices::log(string &message, NsException &exception, LogLevel level) noexcept {
//	stringstream ss;
//	ss << message << endl;
//	ss << "Exception:" << endl;
//	ss << exception.fullDescription();
//
//	log(ss, level);
//}
//
//void nanoservices::log(stringstream &message, NsException &exception, LogLevel level) noexcept {
//	string s = message.str();
//	log(s, exception, level);
//}