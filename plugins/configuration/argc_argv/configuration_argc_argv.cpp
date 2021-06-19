//
// Created by dmerkushov on 3/13/21.
//

#include "../configuration.h"

#include <map>
#include <algorithm>

using namespace std;
using namespace nanoservices;

static map<string, shared_ptr<string>, less<>> properties;

void nanoservices::conf_initialize(int argc, char **argv) noexcept {
    // argv[0] is the executable name, argv[1] and agrv[2] are required for at least one configuration property
    // So, minimum argc to fill up configuration is 3
    if (argc < 3) {
        return;
    }

    for (
            int i = 1; i < argc - 1; i += 2
            ) {
        string name(argv[i]);
        string value(argv[i + 1]);
        properties[name] = make_shared<string>(value);
    }
}

void nanoservices::conf_finalize() noexcept {
    // Do nothing
}

shared_ptr<string> nanoservices::conf_getProperty(const char *propertyName) noexcept {
    if (propertyName == nullptr) {
        return make_shared<string>();
    }

    string name(propertyName);

    auto search = properties.find(name);

    if (search != properties.end()) {
        return search->second;
    } else {
        return make_shared<string>();
    }
}

