//
// Created by dmerkushov on 3/13/21.
//

#include "../configuration.h"

#include <algorithm>

using namespace std;
using namespace nanoservices;

void nanoservices::configuration::initialize(int argc, char **argv) noexcept {
    // argv[0] is the executable name, argv[1] and agrv[2] are required for at
    // least one configuration property So, minimum argc to fill up configuration
    // is 3
    if(argc < 3) {
        return;
    }

    for(int i = 1; i < argc - 1; i += 2) {
        string name(argv[i]);
        string value(argv[i + 1]);
        configuration::_properties[name] = make_shared<string>(value);
    }
}

void nanoservices::configuration::finalize() noexcept {
    // Do nothing
}
