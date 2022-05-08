#include "core/serialization/serialization.h"
#include "plugins/configuration/configuration.h"
#include "plugins/logging/logging.h"
#include "thirdparty/gabime/spdlog/spdlog.h"
#include "util/testing/testing.h"

using namespace std;
using namespace nanoservices;

int main(int argc, char **argv) {
    configuration::initialize(argc, argv);
    logging_initialize();

    log::info("nanoservices2: unit testing running");

    NSTEST_RUN

    log::info("nanoservices2: unit testing finished");

    logging_finalize();
    configuration::finalize();
    return 0;
}
