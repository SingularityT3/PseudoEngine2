#include <math.h>
#include "psc/types/types.h"

using namespace PSC;

Number::Number(bool real) : real(real)
{}

constexpr int mergeType(bool real1, bool real2) {
    return ((int) real1) << 1 | ((int) real2);
}

std::unique_ptr<Number> Number::operator+(const Number &other) const {
    int type = mergeType(real, other.real);
    if (type == mergeType(false, false)) { // int, int
        int_t x = ((Integer*) this)->value + (Integer&) other;
        return std::make_unique<Integer>(x);
    }
    
    else if (type == mergeType(false, true)) { // int, real
        real_t x = ((Integer*) this)->value + (Real&) other;
        return std::make_unique<Real>(x);
    }
    
    else if (type == mergeType(true, false)) { // real, int
        real_t x = ((Real*) this)->value + (Integer&) other;
        return std::make_unique<Real>(x);
    }
    
    else if (type == mergeType(true, true)) { // real, real
        real_t x = ((Real*) this)->value + (Real&) other;
        return std::make_unique<Real>(x);
    }
    std::abort();
}

std::unique_ptr<Number> Number::operator-(const Number &other) const {
    int type = mergeType(real, other.real);
    if (type == mergeType(false, false)) { // int, int
        int_t x = ((Integer*) this)->value - (Integer&) other;
        return std::make_unique<Integer>(x);
    }
    
    else if (type == mergeType(false, true)) { // int, real
        real_t x = ((Integer*) this)->value - (Real&) other;
        return std::make_unique<Real>(x);
    }
    
    else if (type == mergeType(true, false)) { // real, int
        real_t x = ((Real*) this)->value - (Integer&) other;
        return std::make_unique<Real>(x);
    }
    
    else if (type == mergeType(true, true)) { // real, real
        real_t x = ((Real*) this)->value - (Real&) other;
        return std::make_unique<Real>(x);
    }
    std::abort();
}

std::unique_ptr<Number> Number::operator*(const Number &other) const {
    int type = mergeType(real, other.real);
    if (type == mergeType(false, false)) { // int, int
        int_t x = ((Integer*) this)->value * (Integer&) other;
        return std::make_unique<Integer>(x);
    }
    
    else if (type == mergeType(false, true)) { // int, real
        real_t x = ((Integer*) this)->value * (Real&) other;
        return std::make_unique<Real>(x);
    }
    
    else if (type == mergeType(true, false)) { // real, int
        real_t x = ((Real*) this)->value * (Integer&) other;
        return std::make_unique<Real>(x);
    }
    
    else if (type == mergeType(true, true)) { // real, real
        real_t x = ((Real*) this)->value * (Real&) other;
        return std::make_unique<Real>(x);
    }
    std::abort();
}

std::unique_ptr<Number> Number::operator/(const Number &other) const {
    int type = mergeType(real, other.real);
    if (type == mergeType(false, false)) { // int, int
        real_t x = ((Integer*) this)->value / (real_t) (Integer&) other;
        return std::make_unique<Real>(x);
    }
    
    else if (type == mergeType(false, true)) { // int, real
        real_t x = ((Integer*) this)->value / (Real&) other;
        return std::make_unique<Real>(x);
    }
    
    else if (type == mergeType(true, false)) { // real, int
        real_t x = ((Real*) this)->value / (Integer&) other;
        return std::make_unique<Real>(x);
    }
    
    else if (type == mergeType(true, true)) { // real, real
        real_t x = ((Real*) this)->value / (Real&) other;
        return std::make_unique<Real>(x);
    }
    std::abort();
}

real_t modReal(real_t x, real_t y) {
    real_t z = x / y;
    z -= floor(z);
    return z * y;
}

std::unique_ptr<Number> Number::operator%(const Number &other) const {
    int type = mergeType(real, other.real);
    if (type == mergeType(false, false)) { // int, int
        int_t x = ((Integer*) this)->value % (Integer&) other;
        return std::make_unique<Integer>(x);
    }

    else if (type == mergeType(false, true)) { // int, real
        real_t x = modReal(((Integer*) this)->value, (Real&) other);
        return std::make_unique<Real>(x);
    }

    else if (type == mergeType(true, false)) { // real, int
        real_t x = modReal(((Real*) this)->value, (Integer&) other);
        return std::make_unique<Real>(x);
    }
    
    else if (type == mergeType(true, true)) { // real, real
        real_t x = modReal(((Real*) this)->value, (Real&) other);
        return std::make_unique<Real>(x);
    }
    std::abort();
}

std::unique_ptr<Number> Number::operator|(const Number &other) const {
    int type = mergeType(real, other.real);
    if (type == mergeType(false, false)) { // int, int
        int_t x = ((Integer*) this)->value / (Integer&) other;
        return std::make_unique<Integer>(x);
    }
    
    else if (type == mergeType(false, true)) { // int, real
        int_t x = (int_t) floor(((Integer*) this)->value / (Real&) other);
        return std::make_unique<Integer>(x);
    }
    
    else if (type == mergeType(true, false)) { // real, int
        int_t x = (int_t) floor(((Real*) this)->value / (Integer&) other);
        return std::make_unique<Integer>(x);
    }
    
    else if (type == mergeType(true, true)) { // real, real
        int_t x = (int_t) floor(((Real*) this)->value / (Real&) other);
        return std::make_unique<Integer>(x);
    }
    std::abort();
}


std::unique_ptr<Boolean> Number::operator==(const Number &other) const {
    int type = mergeType(real, other.real);

    if (type == mergeType(false, false)) {
        bool x = ((Integer*) this)->value == ((Integer&) other).value;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(false, true)) {
        bool x = ((Integer*) this)->value == ((Real&) other).value;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(true, false)) {
        bool x = ((Real*) this)->value == ((Integer&) other).value;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(true, true)) {
        bool x = ((Real*) this)->value == ((Real&) other).value;
        return std::make_unique<Boolean>(x);
    }
    std::abort();
}

std::unique_ptr<Boolean> Number::operator!=(const Number &other) const {
    int type = mergeType(real, other.real);

    if (type == mergeType(false, false)) {
        bool x = ((Integer*) this)->value != ((Integer&) other).value;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(false, true)) {
        bool x = ((Integer*) this)->value != ((Real&) other).value;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(true, false)) {
        bool x = ((Real*) this)->value != ((Integer&) other).value;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(true, true)) {
        bool x = ((Real*) this)->value != ((Real&) other).value;
        return std::make_unique<Boolean>(x);
    }
    std::abort();
}

std::unique_ptr<Boolean> Number::operator>(const Number &other) const {
    int type = mergeType(real, other.real);

    if (type == mergeType(false, false)) {
        bool x = ((Integer*) this)->value > (Integer&) other;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(false, true)) {
        bool x = ((Integer*) this)->value > (Real&) other;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(true, false)) {
        bool x = ((Real*) this)->value > (Integer&) other;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(true, true)) {
        bool x = ((Real*) this)->value > (Real&) other;
        return std::make_unique<Boolean>(x);
    }
    std::abort();
}

std::unique_ptr<Boolean> Number::operator<(const Number &other) const {
    int type = mergeType(real, other.real);

    if (type == mergeType(false, false)) {
        bool x = ((Integer*) this)->value < (Integer&) other;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(false, true)) {
        bool x = ((Integer*) this)->value < (Real&) other;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(true, false)) {
        bool x = ((Real*) this)->value < (Integer&) other;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(true, true)) {
        bool x = ((Real*) this)->value < (Real&) other;
        return std::make_unique<Boolean>(x);
    }
    std::abort();
}

std::unique_ptr<Boolean> Number::operator>=(const Number &other) const {
    int type = mergeType(real, other.real);

    if (type == mergeType(false, false)) {
        bool x = ((Integer*) this)->value >= (Integer&) other;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(false, true)) {
        bool x = ((Integer*) this)->value >= (Real&) other;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(true, false)) {
        bool x = ((Real*) this)->value >= (Integer&) other;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(true, true)) {
        bool x = ((Real*) this)->value >= (Real&) other;
        return std::make_unique<Boolean>(x);
    }
    std::abort();
}

std::unique_ptr<Boolean> Number::operator<=(const Number &other) const {
    int type = mergeType(real, other.real);

    if (type == mergeType(false, false)) {
        bool x = ((Integer*) this)->value <= (Integer&) other;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(false, true)) {
        bool x = ((Integer*) this)->value <= (Real&) other;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(true, false)) {
        bool x = ((Real*) this)->value <= (Integer&) other;
        return std::make_unique<Boolean>(x);
    }

    else if (type == mergeType(true, true)) {
        bool x = ((Real*) this)->value <= (Real&) other;
        return std::make_unique<Boolean>(x);
    }
    std::abort();
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

std::unique_ptr<Integer> Integer::toInteger() const {
    return std::make_unique<Integer>(*this);
}

std::unique_ptr<Real> Integer::toReal() const {
    return std::make_unique<Real>((real_t) value);
}

std::unique_ptr<Boolean> Integer::toBoolean() const {
    return std::make_unique<Boolean>(value != 0);
}

std::unique_ptr<Char> Integer::toChar() const {
    return std::make_unique<Char>((char) value);
}

std::unique_ptr<String> Integer::toString() const {
    return std::make_unique<String>(std::to_string(value));
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

std::unique_ptr<Integer> Real::toInteger() const {
    return std::make_unique<Integer>((int_t) value);
}

std::unique_ptr<Real> Real::toReal() const {
    return std::make_unique<Real>(*this);
}

std::unique_ptr<Boolean> Real::toBoolean() const {
    return std::make_unique<Boolean>(value != 0);
}

std::unique_ptr<Char> Real::toChar() const {
    return std::make_unique<Char>((char) value);
}

std::unique_ptr<String> Real::toString() const {
    std::string s = std::to_string(value);
    s.erase(s.find_last_not_of('0') + 1);
    if (s.back() == '.') s.erase(s.size() - 1);
    return std::make_unique<String>(s);
}
