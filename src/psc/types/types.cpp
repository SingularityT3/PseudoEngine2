#include "pch.h"

#include "psc/types/datatypes.h"

using namespace PSC;

DataType::DataType() : type(PSC::DataType::NONE) {}

DataType::DataType(Type type, const std::string *name)
    : name(name), type(type) {}

bool DataType::operator==(Type other) const {
    return type == other;
}

bool DataType::operator==(const DataType &other) const {
    if (name == nullptr || other.name == nullptr) return operator==(other.type);
    return type == other.type && *name == *other.name;
}
