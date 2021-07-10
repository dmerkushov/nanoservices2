//
// Created by dmerkushov on 3/20/21.
//

#include "logging.h"

#include "../configuration/configuration.h"

#include <mutex>

using namespace std;
using namespace nanoservices;

const string nanoservices::LOGGING_LOGGER_NAME_DEFAULT = "defaultLogger";

const string nanoservices::LOGGING_LOGGER_LEVELNAME_DEFAULT = "INFO";

map<string, shared_ptr<Logger>, less<>> Logger::_name2LoggerMap;

mutex Logger::_name2LoggerMapMutex;

Logger::Logger(const std::string &name) noexcept {
    // Set name
    this->_name = std::make_shared<std::string>(name);

    // Set level
    std::string configLevelName = std::string("nanoservices.logging.") + name + ".level";
    std::shared_ptr<std::string> levelName = Configuration::getProperty(configLevelName);
    if(!levelName) {
        levelName = Configuration::getProperty("nanoservices.logging.level");
        if(!levelName) {
            levelName = std::make_shared<std::string>(LOGGING_LOGGER_LEVELNAME_DEFAULT);
        }
    }
    setLevel(levelName->c_str());
}

shared_ptr<Logger> Logger::getLogger(const std::string &name) noexcept {
    std::scoped_lock<std::mutex> lock(Logger::_name2LoggerMapMutex);

    shared_ptr<Logger> loggerSharedPtr = Logger::_name2LoggerMap[name];
    if(!loggerSharedPtr) {
        // Sonar will argue about not using make_shared, but we need to call a private constructor here
        loggerSharedPtr = shared_ptr<Logger>(new Logger(name));
        Logger::_name2LoggerMap[name] = loggerSharedPtr;
    }

    return loggerSharedPtr;
}

void Logger::setLevel(const LogLevel level) noexcept {
    this->_level = level;
}

void Logger::setLevel(const char *levelName) noexcept {
    setLevel(Logger::levelByName(levelName));
}

bool Logger::isLoggable(const LogLevel logLevel) const noexcept {
    return logLevel >= this->level();
}

Logger::LogLevel Logger::level() const noexcept {
    return this->_level;
}

Logger::LogLevel Logger::levelByName(const char *name) noexcept {
    string nameStr(name);
    auto trimmedAndUppered = str_toUpper_copy(*str_trim_copy(nameStr));

    if(*trimmedAndUppered == "OFF") {
        return LogLevel::OFF;
    }
    if(*trimmedAndUppered == "FATAL") {
        return LogLevel::FATAL;
    }
    if(*trimmedAndUppered == "ERROR") {
        return LogLevel::ERROR;
    }
    if(*trimmedAndUppered == "WARN") {
        return LogLevel::WARN;
    }
    if(*trimmedAndUppered == "INFO") {
        return LogLevel::INFO;
    }
    if(*trimmedAndUppered == "DEBUG") {
        return LogLevel::DEBUG;
    }
    if(*trimmedAndUppered == "TRACE") {
        return LogLevel::TRACE;
    }
    if(*trimmedAndUppered == "ALL") {
        return LogLevel::ALL;
    }

    return levelByName(LOGGING_LOGGER_LEVELNAME_DEFAULT.c_str());
}

const char *Logger::levelName(const LogLevel level) noexcept {
    if(level >= LogLevel::OFF) {
        return "OFF";
    }
    if(level >= LogLevel::FATAL) {
        return "FATAL";
    }
    if(level >= LogLevel::ERROR) {
        return "ERROR";
    }
    if(level >= LogLevel::WARN) {
        return "WARN";
    }
    if(level >= LogLevel::INFO) {
        return "INFO";
    }
    if(level >= LogLevel::DEBUG) {
        return "DEBUG";
    }
    if(level >= LogLevel::TRACE) {
        return "TRACE";
    }
    if(level >= LogLevel::ALL) {
        return "ALL";
    }

    return LOGGING_LOGGER_LEVELNAME_DEFAULT.c_str();
}