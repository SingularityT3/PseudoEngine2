#pragma once
#include <string>
#include <memory>
#include <concepts>
#include "psc/types/types.h"

namespace PSC {
    class DataHolder {
    public:
        const std::string name;

        DataHolder(const std::string &name);

        virtual ~DataHolder() = default;

        virtual bool isArray() const = 0;
    };

    class Variable : public DataHolder {
    private:
        Value *data;
        bool reference;

        // Reference constructor
        Variable(const std::string &name, Variable *v);

        // Array element reference constructor
        Variable(const std::string &name, PSC::DataType type, Value *data);

    public:
        const PSC::DataType type;
        const bool isConstant;

        Variable(const std::string &name, PSC::DataType type, bool isConstant, const Value *initialData = nullptr);

        ~Variable();

        constexpr bool isArray() const override {return false;}

        void set(Value *_data);

        template<std::derived_from<PSC::Value> T>
        T &get() { return *((T*) data); }

        template<std::derived_from<PSC::Value> T>
        T &getConst() const { return *((const T*) data); }

        Variable *createReference(const std::string &refName);

        static Variable *createArrayElementReference(const std::string &refName, PSC::DataType type, PSC::Value *value);
    };
}
