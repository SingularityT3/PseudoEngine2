#pragma once
#include <string>
#include <memory>
#include <concepts>
#include "psc/types/types.h"

namespace PSC {
    class Variable {
    private:
        Value *data;
        bool reference;

        // Reference constructor
        Variable(const std::string &name, Variable *v);

        // Array element reference constructor
        Variable(const std::string &name, PSC::DataType type, Value *data);

    public:
        const std::string name;
        const PSC::DataType type;
        const bool isConstant;

        Variable(const std::string &name, PSC::DataType type, bool isConstant, const Value *initialData = nullptr);

        ~Variable();

        void set(Value *data);

        template<std::derived_from<PSC::Value> T>
        T &get() { return *((T*) data); }

        Variable *createReference(const std::string &refName);

        static Variable *createArrayElementReference(const std::string &refName, PSC::DataType type, PSC::Value *value);
    };
}
