#pragma once
#include <string>
#include <memory>

namespace PSC {
    typedef long int_t;
    typedef double real_t;

    enum class DataType {
        NONE, INTEGER, REAL, BOOLEAN, CHAR, STRING
    };

    class Integer;
    class Real;
    class Boolean;
    class Char;
    class String;

    class Value {
    public:
        virtual ~Value() = default;

        virtual std::unique_ptr<Integer> toInteger() const = 0;

        virtual std::unique_ptr<Real> toReal() const = 0;

        virtual std::unique_ptr<Boolean> toBoolean() const = 0;

        virtual std::unique_ptr<Char> toChar() const = 0;

        virtual std::unique_ptr<String> toString() const = 0;
    };

    class Number : public Value {
    public:
        const bool real;

        Number(bool real);

        std::unique_ptr<Number> operator+(const Number &other) const;

        std::unique_ptr<Number> operator-(const Number &other) const;

        std::unique_ptr<Number> operator*(const Number &other) const;

        std::unique_ptr<Number> operator/(const Number &other) const;

        std::unique_ptr<Number> operator%(const Number &other) const;

        std::unique_ptr<Number> operator|(const Number &other) const; // Integer division


        std::unique_ptr<Boolean> operator==(const Number &other) const;

        std::unique_ptr<Boolean> operator!=(const Number &other) const;

        std::unique_ptr<Boolean> operator>(const Number &other) const;

        std::unique_ptr<Boolean> operator<(const Number &other) const;

        std::unique_ptr<Boolean> operator>=(const Number &other) const;

        std::unique_ptr<Boolean> operator<=(const Number &other) const;
    };

    class Integer : public Number {
    public:
        int_t value = 0;

        Integer();

        Integer(int_t value);

        Integer(const PSC::Integer&) = default;

        operator int_t() const;

        void operator=(int_t x);

        void operator=(const Integer &x);

        std::unique_ptr<Integer> toInteger() const override;

        std::unique_ptr<Real> toReal() const override;

        std::unique_ptr<Boolean> toBoolean() const override;

        std::unique_ptr<Char> toChar() const override;

        std::unique_ptr<String> toString() const override;
    };

    class Real : public Number {
    public:
        real_t value = 0.0f;

        Real();

        Real(real_t value);

        Real(const Real&) = default;

        operator real_t() const;

        void operator=(real_t x);

        void operator=(const Real& x);

        std::unique_ptr<Integer> toInteger() const override;

        std::unique_ptr<Real> toReal() const override;

        std::unique_ptr<Boolean> toBoolean() const override;

        std::unique_ptr<Char> toChar() const override;

        std::unique_ptr<String> toString() const override;
    };

    class Boolean : public Value {
    public:
        bool value = false;

        Boolean() = default;

        Boolean(bool value);

        Boolean(const Boolean&) = default;

        operator bool() const;

        void operator=(bool x);

        void operator=(const Boolean &x);

        std::unique_ptr<Integer> toInteger() const override;

        std::unique_ptr<Real> toReal() const override;

        std::unique_ptr<Boolean> toBoolean() const override;

        std::unique_ptr<Char> toChar() const override;

        std::unique_ptr<String> toString() const override;
    };

    class Char : public Value {
    public:
        char value = 0;

        Char() = default;

        Char(char value);

        Char(const Char&) = default;

        operator char() const;

        void operator=(char x);

        void operator=(const Char &x);

        std::unique_ptr<Integer> toInteger() const override;

        std::unique_ptr<Real> toReal() const override;

        std::unique_ptr<Boolean> toBoolean() const override;

        std::unique_ptr<Char> toChar() const override;

        std::unique_ptr<String> toString() const override;
    };

    class String : public Value {
    public:
        std::string value;

        String() = default;

        String(const std::string &value);

        String(const String&) = default;

        void operator=(const std::string &x);

        void operator=(const String &x);

        std::unique_ptr<String> operator&(const String &other) const;

        std::unique_ptr<Integer> toInteger() const override;

        std::unique_ptr<Real> toReal() const override;

        std::unique_ptr<Boolean> toBoolean() const override;

        std::unique_ptr<Char> toChar() const override;

        std::unique_ptr<String> toString() const override;
    };
}
