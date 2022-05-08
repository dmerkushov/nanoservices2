#include "testing.h"

#include "../../core/exception/ns_exception.h"
#include "../../plugins/logging/logging.h"
#include "../../util/typeutils/demangle.h"

#include <exception>
#include <spdlog/fmt/bundled/color.h>

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
        } catch(exception const &e) {
            log::warn(".... Test {} failed due to an exception derived from std::exception: {}: {}", it->first, nanoservices::internal::demangle_type_name(typeid(e).name()), e.what());
            result = false;
        } catch(...) {
            log::warn(".... Test {} failed due to a thrown non-an-std::exception", it->first);
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

    uint32_t totalCount = 0;
    uint32_t passedCount = 0;
    uint32_t failedCount = 0;

    for(auto it = testResults.begin(); it != testResults.end(); it++) {
        totalCount++;
        it->second ? passedCount++ : failedCount++;

        log::info("{} {}", it->second ? testPassedIndicator : testFailedIndicator, it->first);
    }

    log::info("==== Performing tests finished: total {} tests, {} ({:.2f}%) passed, {} ({:.2f}%) failed",
              totalCount,
              passedCount,
              passedCount * 100.0 / totalCount,
              failedCount,
              failedCount * 100.0 / totalCount);
}