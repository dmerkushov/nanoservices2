//
// Created by dmerkushov on 3/13/21.
//

#include "configuration.h"

#include <stdexcept>
#include <string.h>

using namespace std;
using namespace nanoservices;

map<string, shared_ptr<string>, less<>> configuration::_properties;

#ifndef NANOSERVICES_CONF_PLUGIN_IMPLEMENTS_GETPROPERTY_STRING
shared_ptr<string> configuration::get_property(const string &property_name) noexcept {
    if(property_name == "") {
        return make_shared<string>();
    }

    auto property_find_iterator = configuration::_properties.find(property_name);

    shared_ptr<string> result;
    if(property_find_iterator == configuration::_properties.end()) {
        result = nullptr;
        configuration::_properties[property_name] = result;
    } else {
        result = property_find_iterator->second;
    }

    return result;
}
#endif

shared_ptr<string> configuration::get_property(const char *property_name) noexcept {
    if(property_name == nullptr) {
        return make_shared<string>();
    }

    if(::strncmp("", property_name, 1) == 0) {
        return make_shared<string>();
    }

    if(::strnlen(property_name, MAX_PROPERTYNAME_LEN_CONSTCHARPTR + 1) == MAX_PROPERTYNAME_LEN_CONSTCHARPTR + 1) {
        return make_shared<string>();
    }

    string propertyNameStr(property_name);

    return get_property(propertyNameStr);
}

// shared_ptr<bool> configuration::get_property_bool(const char *property_name) noexcept {
//     if(property_name == nullptr) {
//         return make_shared<bool>();
//     }
//
//     if(::strncmp("", property_name, 1) == 0) {
//         return make_shared<bool>();
//     }
// }