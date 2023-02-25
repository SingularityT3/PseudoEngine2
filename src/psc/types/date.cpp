#include "pch.h"

#include "psc/types/types.h"

using namespace PSC;

Date::Date(std::chrono::year_month_day date)
    : date(date) {}

std::unique_ptr<Integer> Date::toInteger() const {
    unsigned int day = static_cast<unsigned int>(date.day());
    unsigned int month = static_cast<unsigned int>(date.month());
    int year = static_cast<int>(date.year());

    int_t cmp = year * 372 + month * 31 + day;
    return std::make_unique<Integer>(cmp);
}

std::unique_ptr<Real> Date::toReal() const {std::abort();}

std::unique_ptr<Boolean> Date::toBoolean() const {std::abort();}

std::unique_ptr<Char> Date::toChar() const {std::abort();}

std::unique_ptr<String> Date::toString() const {
    std::ostringstream ss;
    unsigned int day = static_cast<unsigned int>(date.day());
    unsigned int month = static_cast<unsigned int>(date.month());
    int year = static_cast<int>(date.year());
    ss << day << "/" << month << "/" << year;
    return std::make_unique<String>(ss.str());
}
