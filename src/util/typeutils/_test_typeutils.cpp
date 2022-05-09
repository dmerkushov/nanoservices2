//
// Created by dmerkushov on 09.05.22.
//

#include "../../core/exception/ns_exception.h"
#include "../../plugins/logging/logging.h"
#include "../testing/testing.h"
#include "demangle.h"

using namespace std;
using namespace nanoservices;

function<bool()> test_demangle_type_name = []() {
    auto performCheck = [](const char *typeid_name, bool expectException, const char *check) {
        string checkStr(check);

        bool exceptionThrown = false;

        shared_ptr<string> singleDemangle;
        try {
            singleDemangle = internal::demangle_type_name(typeid_name, true);
        } catch(ns_exception &nse) {
            singleDemangle = nse.message();
            exceptionThrown = true;
        }

        bool checkResult;
        if(exceptionThrown) {
            if(expectException) {
                checkResult = checkStr.compare(*singleDemangle) == 0;
            } else {
                checkResult = false;
            }
        } else if(expectException) {
            checkResult = false;
        } else {
            checkResult = checkStr.compare(*singleDemangle) == 0;
        }

        log::debug("Checking \"{}\", expecting exception {}, expected result \"{}\" -> exception thrown {}, actual result \"{}\" : {}",
                   typeid_name,
                   expectException,
                   check,
                   exceptionThrown,
                   singleDemangle,
                   checkResult ? "OK" : "FAILED");
        return checkResult;
    };

    bool result = true;
    result &= performCheck("void", true, "Demangling failed: \"void\". Mangled name is not a valid name under the C++ mangling rules");
    result &= performCheck(typeid(void).name(), false, "void");
    result &= performCheck(typeid(result).name(), false, "bool");
    result &= performCheck(typeid(ns_exception).name(), false, "nanoservices::ns_exception");

    return result;
};

NSTEST_ADD(test_demangle_type_name)