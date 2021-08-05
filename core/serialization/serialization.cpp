//
// Created by dmerkushov on 23.07.2021.
//

#include "serialization.h"

using namespace std;
using namespace nanoservices;

shared_ptr<string> nanoservices::Serializer::_mapItemKeyRecordName = make_shared<string>("nanoservices::Serializer::mapKey");
shared_ptr<string> nanoservices::Serializer::_mapItemValueRecordName = make_shared<string>("nanoservices::Serializer::mapValue");
shared_ptr<string> nanoservices::Serializer::_listItemRecordName = make_shared<string>("nanoservices::Serializer::listItem");
shared_ptr<Logger> nanoservices::Serializer::_logger = nullptr;

void nanoservices::Serializer::_initLogger() {
    if(!_logger) {
        _logger = Logger::getLogger();
    }
}

const char *nanoservices::getRecordTypeName(RecordType recordType) {
    switch(recordType) {
    case RecordType::NULL_VALUE:
        return "NULL_VALUE";
    case RecordType::BYTE_ARRAY:
        return "BYTE_ARRAY";
    case RecordType::STRING:
        return "STRING";
    case RecordType::SIGNED_INT_8:
        return "SIGNED_INT_8";
    case RecordType::UNSIGNED_INT_8:
        return "UNSIGNED_INT_8";
    case RecordType::SIGNED_INT_16:
        return "SIGNED_INT_16";
    case RecordType::UNSIGNED_INT_16:
        return "UNSIGNED_INT_16";
    case RecordType::SIGNED_INT_32:
        return "SIGNED_INT_32";
    case RecordType::UNSIGNED_INT_32:
        return "UNSIGNED_INT_32";
    case RecordType::SIGNED_INT_64:
        return "SIGNED_INT_64";
    case RecordType::UNSIGNED_INT_64:
        return "UNSIGNED_INT_64";
    case RecordType::FLOAT_32:
        return "FLOAT_32";
    case RecordType::FLOAT_64:
        return "FLOAT_64";
    case RecordType::LIST:
        return "LIST";
    case RecordType::MAP:
        return "MAP";
    default:
        return "UNKNOWN";
    }
}

std::ostream &nanoservices::operator<<(std::ostream &os, const RecordType &recordType) {
    os << getRecordTypeName(recordType);
    return os;
}
