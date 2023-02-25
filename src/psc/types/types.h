#pragma once
#include <string>
#include <memory>
#include <chrono>

#include "psc/types/datatypes.h"
#include "psc/types/type_definitions.h"

namespace PSC {
    typedef long int_t;
    typedef double real_t;

    std::ostream &operator<<(std::ostream &os, DataType &type);

    class Integer;
    class Real;
    class Boolean;
    class Char;
    class String;

    class Context;
    class DataHolder;
    class Variable;

    class Value {
    public:
        virtual ~Value() = default;

        virtual bool isPrimitive() const = 0;
    };

    class Primitive : public Value {
    public:
        virtual std::unique_ptr<Integer> toInteger() const = 0;

        virtual std::unique_ptr<Real> toReal() const = 0;

        virtual std::unique_ptr<Boolean> toBoolean() const = 0;

        virtual std::unique_ptr<Char> toChar() const = 0;

        virtual std::unique_ptr<String> toString() const = 0;

        constexpr bool isPrimitive() const override {return true;}
    };

    class Number : public Primitive {
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

    class Boolean : public Primitive {
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

    class Char : public Primitive {
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

    class String : public Primitive {
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

    class Date : public Primitive {
    public:
        std::chrono::year_month_day date;

        Date() = default;

        Date(std::chrono::year_month_day date);

        Date(const Date&) = default;

        std::unique_ptr<Integer> toInteger() const override;

        std::unique_ptr<Real> toReal() const override;

        std::unique_ptr<Boolean> toBoolean() const override;

        std::unique_ptr<Char> toChar() const override;

        std::unique_ptr<String> toString() const override;
    };


    class Custom : public Value {
    public:
        constexpr bool isPrimitive() const override {return false;}
    };

    class Enum : public Custom {
    public:
        const std::string definitionName;
        const std::string *value;

        Enum(const std::string &name);

        Enum(const Enum &other) = default;

        void operator=(const Enum &other);

        const EnumTypeDefinition &getDefinition(Context &ctx) const;
    };

    class Pointer : public Custom {
    private:
        Variable *ptr = nullptr;
        Context *varCtx = nullptr;

    public:
        const std::string definitionName;

        Pointer(const std::string &name);

        Pointer(const Pointer &other);

        void setValue(Variable *value);

        void operator=(const Pointer &other);

        Variable *getValue() const;

        const Context *getCtx() const;

        const PointerTypeDefinition &getDefinition(Context &ctx) const;
    };

    class Composite : public Custom {
    public:
        const std::string definitionName;
        std::unique_ptr<Context> ctx;

        Composite(const std::string &name, Context &parent);

        Composite(const Composite &other);

        void operator=(const Composite &other);

        DataHolder *getMember(const std::string &name);

        const CompositeTypeDefinition &getDefinition(Context &ctx) const;
    };
}
