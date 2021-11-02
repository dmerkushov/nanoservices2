//
// Created by dmerkushov on 3/13/21.
//

#include "monitoring.h"

using namespace std;
using namespace nanoservices;

void nanoservices::monitoring::initialize() noexcept {
    // Do nothing
}

void nanoservices::monitoring::finalize() noexcept {
    // Do nothing
}

bool nanoservices::monitoring::active() noexcept {
    return false;
}

void nanoservices::monitoring::event([[maybe_unused]] uint32_t eventId, [[maybe_unused]] shared_ptr<map<string, string>> eventData) noexcept {
    // Do nothing
}
