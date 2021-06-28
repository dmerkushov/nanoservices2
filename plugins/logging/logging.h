//
// Created by dmerkushov on 3/20/21.
//

#ifndef LOGGING_H_
#define LOGGING_H_

#include "../../util/stringutils/stringutils.h"

#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <string>
//#include "../../core/exception/NsException.h"

namespace nanoservices {

struct LogRecord;

class Logger {
public:
    /**
     * Logging levels enum. Levels are:
     * <ul>
     * <li>ALL, OFF - the lowest and the highest levels, respectively. NOT ADVISED to be used when calling log
     * functions. Only advised to be used when setting the desired logging volubility via configuration. So when setting
     * to ALL, all the logging messages will appear in the log, and when setting to OFF, no messages will appear in the
     * log. <li>TRACE, DEBUG, INFO, WARN, ERROR, FATAL - levels advised to be used both in configuration and in the code
     * </ul>
     */
    enum class LogLevel : uint32_t {
        ALL = 0,
        TRACE = 100,
        DEBUG = 200,
        INFO = 300,
        WARN = 400,
        ERROR = 500,
        FATAL = 600,
        OFF = UINT32_MAX
    };

private:
    /**
     * @brief The current log level of the logger
     */
    LogLevel _level;

    /**
     * @brief Name of the logger
     */
    std::shared_ptr<std::string> _name;

    /**
     * @brief Write the supplied log record to the log.
     * @details One of the three methods to be implemented by the logging plugin. Others are: Logger::initialize() and
     * Logger::finalize().
     *
     * No log level check is needed, it is done beforehand in user-callable methods.
     * @param logRecord
     */
    void writeToLog(LogRecord &logRecord) noexcept;

public:
    /**
     * @brief The default log level
     */
    static const LogLevel defaultLevel;

    /**
     * @brief Initialize the logging engine.
     * @details One of the three methods to be implemented by the logging plugin. Others are:
     * Logger::writeToLog(LogRecord&) and Logger::finalize()
     */
    static void initialize() noexcept;

    /**
     * @brief Finalize the logging engine.
     * @details One of the three methods to be implemented by the logging plugin. Others are:
     * Logger::writeToLog(LogRecord&) and Logger::initialize()
     */
    static void finalize() noexcept;

    /**
     * @brief Create a logger with the default name and the configured log level, or, if there is no configured log
     * level, the default log level.
     * @details The name for the logger will be "DEFAULT".
     *
     * In configuration, the property named "nanoservices.logging.{name}.logLevel" provides the name of the
     * log level for the logger ("{name}" is substituted by the logger name). If no such property is found, the
     * property named "nanoservices.logging.logLevel" is used. If no such property is found, too, the log level is
     * set to the default value, Logger::defaultLevel.
     */
    Logger() noexcept;

    /**
     * @brief Create a logger with the given name and the configured log level, or, if there is no configured log level,
     * the default log level.
     * @details In configuration, the property named "nanoservices.logging.{name}.logLevel" provides the name of the
     * log level for the logger ("{name}" is substituted by the logger name). If no such property is found, the
     * property named "nanoservices.logging.logLevel" is used. If no such property is found, too, the log level is
     * set to the default value, Logger::defaultLevel.
     * @param name
     */
    explicit Logger(std::string &name) noexcept;

    /**
     * @brief Create a logger with the default name and the given log level
     * @details The name for the logger will be "DEFAULT"
     * @param logLevel
     */
    explicit Logger(LogLevel logLevel) noexcept;

    /**
     * @brief Create a logger with the given name and log level
     * @param name
     * @param logLevel
     */
    Logger(std::string &name, LogLevel logLevel) noexcept;

    /**
     * @brief Set the given log level for the logger
     * @param logLevel
     */
    void setLevel(LogLevel logLevel) noexcept;

    /**
     * @brief Get the current log level for the logger
     * @return
     */
    LogLevel level() noexcept;

    /**
     * @brief Is the given level currently loggable by the logger?
     * @param logLevel
     * @return
     */
    bool isLoggable(LogLevel logLevel) noexcept;
};

struct LogRecord {

    /**
     * @brief Time point of the event
     */
    const std::chrono::time_point<std::chrono::system_clock> timePoint;

    /**
     * @brief Log level for the event
     */
    const Logger::LogLevel logLevel;

    /**
     * @brief Logger name that has logged the event. By default, an empty string
     */
    std::shared_ptr<std::string> loggerName;

    /**
     * @brief The event message.
     */
    std::shared_ptr<std::string> message;
};

/**
 * Get the name of the log level
 * @param level
 * @return
 */
const char *log_getLevelName(const LogLevel level) noexcept;

/**
 * Get the log level by its name. If the name is empty or unknown, returns the default log level, which is INFO
 * @param name
 * @return
 */
LogLevel log_getLevelByName(const char *) noexcept;

/**
 * @brief Initialize the logging engine. Called after the configuration is loaded, so logging may be configured via the
 * configuration engine.
 */
void log_initialize() noexcept;

/**
 * @brief Finalize the logging engine.
 */
void log_finalize() noexcept;

/**
 * @brief Get the current logging level
 * @return
 */
LogLevel log_getLevel() noexcept;

/**
 * @brief Set the logging level
 * @param level INFO by default
 */
void log_setLevel(const LogLevel level = _defaultLevel) noexcept;

/**
 * @brief Is the logging active for the given level? By default, no, so the user may skip building the logging data.
 * @param level Logging level to test. INFO by default.
 * @return If logging for the given level is active
 * @see LogLevel
 */
bool log_active(const LogLevel level = _defaultLevel) noexcept;

/**
 * @brief Log an event at the given log level
 * @param message Description of the event
 * @param level the level of the event. INFO by default
 */
void log(const std::string &message, const LogLevel level = _defaultLevel) noexcept;

/**
 * @brief Log an event at the given log level
 * @param message Description of the event
 * @param level the level of the event. INFO by default
 */
void log(const std::stringstream &message, const LogLevel level = _defaultLevel) noexcept;

/**
 * @brief Log a message produced by the supplied function (log message producer - LMP), with a log level check before
 * the LMP invocation. Note that the LMP may not throw
 * @param logMessageProducer The function producing the log message. May produce a string or a stringstream. May be a
 * lambda
 * @param level the level of the message. INFO by default
 */
template<class MessageType>
void log(const std::function<MessageType()> &logMessageProducer, const LogLevel level = _defaultLevel) noexcept {
    static_assert(nanoservices::is_string_or_stringstream<MessageType>,
                  "Log message producer function returns neither std::string nor std::stringstream");

    if(!log_active(level)) {
        return;
    }

    MessageType msg = logMessageProducer();
    log(msg, level);
}

} // namespace nanoservices

#endif // LOGGING_H_
