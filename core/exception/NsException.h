/*
 * Copyright 2019 dmerkushov.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * File:   NsException.h
 * Author: dmerkushov
 *
 * Created on February 21, 2019, 6:39 PM
 */

#ifndef NSEXCEPTION_H_
#define NSEXCEPTION_H_

#include <exception>
#include <sstream>
#include <string>
#include <vector>

#define NS_POSITION_FOR_EXCEPT \
    ((std::string("") + __FILE__ + ":" + std::to_string(__LINE__) + ": in " + __PRETTY_FUNCTION__).c_str())

#ifndef NS_EXCEPTION
#    define NS_EXCEPTION(message) NsException(NS_POSITION_FOR_EXCEPT, message)
#    define NS_EXCEPTION(message, cause) NsException(NS_POSITION_FOR_EXCEPT, message, cause)
#endif

namespace nanoservices {

class NsException : public std::exception {
private:
    /**
     * @brief The maximum size of the stacktrace kept in an exception
     */
    static const size_t STACKTRACE_SIZE_MAX;

    std::string _message;
    std::string _fullDescription;
    std::string _stacktrace;
    std::string _rootExceptionFullDescription;

public:
    NsException(const char *position, const char *message) noexcept;

    NsException(const char *position, const char *message, const std::exception &cause) noexcept;

    ~NsException() noexcept override;

    /**
     * Inherited from std::exception
     * @return The full description of the exception
     */
    const char *what() const noexcept override;

    /**
     *
     * @return Stacktrace of the exception
     */
    const std::string &stacktrace() const noexcept;

    /**
     * @brief Get the message of the exception
     * @return
     */
    const std::string &message() const noexcept;

    /**
     * @brief Get the short description of the exception: only the message.
     * file, line, and function name, and the message of the cause exception, if
     * any
     * @return
     */
    const std::string &shortDescription() const noexcept;

    /**
     * @brief Get the full description of the exception:
     * <ul>
     * <li>the message,
     * <li>file, line, function name of the exception creation point,
     * <li>the stacktrace
     * <li>the same, recursively, for the cause exception (if any)
     * </ul>
     * @return Full description of the exception; the same string as would be
     * constructed if calling what()
     */
    const std::string &fullDescription() const noexcept;

    /**
     *
     * @return Full description of the root exception; empty string if no root
     * exception provided
     */
    const std::string &rootExceptionFullDescription() const noexcept;

private:
    /**
     * Utility method to use in the constructors
     * @param rootExStack
     * @param rootExShort
     * @param rootExFull
     */
    void init(const std::string &rootExStack = "",
              const std::string &rootExShort = "",
              const std::string &rootExFull = "") noexcept;
};

///**
// * @brief Log an event with an exception at the given log level
// * @param message Description of the event
// * @param exception The exception
// * @param level the level of the event. ERROR by default
// */
// void log(std::string &message, NsException &exception, LogLevel level =
// LogLevel::ERROR) noexcept;
//
///**
// * @brief Log an event with an exception at the given log level
// * @param message Description of the event
// * @param exception The exception
// * @param level the level of the event. ERROR by default
// */
// void log(std::stringstream &message, NsException &exception, LogLevel level =
// LogLevel::ERROR) noexcept;
} // namespace nanoservices

struct LogRecordWithException : LogRecord {

    /**
     * @brief An exception that is bound to the event; may be an empty shared pointer
     */
    std::shared_ptr<NsException> exception;
};

#endif /* NSEXCEPTION_H */
