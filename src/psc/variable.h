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
        Variable *ref;

        // Reference constructor
        Variable(const std::string &name, Variable *v);

    public:
        const DataType type;
        const bool isConstant;
        Context *const parent;

        Variable(const std::string &name, DataType type, bool isConstant, Context *ctx, const Value *initialData = nullptr);

        Variable(const Variable &other, Context *ctx);

        ~Variable();

        constexpr bool isArray() const override {return false;}

        void set(Value *_data, bool copy = false);

        template<std::derived_from<Value> T>
        T &get() { return *((T*) data); }

        template<std::derived_from<Value> T>
        T &getConst() const { return *((const T*) data); }

        Variable *createReference(const std::string &refName);

        Variable *getReference();
    };
}
