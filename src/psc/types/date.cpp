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

void Date::dump(std::ostream &out) const {
    unsigned int day = static_cast<unsigned int>(date.day());
    unsigned int month = static_cast<unsigned int>(date.month());
    int year = static_cast<int>(date.year());
    out << "DATE " << day << " " << month << " " << year;
}

bool Date::load(std::istream &in, Context&) {
    std::string s;
    in >> s;
    if (s != "DATE") return false;
    
    unsigned int day = 0;
    unsigned int month = 0;
    int year = 0;

    in >> day;
    in >> month;
    in >> year;
    if (in.fail()) return false;

    date = std::chrono::year_month_day(
        std::chrono::year(year),
        std::chrono::month(month),
        std::chrono::day(day)
    );

    return true;
}
