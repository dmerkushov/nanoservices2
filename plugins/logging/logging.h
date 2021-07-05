//
// Created by dmerkushov on 3/20/21.
//

#ifndef LOGGING_H_
#define LOGGING_H_

#include "../../util/stringutils/stringutils.h"

#include <cassert>
#include <chrono>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <type_traits>

namespace nanoservices {

/**
 * @brief The name for the default logger
 */
extern const std::string LOGGING_LOGGER_NAME_DEFAULT;

/**
 * @brief The name of the default logging level
 */
extern const std::string LOGGING_LOGGER_LEVELNAME_DEFAULT;

/**
 * @brief The concept denotes a loggable entity: any of
 * <ul>
 * <li>anything convertible to std::string
 * <li>std::stringstream
 * <li>a string or stringstream producer: a function that returns any of
 * <ul>
 * <li>anything convertible to std::string
 * <li>std::stringstream
 * </ul>
 * when called without parameters
 * </ul>
 * @tparam MT
 */
template<class MT>
concept LogMessage = StringOrSStream<MT> || StringOrSStreamProducer<MT>;

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
     * @brief Mutex to control access to the loggers map
     */
    static std::mutex _name2LoggerMapMutex;

    /**
     * @brief Map logger names to loggers
     */
    static std::map<std::string, std::shared_ptr<Logger>, std::less<>> _name2LoggerMap;

    /**
     * @brief Create a logger with the given name and the configured log level, or, if there is no configured log level,
     * the default log level.
     * @details In configuration, the property named "nanoservices.logging.{name}.level" provides the name of the
     * log level for the logger ("{name}" is substituted by the logger name). If no such property is found, the
     * property named "nanoservices.logging.level" is used. If no such property is found, too, the log level is
     * set to the default value, Logger::defaultLevel.
     * @param name
     */
    explicit Logger(const std::string &name) noexcept;

    /**
     * @brief Write the supplied log record to the log.
     * @details One of the three methods to be implemented by the logging plugin. Others are: Logger::initialize()
     * and Logger::finalize().
     * @details No log level check is needed in this method, it is done beforehand in user-callable methods.
     * @details This method must guarantee the following: for any two subsequent entries into this method, the
     * corresponding log records in the resulting log will follow the same order (if applicable).
     * @details This method must return only after the logging operation is over.
     * @details This method must be thread-safe.
     * @param logRecord The log record to be logged
     */
    void doLog(const LogRecord &logRecord) const noexcept;

public:
    /**
     * @brief Initialize the logging engine.
     * @details One of the three methods to be implemented by the logging plugin. Others are:
     * Logger::doLog(LogRecord&) and Logger::finalize().
     * @details This method is called exactly once at the start of the nanoservice. The method call is not required
     * to be thread-safe.
     */
    static void initialize() noexcept;

    /**
     * @brief Finalize the logging engine.
     * @details One of the three methods to be implemented by the logging plugin. Others are:
     * Logger::doLog(LogRecord&) and Logger::initialize().
     * @details This method is not guaranteed to be called, but if called, it is called exactly once when the
     * nanoservice is finishing, right before finalizing the configuration engine.
     * @details The method is not required to be thread-safe.
     */
    static void finalize() noexcept;

    /**
     * @brief Get a logger by its name.
     * @details The returned logger will have the supplied name (case sensitive) and the configured log level for
     * that name.
     * @details In configuration, the property named "nanoservices.logging.{name}.level" provides the name of the
     * log level for the logger ("{name}" is substituted by the logger name). If no such property is found, the
     * property named "nanoservices.logging.level" is used. If no such property is found, too, the log level is
     * set to the default value, nanoservices::LOGGING_LOGGER_LEVELNAME_DEFAULT.
     * @details If such a logger had been already constructed, the same instance is pointed to by the returned
     * shared pointer.
     * @details The returned shared pointer is guaranteed to be not empty.
     * @details This method is thread-safe, but it uses a blocking std::map to access loggers by their names. For best
     * performance, a rare use of this method is recommended.
     * @param name
     * @return
     */
    static std::shared_ptr<Logger> getLogger(const std::string &name = LOGGING_LOGGER_NAME_DEFAULT) noexcept;

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
    static const char *levelName(const LogLevel level) noexcept;

    /**
     * @brief Set the given log level for the logger
     * @details This method is thread-safe.
     * @param level
     */
    void setLevel(const LogLevel level = Logger::levelByName(LOGGING_LOGGER_LEVELNAME_DEFAULT.c_str())) noexcept;

    /**
     * @brief Set the given log level for the logger
     * @details This method is thread-safe.
     * @param levelName
     */
    void setLevel(const char *levelName = LOGGING_LOGGER_LEVELNAME_DEFAULT.c_str()) noexcept;

    /**
     * @brief Get the current log level for the logger
     * @details This method is thread-safe.
     * @return
     */
    LogLevel level() const noexcept;

    /**
     * @brief Is the given level currently loggable by the logger?
     * @details This method is thread-safe.
     * @param logLevel
     * @return
     */
    bool isLoggable(const LogLevel logLevel) const noexcept;

    /**
     * @brief Send a message to the log at the TRACE level
     * @details This method is thread-safe.
     * @details This method is not intended to be implemented by the plugin.
     * @tparam MT message type must be either std::string, or std::stringstream, or a string producer function
     * @param message or message producer function
     * @param exception an optional exception parameter. If not empty, the exception message will be added to the
     * log. message, delimited by std::endl. The exception message is retrieved by running std::exception::what()
     */
    template<LogMessage MT>
    void trace(MT &message, std::shared_ptr<std::exception> exception = nullptr) noexcept {
        if(!isLoggable(LogLevel::TRACE)) {
            return;
        }

        log(message, LogLevel::TRACE, exception);
    }

    /**
     * @brief Send a message to the log at the DEBUG level
     * @details This method is thread-safe.
     * @details This method is not intended to be implemented by the plugin.
     * @tparam MT message type must be either std::string, or std::stringstream, or a string producer function
     * @param message or message producer function
     * @param exception an optional exception parameter. If not empty, the exception message will be added to the
     * log. message, delimited by std::endl. The exception message is retrieved by running std::exception::what()
     */
    template<LogMessage MT>
    void debug(MT &message, std::shared_ptr<std::exception> exception = nullptr) noexcept {
        if(!isLoggable(LogLevel::DEBUG)) {
            return;
        }

        log(message, LogLevel::DEBUG, exception);
    }

    /**
     * @brief Send a message to the log at the INFO level
     * @details This method is thread-safe.
     * @details This method is not intended to be implemented by the plugin.
     * @tparam MT message type must be either std::string, or std::stringstream, or a string producer function
     * @param message or message producer function
     * @param exception an optional exception parameter. If not empty, the exception message will be added to the
     * log. message, delimited by std::endl. The exception message is retrieved by running std::exception::what()
     */
    template<LogMessage MT>
    void info(MT &message, std::shared_ptr<std::exception> exception = nullptr) noexcept {
        if(!isLoggable(LogLevel::INFO)) {
            return;
        }

        log(message, LogLevel::INFO, exception);
    }

    /**
     * @brief Send a message to the log at the WARN level
     * @details This method is thread-safe.
     * @details This method is not intended to be implemented by the plugin.
     * @tparam MT message type must be either std::string, or std::stringstream, or a string producer function
     * @param message or message producer function
     * @param exception an optional exception parameter. If not empty, the exception message will be added to the
     * log. message, delimited by std::endl. The exception message is retrieved by running std::exception::what()
     */
    template<LogMessage MT>
    void warn(MT &message, std::shared_ptr<std::exception> exception = nullptr) noexcept {
        if(!isLoggable(LogLevel::WARN)) {
            return;
        }

        log(message, LogLevel::WARN, exception);
    }

    /**
     * @brief Send a message to the log at the ERROR level
     * @details This method is thread-safe.
     * @details This method is not intended to be implemented by the plugin.
     * @tparam MT message type must be either std::string, or std::stringstream, or a string producer function
     * @param message or message producer function
     * @param exception an optional exception parameter. If not empty, the exception message will be added to the
     * log. message, delimited by std::endl. The exception message is retrieved by running std::exception::what()
     */
    template<LogMessage MT>
    void error(MT &message, std::shared_ptr<std::exception> exception = nullptr) noexcept {
        if(!isLoggable(LogLevel::ERROR)) {
            return;
        }

        log(message, LogLevel::ERROR, exception);
    }

    /**
     * @brief Send a message to the log at the FATAL level
     * @details This method is thread-safe.
     * @details This method is not intended to be implemented by the plugin.
     * @tparam MT message type must be either std::string, or std::stringstream, or a string producer function
     * @param message or message producer function
     * @param exception an optional exception parameter. If not empty, the exception message will be added to the
     * log. message, delimited by std::endl. The exception message is retrieved by running std::exception::what()
     */
    template<LogMessage MT>
    void fatal(MT &message, std::shared_ptr<std::exception> exception = nullptr) noexcept {
        if(!isLoggable(LogLevel::FATAL)) {
            return;
        }

        log(message, LogLevel::FATAL, exception);
    }

    /**
     * @brief Send a message to the log with the given level and with the given timepoint recorded
     * @tparam MT type of the message
     * @param message
     * @param exception if not nullptr, its message will be added to the log message, delimited by std::endl
     * @param level
     * @param timePoint
     */
    template<LogMessage MT>
    void log(const MT &message,
             const LogLevel level = Logger::levelByName(LOGGING_LOGGER_LEVELNAME_DEFAULT.c_str()),
             const std::shared_ptr<std::exception> exception = nullptr,
             const std::chrono::time_point<std::chrono::system_clock> timePoint =
                     std::chrono::system_clock::now()) const noexcept {

        if(!Logger::isLoggable(level)) {
            return;
        }

        static_assert(
                String<MT> || SStream<MT> || StringProducer<MT> || SStreamProducer<MT>,
                "Will not be able to create a message: MT is unexpected. Check the LogMessage<MT> concept for any additions");

        LogRecord logRecord;
        if constexpr(String<MT>) {
            logRecord.message = std::make_shared<std::string>(message);
        } else if constexpr(SStream<MT>) {
            logRecord.message = std::make_shared<std::string>(message.str());
        } else if constexpr(StringProducer<MT>) {
            logRecord.message = std::make_shared<std::string>(message());
        } else if constexpr(SStreamProducer<MT>) {
            logRecord.message = std::make_shared<std::string>(message().str());
        }

        if(exception) {
            std::stringstream msgSS;
            msgSS << *logRecord.message << std::endl << exception->what();
            logRecord.message = std::make_shared<std::string>(msgSS.str());
        }

        logRecord.level = level;
        logRecord.timePoint = timePoint;
        logRecord.loggerName = this->_name;

        doLog(logRecord);
    }
};

} // namespace nanoservices

#endif // LOGGING_H_
