#include "math.h"
#include "psc/types.h"

using namespace PSC;


Number::Number(bool real) : real(real)
{}

constexpr int mergeType(bool real1, bool real2) {
    return ((int) real1) << 1 | ((int) real2);
}

Number *Number::operator+(const Number &other) const {
    int type = mergeType(real, other.real);
    if (type == mergeType(false, false)) { // int, int
        int_t x = ((Integer*) this)->value + (Integer&) other;
        return new Integer(x);
    }
    
    else if (type == mergeType(false, true)) { // int, real
        real_t x = ((Integer*) this)->value + (Real&) other;
        return new Real(x);
    }
    
    else if (type == mergeType(true, false)) { // real, int
        real_t x = ((Real*) this)->value + (Integer&) other;
        return new Real(x);
    }
    
    else if (type == mergeType(true, true)) { // real, real
        real_t x = ((Real*) this)->value + (Real&) other;
        return new Real(x);
    }
    throw 0;
}

Number *Number::operator-(const Number &other) const {
    int type = mergeType(real, other.real);
    if (type == mergeType(false, false)) { // int, int
        int_t x = ((Integer*) this)->value - (Integer&) other;
        return new Integer(x);
    }
    
    else if (type == mergeType(false, true)) { // int, real
        real_t x = ((Integer*) this)->value - (Real&) other;
        return new Real(x);
    }
    
    else if (type == mergeType(true, false)) { // real, int
        real_t x = ((Real*) this)->value - (Integer&) other;
        return new Real(x);
    }
    
    else if (type == mergeType(true, true)) { // real, real
        real_t x = ((Real*) this)->value - (Real&) other;
        return new Real(x);
    }
    throw 0;
}

Number *Number::operator*(const Number &other) const {
    int type = mergeType(real, other.real);
    if (type == mergeType(false, false)) { // int, int
        int_t x = ((Integer*) this)->value * (Integer&) other;
        return new Integer(x);
    }
    
    else if (type == mergeType(false, true)) { // int, real
        real_t x = ((Integer*) this)->value * (Real&) other;
        return new Real(x);
    }
    
    else if (type == mergeType(true, false)) { // real, int
        real_t x = ((Real*) this)->value * (Integer&) other;
        return new Real(x);
    }
    
    else if (type == mergeType(true, true)) { // real, real
        real_t x = ((Real*) this)->value * (Real&) other;
        return new Real(x);
    }
    throw 0;
}

Number *Number::operator/(const Number &other) const {
    int type = mergeType(real, other.real);
    if (type == mergeType(false, false)) { // int, int
        real_t x = ((Integer*) this)->value / (real_t) (Integer&) other;
        return new Real(x);
    }
    
    else if (type == mergeType(false, true)) { // int, real
        real_t x = ((Integer*) this)->value / (Real&) other;
        return new Real(x);
    }
    
    else if (type == mergeType(true, false)) { // real, int
        real_t x = ((Real*) this)->value / (Integer&) other;
        return new Real(x);
    }
    
    else if (type == mergeType(true, true)) { // real, real
        real_t x = ((Real*) this)->value / (Real&) other;
        return new Real(x);
    }
    throw 0;
}

real_t modReal(real_t x, real_t y) {
    real_t z = x / y;
    z -= std::floor(z);
    return z * y;
}

Number *Number::operator%(const Number &other) const {
    int type = mergeType(real, other.real);
    if (type == mergeType(false, false)) { // int, int
        int_t x = ((Integer*) this)->value % (Integer&) other;
        return new Integer(x);
    }

    else if (type == mergeType(false, true)) { // int, real
        real_t x = modReal(((Integer*) this)->value, (Real&) other);
        return new Real(x);
    }

    else if (type == mergeType(true, false)) { // real, int
        real_t x = modReal(((Real*) this)->value, (Integer&) other);
        return new Real(x);
    }
    
    else if (type == mergeType(true, true)) { // real, real
        real_t x = modReal(((Real*) this)->value, (Real&) other);
        return new Real(x);
    }
    throw 0;
}

Number *Number::operator|(const Number &other) const {
    int type = mergeType(real, other.real);
    if (type == mergeType(false, false)) { // int, int
        int_t x = ((Integer*) this)->value / (Integer&) other;
        return new Integer(x);
    }
    
    else if (type == mergeType(false, true)) { // int, real
        int_t x = std::floor(((Integer*) this)->value / (Real&) other);
        return new Integer(x);
    }
    
    else if (type == mergeType(true, false)) { // real, int
        int_t x = std::floor(((Real*) this)->value / (Integer&) other);
        return new Integer(x);
    }
    
    else if (type == mergeType(true, true)) { // real, real
        int_t x = std::floor(((Real*) this)->value / (Real&) other);
        return new Integer(x);
    }
    throw 0;
}


Boolean *Number::operator==(const Number &other) const {
    int type = mergeType(real, other.real);

    if (type == mergeType(false, false)) {
        bool x = ((Integer*) this)->value == ((Integer&) other).value;
        return new Boolean(x);
    }

    else if (type == mergeType(false, true)) {
        bool x = ((Integer*) this)->value == ((Real&) other).value;
        return new Boolean(x);
    }

    else if (type == mergeType(true, false)) {
        bool x = ((Real*) this)->value == ((Integer&) other).value;
        return new Boolean(x);
    }

    else if (type == mergeType(true, true)) {
        bool x = ((Real*) this)->value == ((Real&) other).value;
        return new Boolean(x);
    }
    throw 0;
}

Boolean *Number::operator!=(const Number &other) const {
    int type = mergeType(real, other.real);

    if (type == mergeType(false, false)) {
        bool x = ((Integer*) this)->value != ((Integer&) other).value;
        return new Boolean(x);
    }

    else if (type == mergeType(false, true)) {
        bool x = ((Integer*) this)->value != ((Real&) other).value;
        return new Boolean(x);
    }

    else if (type == mergeType(true, false)) {
        bool x = ((Real*) this)->value != ((Integer&) other).value;
        return new Boolean(x);
    }

    else if (type == mergeType(true, true)) {
        bool x = ((Real*) this)->value != ((Real&) other).value;
        return new Boolean(x);
    }
    throw 0;
}

Boolean *Number::operator>(const Number &other) const {
    int type = mergeType(real, other.real);

    if (type == mergeType(false, false)) {
        bool x = ((Integer*) this)->value > (Integer&) other;
        return new Boolean(x);
    }

    else if (type == mergeType(false, true)) {
        bool x = ((Integer*) this)->value > (Real&) other;
        return new Boolean(x);
    }

    else if (type == mergeType(true, false)) {
        bool x = ((Real*) this)->value > (Integer&) other;
        return new Boolean(x);
    }

    else if (type == mergeType(true, true)) {
        bool x = ((Real*) this)->value > (Real&) other;
        return new Boolean(x);
    }
    throw 0;
}

Boolean *Number::operator<(const Number &other) const {
    int type = mergeType(real, other.real);

    if (type == mergeType(false, false)) {
        bool x = ((Integer*) this)->value < (Integer&) other;
        return new Boolean(x);
    }

    else if (type == mergeType(false, true)) {
        bool x = ((Integer*) this)->value < (Real&) other;
        return new Boolean(x);
    }

    else if (type == mergeType(true, false)) {
        bool x = ((Real*) this)->value < (Integer&) other;
        return new Boolean(x);
    }

    else if (type == mergeType(true, true)) {
        bool x = ((Real*) this)->value < (Real&) other;
        return new Boolean(x);
    }
    throw 0;
}

Boolean *Number::operator>=(const Number &other) const {
    int type = mergeType(real, other.real);

    if (type == mergeType(false, false)) {
        bool x = ((Integer*) this)->value >= (Integer&) other;
        return new Boolean(x);
    }

    else if (type == mergeType(false, true)) {
        bool x = ((Integer*) this)->value >= (Real&) other;
        return new Boolean(x);
    }

    else if (type == mergeType(true, false)) {
        bool x = ((Real*) this)->value >= (Integer&) other;
        return new Boolean(x);
    }

    else if (type == mergeType(true, true)) {
        bool x = ((Real*) this)->value >= (Real&) other;
        return new Boolean(x);
    }
    throw 0;
}

Boolean *Number::operator<=(const Number &other) const {
    int type = mergeType(real, other.real);

    if (type == mergeType(false, false)) {
        bool x = ((Integer*) this)->value <= (Integer&) other;
        return new Boolean(x);
    }

    else if (type == mergeType(false, true)) {
        bool x = ((Integer*) this)->value <= (Real&) other;
        return new Boolean(x);
    }

    else if (type == mergeType(true, false)) {
        bool x = ((Real*) this)->value <= (Integer&) other;
        return new Boolean(x);
    }

    else if (type == mergeType(true, true)) {
        bool x = ((Real*) this)->value <= (Real&) other;
        return new Boolean(x);
    }
    throw 0;
}


Integer::Integer()
    : Number(false)
{}

Integer::Integer(int_t value)
    : Number(false), value(value)
{}

Integer::operator int_t() const {
    return value;
}

void Integer::operator=(int_t x) {
    value = x;
}

void Integer::operator=(const Integer &x) {
    value = x.value;
}


Real::Real()
    : Number(true)
{}

Real::Real(real_t value)
    : Number(true), value(value)
{}

Real::operator real_t() const {
    return value;
}

void Real::operator=(real_t x) {
    value = x;
}

void Real::operator=(const Real &x) {
    value = x.value;
}


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

String::String(const std::string &value) : value(value) {}

void String::operator=(const std::string &x) {
    value = x;
}

void String::operator=(const String &x) {
    value = x.value;
}
