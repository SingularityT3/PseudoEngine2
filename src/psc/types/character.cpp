#include "pch.h"

#include "psc/types/types.h"

using namespace PSC;

Char::Char(char value) : value(value) {}

Char::operator char() const {
    return value;
}

void Char::operator=(char x) {
    value = x;
}

void Char::operator=(const Char &x) {
    value = x.value;
}

std::unique_ptr<Integer> Char::toInteger() const {
    return std::make_unique<Integer>((int_t) value);
}

std::unique_ptr<Real> Char::toReal() const {
    return std::make_unique<Real>((real_t) value);
}

std::unique_ptr<Boolean> Char::toBoolean() const {
    return std::make_unique<Boolean>(value != 0);
}

std::unique_ptr<Char> Char::toChar() const {
    return std::make_unique<Char>(*this);
}

std::unique_ptr<String> Char::toString() const {
    std::string s;
    s += value;
    return std::make_unique<String>(s);
}

void Char::dump(std::ostream &out) const {
    out << "CHAR " << value;
    if (value == '\n') out << '#';
}

bool Char::load(std::istream &in, Context&) {
    std::string s;
    in >> s;
    if (s != "CHAR") return false;

    in.ignore(1); // whitespace
    value = in.get();

    return !in.fail();
}


String::String(const std::string &value) : value(value) {}

void String::operator=(const std::string &x) {
    value = x;
}

void String::operator=(const String &x) {
    value = x.value;
}

std::unique_ptr<String> String::operator&(const String &other) const {
    auto s = std::make_unique<String>(*this);
    s->value += other.value;
    return s;
}

std::unique_ptr<Integer> String::toInteger() const {
    const char *cStr = value.c_str();
    char *end;

    int_t x = std::strtol(cStr, &end, 10);
    if (end[0] != '\0' || cStr[0] == '\0') {
        x = 0;
    }

    return std::make_unique<Integer>(x);
}

std::unique_ptr<Real> String::toReal() const {
    const char *cStr = value.c_str();
    char *end;

    real_t x = std::strtod(cStr, &end);
    if (end[0] != '\0' || cStr[0] == '\0') {
        x = 0.0;
    }

    return std::make_unique<Real>(x);
}

std::unique_ptr<Boolean> String::toBoolean() const {
    return std::make_unique<Boolean>(value.size() > 0);
}

std::unique_ptr<Char> String::toChar() const {
    return std::make_unique<Char>((char) 0);
}

std::unique_ptr<String> String::toString() const {
    return std::make_unique<String>(*this);
}

void String::dump(std::ostream &out) const {
    std::string s;
    s.reserve(value.size());
    for (size_t i = 0; i < value.size(); i++) {
        s += value[i];
        if (value[i] == '\n') s += '#';
    }
    out << "STRING " << s.length() << " " << s;
}

bool String::load(std::istream &in, Context&) {
    std::string s;
    in >> s;
    if (s != "STRING") return false;

    in >> s;
    if (in.fail()) return false;
    size_t length = std::stoul(s, nullptr);

    char *buf = new char[length];
    in.ignore(1); // whitespace
    in.read(buf, length);
    value.clear();
    value.reserve(length);
    for (size_t i = 0; i < length; i++) {
        if (i > 0 && buf[i] == '#' && buf[i-1] == '\n') continue;
        value += buf[i];
    }
    delete[] buf;

    return true;
}
