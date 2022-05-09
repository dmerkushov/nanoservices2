//
// Created by dmerkushov on 23.07.2021.
//

#include "serialization.h"

using namespace std;
using namespace nanoservices;

shared_ptr<string> nanoservices::serializer::_map_item_key_record_name = make_shared<string>("nanoservices::serializer::mapKey");
shared_ptr<string> nanoservices::serializer::_map_item_value_record_name = make_shared<string>("nanoservices::serializer::mapValue");
shared_ptr<string> nanoservices::serializer::_list_item_record_name = make_shared<string>("nanoservices::serializer::listItem");

const char *nanoservices::get_record_type_name(record_type recordType) {
    switch(recordType) {
    case record_type::NULL_VALUE:
        return "NULL_VALUE";
    case record_type::BYTE_ARRAY:
        return "BYTE_ARRAY";
    case record_type::STRING:
        return "STRING";
    case record_type::SIGNED_INT_8:
        return "SIGNED_INT_8";
    case record_type::UNSIGNED_INT_8:
        return "UNSIGNED_INT_8";
    case record_type::SIGNED_INT_16:
        return "SIGNED_INT_16";
    case record_type::UNSIGNED_INT_16:
        return "UNSIGNED_INT_16";
    case record_type::SIGNED_INT_32:
        return "SIGNED_INT_32";
    case record_type::UNSIGNED_INT_32:
        return "UNSIGNED_INT_32";
    case record_type::SIGNED_INT_64:
        return "SIGNED_INT_64";
    case record_type::UNSIGNED_INT_64:
        return "UNSIGNED_INT_64";
    case record_type::FLOAT_32:
        return "FLOAT_32";
    case record_type::FLOAT_64:
        return "FLOAT_64";
    case record_type::LIST:
        return "LIST";
    case record_type::MAP:
        return "MAP";
    default:
        return "UNKNOWN";
    }
}
