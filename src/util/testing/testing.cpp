#include "testing.h"

#include "../../core/exception/ns_exception.h"
#include "../../plugins/logging/logging.h"

#include <exception>
#include <spdlog/fmt/bundled/color.h>
#include <spdlog/fmt/bundled/printf.h>

using namespace std;
using namespace nanoservices;

const string testPassedIndicator = fmt::format(fmt::bg(fmt::terminal_color::green) | fmt::fg(fmt::terminal_color::bright_white) | fmt::emphasis::bold, " PASSED ");
const string testFailedIndicator = fmt::format(fmt::bg(fmt::terminal_color::red) | fmt::fg(fmt::terminal_color::bright_white) | fmt::emphasis::bold, " FAILED ");

void ns_testing_context::run_tests() {
    log::info("==== Performing tests");

    map<string, bool> testResults;

    for(auto it = _tests.begin(); it != _tests.end(); it++) {
        log::info("---- Performing test: {}", it->first);

        bool result;

        try {
            result = it->second();
        } catch(ns_exception &e) {
            log::warn(".... Test {} failed due to a ns_exception: {}", it->first, e.what());
            result = false;
        } catch(exception &e) {
            log::warn(".... Test {} failed due to a std::exception: {}", it->first, e.what());
            result = false;
        }
        testResults[it->first] = result;
        if(result) {
            log::info(".... Test {} {}", it->first, testPassedIndicator);
        } else {
            log::info(".... Test {} {}", it->first, testFailedIndicator);
        }
    }

    log::info("==== Test results:");

    size_t totalCount = 0;
    size_t passedCount = 0;
    size_t failedCount = 0;

    for(auto it = testResults.begin(); it != testResults.end(); it++) {
        totalCount++;
        if(it->second) {
            passedCount++;
        } else {
            failedCount++;
        }

        log::info("{} {}", it->second ? testPassedIndicator : testFailedIndicator, it->first);
    }

    log::info("==== Performing tests finished: total {} tests, passed {} ({}%), failed {} ({}%)",
              totalCount,
              passedCount,
              fmt::sprintf("%.2f", passedCount * 100.0 / totalCount),
              failedCount,
              fmt::sprintf("%.2f", passedCount * 100.0 / totalCount));
}