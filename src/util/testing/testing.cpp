#include "testing.h"

#include "../../core/exception/ns_exception.h"
#include "../../plugins/logging/logging.h"

#include <exception>

using namespace std;
using namespace nanoservices;

ns_testing_context::ns_testing_context() noexcept {
}

ns_testing_context::~ns_testing_context() noexcept {
}

void ns_testing_context::run_tests() {
    map<string, bool> testResults;

    for(auto it = _tests.begin(); it != _tests.end(); it++) {
        bool result = it->second();
        log::info("==== Performing test: {}", it->first);
        try {
            testResults[it->first] = it->second();
        } catch(ns_exception &e) {
            log::warn("==== Test {} failed due to a ns_exception: {}", it->first, e.what());
            testResults[it->first] = false;
        } catch(exception &e) {
            log::warn("==== Test {} failed due to a std::exception: {}", it->first, e.what());
            testResults[it->first] = false;
        }
        log::info("==== Test {} result: {}", it->first, testResults[it->first] ? "OK" : "Failure");
    }
}