#include "core/exception/NsException.h"
#include "core/serialization/serialization.h"
#include "plugins/configuration/configuration.h"
#include "plugins/logging/logging.h"
#include "thirdparty/swansontec/map-macro/do_foreach.h"

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace nanoservices;

class MyClass1 {
public:
    int32_t myField1 = 234;

    NANOSERVICES2_MAKE_SERIALIZABLE(myField1);
};

class MyClass2 {
    MyClass1 myField2;

    NANOSERVICES2_MAKE_SERIALIZABLE(myField2);
};

int main(int argc, char **argv) {
    Configuration::initialize(argc, argv);
    Logger::initialize();
    shared_ptr<Logger> logger = Logger::getLogger();

    logger->info("+main()");

    MyClass myClass;
    auto serialized = myClass.__nanoservices_serialize();

    {
        stringstream msgSS;
        auto serializedListData =
                static_pointer_cast<std::vector<std::shared_ptr<SerializationRecord>>>(serialized->at(0)->data);

        for(auto iter = serializedListData->begin(); iter != serializedListData->end(); ++iter) {
            auto serializedListRec = *iter;
            auto serializedListRecData = static_pointer_cast<int32_t>(serializedListRec->data);
            msgSS << "Serialized list: " << *serializedListRecData << endl;
        }
        logger->info(msgSS);
    }
    {
        stringstream msgSS;
        auto serializedMapData =
                static_pointer_cast<std::vector<std::shared_ptr<SerializationRecord>>>(serialized->at(1)->data);

        auto iter = serializedMapData->begin();
        auto serializedMapRec = *iter;
        msgSS << serializedMapRec->type << endl;
        auto serializedMapRecData = static_pointer_cast<int32_t>(serializedMapRec->data);
        msgSS << "Serialized map: " << *serializedMapRecData << endl;
        logger->info(msgSS);
    }

    logger->info("-main()");

    Logger::finalize();
    Configuration::finalize();

    return 0;
}
