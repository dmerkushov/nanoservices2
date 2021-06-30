//
// Created by dmerkushov on 3/20/21.
//

#ifndef LOGGING_H_
#define LOGGING_H_

#include "../../util/stringutils/stringutils.h"

#include <chrono>
#include <exception>
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <string>

namespace nanoservices {

/**
 * @brief The name for the default logger
 */
extern const std::string DEFAULT_LOGGER_NAME;

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
     * @brief A record in the log.
     */
    struct LogRecord {

        /**
         * @brief Time point of the event
         */
        std::chrono::time_point<std::chrono::system_clock> timePoint;

        /**
         * @brief Log level for the event
         */
        Logger::LogLevel level;

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
     * @brief The current log level of the logger
     */
    LogLevel _level;

    /**
     * @brief Name of the logger
     */
    std::shared_ptr<std::string> _name;

    /**
     * @brief Create a logger with the given name and the configured log level, or, if there is no configured log level,
     * the default log level.
     * @details In configuration, the property named "nanoservices.logging.{name}.level" provides the name of the
     * log level for the logger ("{name}" is substituted by the logger name). If no such property is found, the
     * property named "nanoservices.logging.level" is used. If no such property is found, too, the log level is
     * set to the default value, Logger::defaultLevel.
     * @param name
     */
    explicit Logger(std::string &name) noexcept;

    /**
     * @brief Write the supplied log record to the log.
     * @details One of the three methods to be implemented by the logging plugin. Others are: Logger::initialize() and
     * Logger::finalize().
     * @details No log level check is needed in this method, it is done beforehand in user-callable methods.
     * @details This method must guarantee the following: for any two subsequent entries into this method, the
     * corresponding log records in the resulting log will follow the same order (if applicable).
     * @details This method must return only after the logging operation is over.
     * @details This method must be thread-safe.
     * @param logRecord The log record to be logged
     */
    void doLog(LogRecord &logRecord) noexcept;

public:
    /**
     * @brief The default log level
     */
    static const LogLevel defaultLevel;

    /**
     * @brief Initialize the logging engine.
     * @details One of the three methods to be implemented by the logging plugin. Others are:
     * Logger::doLog(LogRecord&) and Logger::finalize().
     * @details This method is called exactly once at the start of the nanoservice. The method call is not required to
     * be thread-safe.
     */
    static void initialize() noexcept;

    /**
     * @brief Finalize the logging engine.
     * @details One of the three methods to be implemented by the logging plugin. Others are:
     * Logger::doLog(LogRecord&) and Logger::initialize().
     * @details This method is not guaranteed to be called, but if called, it is called exactly once when the
     * nanoservice is finishing. The method call is not required to be thread-safe.
     */
    static void finalize() noexcept;

    /**
     * @brief Get the default logger.
     * @details The returned logger will have the name set by nanoservices::DEFAULT_LOGGER_NAME (case sensitive) and the
     * configured log level for that name.
     * @details In configuration, the property named "nanoservices.logging.{name}.level" provides the name of the
     * log level for the logger ("{name}" is substituted by the logger name). If no such property is found, the
     * property named "nanoservices.logging.level" is used. If no such property is found, too, the log level is
     * set to the default value, Logger::defaultLevel.
     * @details If such a logger had been already constructed, the same instance is pointed to by the returned shared
     * pointer.
     * @details The returned shared pointer is guaranteed to be not empty.
     * @details This method is thread-safe.
     * @return
     */
    static std::shared_ptr<Logger> getDefaultLogger() noexcept;

    /**
     * @brief Get a logger by its name.
     * @details The returned logger will have the supplied name (case sensitive) and the configured log level for
     * that name.
     * @details In configuration, the property named "nanoservices.logging.{name}.level" provides the name of the
     * log level for the logger ("{name}" is substituted by the logger name). If no such property is found, the
     * property named "nanoservices.logging.level" is used. If no such property is found, too, the log level is
     * set to the default value, Logger::defaultLevel.
     * @details If such a logger had been already constructed, the same instance is pointed to by the returned shared
     * pointer.
     * @details The returned shared pointer is guaranteed to be not empty.
     * @details This method is thread-safe.
     * @param name
     * @return
     */
    static std::shared_ptr<Logger> getLoggerByName(std::string &name) noexcept;

    /**
     * @brief Get a log level by its name
     * @details If the name is unexpected, Logger::defaultLevel is returned.
     * @details This method is thread-safe.
     * @param name
     * @return
     */
    static LogLevel levelByName(const char *name) noexcept;

    /**
     * @brief Get the name of the log level
     * @details This method is thread-safe.
     * @param level
     * @return Name of the log level in uppercase
     */
    static const char *levelName(LogLevel level) noexcept;

    /**
     * @brief Set the given log level for the logger
     * @details This method is thread-safe.
     * @param logLevel
     */
    void setLevel(LogLevel logLevel) noexcept;

    /**
     * @brief Get the current log level for the logger
     * @details This method is thread-safe.
     * @return
     */
    LogLevel level() noexcept;

    /**
     * @brief Is the given level currently loggable by the logger?
     * @details This method is thread-safe.
     * @param logLevel
     * @return
     */
    bool isLoggable(LogLevel logLevel) noexcept;

    /**
     * @brief Send a message to the log with the TRACE level
     * @details This method is thread-safe.
     * @details This method is not intended to be implemented by the plugin.
     * @tparam message type must be either std::string or std::stringstream
     * @param message or message producer function
     * @param exception an optional exception parameter. If not empty, the exception message will be added to the log.
     * message, delimited by std::endl.
     */
    template<class MT>
    void trace(MT &message, std::shared_ptr<std::exception> exception = nullptr) noexcept {
        static_assert(nanoservices::is_string_or_stringstream_or_strproducerfunc<MT>,
                      "Log message must be either std::string or std::stringstream or a string producer function");

        log(message, LogLevel::TRACE);
    }

    /**
     * @brief Send a message to the log with the DEBUG level
     * @details This method is thread-safe.
     * @param message
     */
    void debug(std::string &message) noexcept;

    /**
     * @brief Send a message to the log with the INFO level
     * @details This method is thread-safe.
     * @param message
     */
    void info(std::string &message) noexcept;

    /**
     * @brief Send a message to the log with the WARN level
     * @details This method is thread-safe.
     * @param message
     */
    void warn(std::string &message) noexcept;

    /**
     * @brief Send a message to the log with the ERROR level
     * @details This method is thread-safe.
     * @param message
     */
    void error(std::string &message) noexcept;

    /**
     * @brief Send a message to the log with the FATAL level
     * @details This method is thread-safe.
     * @param message
     */
    void fatal(std::string &message) noexcept;

    /**
     * @brief Send a message to the log with the FATAL level, with addition of an exception description via
     * std::exception::what()
     * @details The exception description is acquired by calling std::exception::what()
     * @details This method is thread-safe.
     * @param message
     */
    void fatal(std::string &message, std::exception &exception) noexcept;

    /**
     * @brief Send a message to the log with the given level and with the given timepoint recorded
     * @tparam type of the message. Must be either std::string or std::stringstream
     * @param message
     * @param exception if not nullptr, its message will be added to the log message, delimited by std::endl
     * @param level
     * @param timePoint
     */
    template<class MT>
    void log(const MT &message,
             const std::exception *exception = nullptr,
             const LogLevel level = Logger::defaultLevel,
             std::chrono::time_point<std::chrono::system_clock> timePoint = std::chrono::system_clock::now()) noexcept {
        static_assert(nanoservices::is_string_or_stringstream<MT>,
                      "Message is neither std::string nor std::stringstream");

        if(!Logger::isLoggable(level)) {
            return;
        }

        std::stringstream msgSS;
        if constexpr(std::is_same_v<MT, std::string>) {
            msgSS << message;
        } else if constexpr(std::is_same_v<MT, std::stringstream>) {
            msgSS = message;
        }

        if(exception != nullptr) {
            msgSS << std::endl << exception->what();
        }

        LogRecord logRecord;
        logRecord.level = level;
        logRecord.timePoint = timePoint;
        logRecord.message = std::make_shared<std::string>(msgSS.str());
        logRecord.loggerName = this->_name;

        doLog(logRecord);
    }

    /**
     * @brief Send a message to the log with the given level and with the given timepoint recorded. The message is
     * retrieved from the supplied producer function.
     * @tparam MT type of the message. Must be either std::string or std::stringstream
     * @param messageProducer
     * @param exception
     * @param level
     * @param timePoint
     */
    template<class MT>
    void log(const std::function<MT()> &messageProducer,
             const std::exception *exception = nullptr,
             const LogLevel level = Logger::defaultLevel,
             std::chrono::time_point<std::chrono::system_clock> timePoint = std::chrono::system_clock::now()) noexcept {
        static_assert(nanoservices::is_string_or_stringstream<MT>,
                      "Log message producer function returns neither std::string nor std::stringstream");

        if(!Logger::isLoggable(level)) {
            return;
        }

        MT message = messageProducer();

        log(message, exception, level, timePoint);
    }
};

} // namespace nanoservices

#endif // LOGGING_H_
