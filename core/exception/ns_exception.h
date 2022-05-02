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

#include "../../thirdparty/gabime/spdlog/fmt/fmt.h"
#include "../../util/macroutils/position.h"

#include <exception>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#ifndef NS_EXCEPTION
#    define NS_THROW(message) throw ns_exception((message), std::make_shared<std::string>(NS_POSITION))
#    define NS_THROW_WITHCAUSE(cause, message) throw ns_exception((message), std::make_shared<std::string>(NS_POSITION), (cause))
#    define NS_THROW_FMT(...) throw NS_EXCEPTION(fmt::format(...).c_str())
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

    std::shared_ptr<std::exception> _cause;
    std::shared_ptr<std::string> _message;
    std::shared_ptr<std::string> _position;

    mutable std::shared_ptr<std::string> _what;

public:
    ns_exception(const char *message, const std::shared_ptr<std::string> position) noexcept;

    ns_exception(const exception &cause, const std::shared_ptr<std::string> message, const std::shared_ptr<std::string> position) noexcept;

    ns_exception(const std::shared_ptr<std::exception> cause, const std::shared_ptr<std::string> message, const std::shared_ptr<std::string> position) noexcept;

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

namespace fmt {

template<>
struct formatter<nanoservices::ns_exception> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx);

    template<typename FormatContext>
    auto format(nanoservices::ns_exception const &e, FormatContext &ctx);
};

template<typename ParseContext>
constexpr auto formatter<nanoservices::ns_exception>::parse(ParseContext &ctx) {
    return std::begin(ctx);
}

template<typename FormatContext>
auto formatter<nanoservices::ns_exception>::format(nanoservices::ns_exception const &e, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "{}", e.what());
}

} // namespace fmt

#endif // NSEXCEPTION_H_