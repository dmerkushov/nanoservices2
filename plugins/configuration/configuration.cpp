//
// Created by dmerkushov on 3/13/21.
//

#include "configuration.h"

#include <stdexcept>
#include <string.h>

using namespace std;
using namespace nanoservices;

map<string, shared_ptr<string>, less<>> Configuration::_properties;

#ifndef NANOSERVICES_CONF_PLUGIN_IMPLEMENTS_GETPROPERTY_STRING
shared_ptr<string> Configuration::getProperty(const string &propertyName) noexcept {
    if(propertyName == "") {
        return make_shared<string>();
    }

    auto propertyFindIterator = Configuration::_properties.find(propertyName);

    shared_ptr<string> result;
    if(propertyFindIterator == Configuration::_properties.end()) {
        result = nullptr;
        Configuration::_properties[propertyName] = result;
    } else {
        result = propertyFindIterator->second;
    }

    return result;
}
#endif

shared_ptr<string> Configuration::getProperty(const char *propertyName) noexcept {
    if(propertyName == nullptr) {
        return make_shared<string>();
    }

    if(::strncmp("", propertyName, 1) == 0) {
        return make_shared<string>();
    }

    if(::strnlen(propertyName, MAX_PROPERTYNAME_LEN_CONSTCHARPTR + 1) == MAX_PROPERTYNAME_LEN_CONSTCHARPTR + 1) {
        return make_shared<string>();
    }

    string propertyNameStr(propertyName);

    return getProperty(propertyNameStr);
}
