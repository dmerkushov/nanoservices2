#include "core/exception/NsException.h"
#include "plugins/configuration/configuration.h"
#include "plugins/logging/logging.h"
#include "plugins/serialization/serializer.h"
#include "thirdparty/swansontec/map-macro/map.h"

#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace nanoservices;

class MyClass {
public:
    vector<int32_t> myField3 = {234};

    NANOSERVICES2_MAKE_SERIALIZABLE(myField3)
};

#define LOGX(...) \
    { \
        stringstream msgSS_X; \
        msgSS_X << #__VA_ARGS__; \
        logger->info(msgSS_X); \
    }
int main(int argc, char **argv) {
    Configuration::initialize(argc, argv);
    Logger::initialize();
    shared_ptr<Logger> logger = Logger::getLogger();

    logger->info("+main()");

    MyClass myClass;
    auto serialized = myClass.__nanoservices_serialize();

    stringstream msgSS;
    auto serializedField3Data =
            static_pointer_cast<std::vector<std::shared_ptr<SerializerRecord>>>(serialized->at(0)->data);
    auto serializedField3Rec0 = serializedField3Data->at(0);
    auto serializedField3Rec0Data = static_pointer_cast<int32_t>(serializedField3Rec0->data);
    msgSS << "Ser: " << *serializedField3Rec0Data << endl;
    logger->info(msgSS);

    // stringstream msgSS0;
    // msgSS0 << "Serialized data 0: " << *(static_pointer_cast<int32_t>(serialized->at(0)->data));
    // logger->info(msgSS0);
    //
    // stringstream msgSS1;
    // msgSS1 << "Serialized data 1: " << *(static_pointer_cast<int32_t>(serialized->at(1)->data));
    // logger->info(msgSS1);
    //
    MyClass myClass2;
    myClass2.__nanoservices_deserialize(serialized);
    //
    // stringstream msgSS2;
    // msgSS2 << "Deserialized myField1: " << myClass2.myField1;
    // logger->info(msgSS2);
    //
    // stringstream msgSS3;
    // msgSS3 << "Deserialized myField2: " << myClass2.myField2;
    // logger->info(msgSS3);

    logger->info("-main()");

    Logger::finalize();
    Configuration::finalize();

    return 0;
}
