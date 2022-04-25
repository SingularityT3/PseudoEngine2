#include "pch.h"

#include "psc/types/types.h"

using namespace PSC;

Boolean::Boolean(bool value)
    : value(value)
{}

Boolean::operator bool() const {
    return value;
}

void Boolean::operator=(bool x) {
    value = x;
}

void Boolean::operator=(const Boolean &x) {
    value = x.value;
}

std::unique_ptr<Integer> Boolean::toInteger() const {
    return std::make_unique<Integer>((int_t) value);
}

std::unique_ptr<Real> Boolean::toReal() const {
    return std::make_unique<Real>((real_t) value);
}

std::unique_ptr<Boolean> Boolean::toBoolean() const {
    return std::make_unique<Boolean>(*this);
}

std::unique_ptr<Char> Boolean::toChar() const {
    return std::make_unique<Char>((char) value);
}

std::unique_ptr<String> Boolean::toString() const {
    std::string s = value ? "TRUE" : "FALSE";
    return std::make_unique<String>(s);
}
