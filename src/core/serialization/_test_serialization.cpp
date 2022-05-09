//
// Created by dmerkushov on 09.05.22.
//

#include "../../plugins/logging/logging.h"
#include "../../util/testing/testing.h"
#include "serialization.h"

using namespace std;
using namespace nanoservices;

template<typename T>
struct PrimitiveFields {
    T field;

    NANOSERVICES2_MAKE_SERIALIZABLE(field);
};

template<typename T>
bool check_primitiveField(T value) {
    PrimitiveFields<T> instance1;
    instance1.field = value;
    auto serialized = instance1.__nanoservices2_serializer_serialize();
    PrimitiveFields<T> instance2;
    instance2.__nanoservices2_serializer_deserialize(serialized);

    bool result = instance1.field == instance2.field;

    log::debug("Check for primitive type field, type {}: {}", typeid(value).name(), result ? "OK" : "FAILED");

    return result;
}

static const function<bool()> test_serialization_classesOfPrimitiveFields = []() {
    bool result = true;

    int8_t i8 = 1;
    result &= check_primitiveField(i8);

    int16_t i16 = 1;
    result &= check_primitiveField(i16);

    int32_t i32 = 1;
    result &= check_primitiveField(i32);

    int64_t i64 = 1;
    result &= check_primitiveField(i64);

    u_int8_t ui8 = 1;
    result &= check_primitiveField(ui8);

    u_int16_t ui16 = 1;
    result &= check_primitiveField(ui16);

    u_int32_t ui32 = 1;
    result &= check_primitiveField(ui32);

    u_int64_t ui64 = 1;
    result &= check_primitiveField(ui64);

    float f = 1.0;
    result &= check_primitiveField(f);

    double d = 1.0;
    result &= check_primitiveField(d);

    return result;
};

NSTEST_ADD(test_serialization_classesOfPrimitiveFields)

struct MyInnerClass {
    int32_t int32Field;
    std::map<int32_t, std::string> mapField;
    std::vector<double> vectorField;

    NANOSERVICES2_MAKE_SERIALIZABLE(int32Field, mapField, vectorField);
};

struct MyOuterClass {
    std::map<u_int8_t, MyInnerClass> enclosingField;

    NANOSERVICES2_MAKE_SERIALIZABLE(enclosingField);
};

static const function<bool()> test_serialization_mapsListsSerializableClasses = []() {
    bool result = true;

    MyOuterClass outer1;
    MyInnerClass inner1;
    outer1.enclosingField[15] = inner1;
    outer1.enclosingField[15].int32Field = 234;
    outer1.enclosingField[15].mapField[32] = "Hallo";
    outer1.enclosingField[15].vectorField = {0.0, 1.23};
    auto serialized = outer1.__nanoservices2_serializer_serialize();
    MyOuterClass outer2;
    outer2.__nanoservices2_serializer_deserialize(serialized);

    auto checkPredicate = [](bool predicate, const char *label) {
        if(predicate) {
            log::debug("OK: {}", label);
        } else {
            log::debug("FAILED: {}", label);
        }

        return predicate;
    };

    result &= checkPredicate(outer2.enclosingField.contains(15), "enclosingField.contains(15)");
    if(result) {
        result &= checkPredicate(outer2.enclosingField[15].int32Field == 234, "int32Field");
        result &= checkPredicate(outer2.enclosingField[15].mapField.contains(32), "mapField.contains(32)");
        if(result) {
            result &= checkPredicate(outer2.enclosingField[15].mapField[32].compare("Hallo") == 0, "mapField[32].compare(\"Hallo\")");
        }
        result &= checkPredicate(outer2.enclosingField[15].vectorField[0] == 0.0, "vectorField[0] == 0.0");
        result &= checkPredicate(outer2.enclosingField[15].vectorField[1] == 1.23, "vectorField[1] == 1.23");
    }

    return result;
};

NSTEST_ADD(test_serialization_mapsListsSerializableClasses)

static const function<bool()> test_serialization_get_record_type_name = []() {
    bool result = true;

    auto checkName = [](record_type recordType, const char *expected) {
        string actual = get_record_type_name(recordType);

        bool checkResult = actual.compare(expected) == 0;

        if(checkResult) {
            log::debug("OK: {}", actual);
        } else {
            log::debug("FAILED: expected: \"{}\", actual: \"{}\"", expected, actual);
        }

        return checkResult;
    };

    result &= checkName(record_type::NULL_VALUE, "NULL_VALUE");
    result &= checkName(record_type::BYTE_ARRAY, "BYTE_ARRAY");
    result &= checkName(record_type::STRING, "STRING");
    result &= checkName(record_type::SIGNED_INT_8, "SIGNED_INT_8");
    result &= checkName(record_type::UNSIGNED_INT_8, "UNSIGNED_INT_8");
    result &= checkName(record_type::SIGNED_INT_16, "SIGNED_INT_16");
    result &= checkName(record_type::UNSIGNED_INT_16, "UNSIGNED_INT_16");
    result &= checkName(record_type::SIGNED_INT_32, "SIGNED_INT_32");
    result &= checkName(record_type::UNSIGNED_INT_32, "UNSIGNED_INT_32");
    result &= checkName(record_type::SIGNED_INT_64, "SIGNED_INT_64");
    result &= checkName(record_type::UNSIGNED_INT_64, "UNSIGNED_INT_64");
    result &= checkName(record_type::FLOAT_32, "FLOAT_32");
    result &= checkName(record_type::FLOAT_64, "FLOAT_64");
    result &= checkName(record_type::LIST, "LIST");
    result &= checkName(record_type::MAP, "MAP");

    return result;
};

NSTEST_ADD(test_serialization_get_record_type_name)