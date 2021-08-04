//#include "core/exception/NsException.h"
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

class MyEnclosedClass {
public:
    int32_t enclosedField = 234;

    NANOSERVICES2_MAKE_SERIALIZABLE(enclosedField);
};

class MyEnclosingClass {
public:
    MyEnclosedClass enclosingField;

    NANOSERVICES2_MAKE_SERIALIZABLE(enclosingField);
};

int main(int argc, char **argv) {
    Configuration::initialize(argc, argv);
    Logger::initialize();
    shared_ptr<Logger> logger = Logger::getLogger();

    logger->info("+main()");

    MyEnclosingClass myClass1;
    auto serialized = myClass1.__nanoservices_serializer_serialize();

    MyEnclosingClass myClass2;
    myClass2.enclosingField.enclosedField = 512;
    myClass2.__nanoservices_serializer_deserialize(serialized);

    {
        stringstream msgSS;
        msgSS << "Deserialized: " << myClass2.enclosingField.enclosedField;
        logger->info(msgSS);
    }
    //
    //    {
    //        stringstream msgSS;
    //        auto serializedListData =
    //                static_pointer_cast<std::vector<std::shared_ptr<SerializationRecord>>>(serialized->at(0)->data);
    //
    //        for(auto iter = serializedListData->begin(); iter != serializedListData->end(); ++iter) {
    //            auto serializedListRec = *iter;
    //            auto serializedListRecData = static_pointer_cast<int32_t>(serializedListRec->data);
    //            msgSS << "Serialized list: " << *serializedListRecData << endl;
    //        }
    //        logger->info(msgSS);
    //    }

    logger->info("-main()");

    Logger::finalize();
    Configuration::finalize();

    return 0;
}
