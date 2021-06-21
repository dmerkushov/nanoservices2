//
// Created by dmerkushov on 3/13/21.
//

#include "monitoring.h"

using namespace std;
using namespace nanoservices;

void nanoservices::mon_initialize() noexcept {
    // Do nothing
}

void nanoservices::mon_finalize() noexcept {
    // Do nothing
}

bool nanoservices::mon_active() noexcept {
    return false;
}

void nanoservices::mon_event([[maybe_unused]] uint32_t eventId,
                             [[maybe_unused]] shared_ptr<map<string, string>> eventData) noexcept {
    // Do nothing
}
