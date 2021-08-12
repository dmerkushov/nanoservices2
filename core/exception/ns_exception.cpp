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
 * File:   ns_exception.cpp
 * Author: dmerkushov
 *
 * Created on February 21, 2019, 6:39 PM
 */

#include "ns_exception.h"

#include "../../thirdparty/gabime/spdlog/fmt/fmt.h"

#include <cstring>
#include <memory>
#include <sstream>
#include <typeinfo>

using namespace nanoservices;
using namespace std;

const size_t ns_exception::STACKTRACE_SIZE_MAX = 200;

ns_exception::ns_exception(const std::shared_ptr<std::string> message, const std::shared_ptr<std::string> position, const std::shared_ptr<std::exception> cause) noexcept :
        _message(message), _position(position), _cause(cause) {
}

ns_exception::ns_exception(const char *message, const std::shared_ptr<std::string> position) noexcept : ns_exception(make_shared<string>(message), position) {
}

const char *ns_exception::what() const noexcept {
    if(!_what) {
        // Collect info for what()
        string what;
        if(_cause) {
            const char *causeWhat = dynamic_pointer_cast<ns_exception>(_cause)->what();
            what = fmt::format("Exception at {}: {}\nCaused by: {}", *_position, *_message, causeWhat);
        } else {
            what = fmt::format("Exception at {}: {}", *_position, *_message);
        }
        _what = make_shared<string>(what);
    }
    return _what->c_str();
}

std::shared_ptr<std::string> ns_exception::message() const noexcept {
    return _message;
}

std::shared_ptr<std::exception> ns_exception::cause() const noexcept {
    return _cause;
}

std::shared_ptr<std::string> ns_exception::position() const noexcept {
    return _position;
}
