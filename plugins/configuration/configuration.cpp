//
// Created by dmerkushov on 3/13/21.
//

#include "configuration.h"

using namespace std;

std::shared_ptr<std::string> nanoservices::conf_getProperty(std::shared_ptr<std::string> propertyName) noexcept {
    if(!propertyName) {
        return std::make_shared<std::string>();
    }

    return conf_getProperty(propertyName->c_str());
}