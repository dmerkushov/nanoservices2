/*
 * Copyright 2019 dmerkushov.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
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
 * File:   ns_exception.h
 * Author: dmerkushov
 *
 * Created on February 21, 2019, 6:39 PM
 */

#ifndef NSEXCEPTION_H_
#define NSEXCEPTION_H_

#include "../../util/macroutils/position.h"

#include <exception>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#ifndef NS_EXCEPTION
#    define NS_EXCEPTION(message) ns_exception((message), std::make_shared<std::string>(NS_POSITION))
#    define NS_EXCEPTION_WITHCAUSE(message, cause) ns_exception((message), std::make_shared<std::string>(NS_POSITION), (cause))
#    define NS_THROW(message) throw NS_EXCEPTION(message)
#    define NS_THROW_WITHCAUSE(message, cause) throw NS_EXCEPTION_WITHCAUSE((message), (cause))
#endif

namespace nanoservices {

/**
 * @brief An immutable exception class for all over nanoservices
 */
class ns_exception : public std::exception {
private:
    /**
     * @brief The maximum number of stacktrace elements of the stacktrace kept in an exception
     */
    static const size_t STACKTRACE_SIZE_MAX;

    std::shared_ptr<std::string> _message;
    std::shared_ptr<std::string> _position;
    std::shared_ptr<std::exception> _cause;

    mutable std::shared_ptr<std::string> _what;

public:
    ns_exception(const char *message, const std::shared_ptr<std::string> position) noexcept;

    ns_exception(const std::shared_ptr<std::string> message, const std::shared_ptr<std::string> position, const std::shared_ptr<std::exception> cause = nullptr) noexcept;

    ~ns_exception() noexcept override = default;

    /**
     * @brief Inherited from std::exception
     * @details
     * @return The full description of the exception, including position, message and the stacktrace. If a cause is set,
     * then the result of the what() call for the causing exception is appended.
     */
    const char *what() const noexcept override;

    /**
     * @brief Get the message of the exception
     * @return
     */
    std::shared_ptr<std::string> message() const noexcept;

    /**
     * @brief Get the position where the ns_exception was constructed
     * @return
     */
    std::shared_ptr<std::string> position() const noexcept;

    /**
     * @brief Get the causing exception
     * @return A shared pointer to the causing exception, or an empty shared pointer if none is set as a cause
     */
    std::shared_ptr<std::exception> cause() const noexcept;
};

} // namespace nanoservices

#endif // NSEXCEPTION_H_