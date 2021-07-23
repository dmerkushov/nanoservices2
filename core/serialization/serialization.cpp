//
// Created by dmerkushov on 23.07.2021.
//

#include "serialization.h"

using namespace std;
using namespace nanoservices;

shared_ptr<string> nanoservices::Serializer::_mapItemKeyRecordName = make_shared<string>("key");
shared_ptr<string> Serializer::_mapItemValueRecordName = make_shared<string>("value");
shared_ptr<string> Serializer::_listItemRecordName = make_shared<string>("");