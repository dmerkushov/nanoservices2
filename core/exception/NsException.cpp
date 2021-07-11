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
 * File:   NsException.cpp
 * Author: dmerkushov
 *
 * Created on February 21, 2019, 6:39 PM
 */

#include "NsException.h"

#include <cstring>
#include <memory>
#include <sstream>
#include <typeinfo>

using namespace nanoservices;
using namespace std;

const size_t NsException::STACKTRACE_SIZE_MAX = 200;

NsException::NsException(const std::shared_ptr<std::string> message,
                         const std::shared_ptr<std::string> position,
                         const std::shared_ptr<std::exception> cause) noexcept :
        _message(message), _position(position), _cause(cause) {
}

NsException::NsException(const char *message, const std::shared_ptr<std::string> position) noexcept :
        NsException(make_shared<string>(message), position) {
}

const char *NsException::what() const noexcept {
    if(!_what) {
        // Collect info for what()
        stringstream wss;
        wss << "Exception at " << *_position << ": " << *_message;
        if(_cause) {
            wss << endl << "Caused by: " << _cause->what();
        }
        _what = make_shared<string>(wss.str());
    }
    return _what->c_str();
}

std::shared_ptr<std::string> NsException::message() const noexcept {
    return _message;
}

std::shared_ptr<std::exception> NsException::cause() const noexcept {
    return _cause;
}

std::shared_ptr<std::string> NsException::position() const noexcept {
    return _position;
}
