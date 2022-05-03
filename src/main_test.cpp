#define DOCTEST_CONFIG_IMPLEMENT

#include "core/serialization/serialization.h"
#include "plugins/configuration/configuration.h"
#include "plugins/logging/logging.h"
#include "thirdparty/doctest/doctest/doctest.h"
#include "thirdparty/gabime/spdlog/spdlog.h"

using namespace std;
using namespace nanoservices;

namespace logger = spdlog;

int main(int argc, char **argv) {
    configuration::initialize(argc, argv);
    logging_initialize();

    log::info("nanoservices2: unit tests running");

    doctest::Context context;
    context.setOption("reporters", "console");
    context.applyCommandLine(argc, argv);
    int doctest_result = context.run();

    log::info("nanoservices2: unit tests finished. shouldExist: {}, exit code: {}", context.shouldExit(), doctest_result);

    if(context.shouldExit()) {
        return doctest_result;
    }

    return 0;
}
