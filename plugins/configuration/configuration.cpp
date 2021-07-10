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

    shared_ptr<string> result;
    try {
        result = Configuration::_properties.at(propertyName);
    } catch(const std::out_of_range &) {
        result = nullptr;
        Configuration::_properties[propertyName] = result;
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
