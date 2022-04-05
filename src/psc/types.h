#pragma once
#include <string>

namespace PSC {
    typedef long int_t;
    typedef double real_t;

    enum DataType {
        DT_NONE, DT_INTEGER, DT_REAL, DT_BOOLEAN, DT_CHAR, DT_STRING
    };

    class String;
    class Boolean;

    class Value {
    public:
        virtual ~Value() = default;
    };

    class Number : public Value {
    public:
        const bool real;

        Number(bool real);

        Number *operator+(const Number &other) const;

        Number *operator-(const Number &other) const;

        Number *operator*(const Number &other) const;

        Number *operator/(const Number &other) const;

        Number *operator%(const Number &other) const;

        Number *operator|(const Number &other) const; // Integer division


        Boolean *operator==(const Number &other) const;

        Boolean *operator!=(const Number &other) const;

        Boolean *operator>(const Number &other) const;

        Boolean *operator<(const Number &other) const;

        Boolean *operator>=(const Number &other) const;

        Boolean *operator<=(const Number &other) const;
    };

    class Integer : public Number {
    public:
        int_t value = 0;

        Integer();

        Integer(int_t value);

        operator int_t() const;

        void operator=(int_t x);

        void operator=(const Integer &x);
    };

    class Real : public Number {
    public:
        real_t value = 0.0f;

        Real();

        Real(real_t value);

        operator real_t() const;

        void operator=(real_t x);

        void operator=(const Real& x);
    };

    class Boolean : public Value {
    public:
        bool value = false;

        Boolean() = default;

        Boolean(bool value);

        operator bool() const;

        void operator=(bool x);

        void operator=(const Boolean &x);
    };

    class Char : public Value {
    public:
        char value = 0;

        Char() = default;

        Char(char value);

        operator char() const;

        void operator=(char x);

        void operator=(const Char &x);
    };

    class String : public Value {
    public:
        std::string value;

        String() = default;

        String(const std::string &value);

        void operator=(const std::string &x);

        void operator=(const String &x);
    };
}
