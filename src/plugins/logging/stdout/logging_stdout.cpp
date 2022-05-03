//
// Created by dmerkushov on 04.09.2021.
//

#include "../../configuration/configuration.h"
#include "../logging.h"

#include <string>

using namespace nanoservices;
using namespace std;

void nanoservices::logging_initialize() noexcept {
    shared_ptr<string> levelName = configuration::get_property("nanoservices.logging.level");
    if(!levelName) {
        levelName = make_shared<string>(LOGGING_LEVELNAME_DEFAULT);
    }
    log::level::level_enum level = nanoservices::logging_level_by_name(levelName->c_str());
    log::set_level(level);

    shared_ptr<string> logPattern = configuration::get_property("nanoservices.logging.pattern");
    if(!logPattern) {
        logPattern = make_shared<string>(LOGGING_PATTERN_DEFAULT);
    }
    log::set_pattern(*logPattern);
}

void nanoservices::logging_finalize() noexcept {
    // Do nothing
}