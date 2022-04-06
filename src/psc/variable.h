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

        Variable(const std::string &name, Variable *v);

    public:
        const std::string name;
        const PSC::DataType type;
        const bool isConstant;

        Variable(const std::string &name, PSC::DataType type, bool isConstant, const void *initialData = nullptr);

        ~Variable();

        void set(Value *data);

        template<std::derived_from<PSC::Value> T>
        T &get() { return *((T*) data); }

        Variable *createReference(const std::string &refName);
    };
}
