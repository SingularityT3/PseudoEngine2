#include "pch.h"

#include "psc/variable.h"

using namespace PSC;

DataHolder::DataHolder(const std::string &name) : name(name) {}

Variable::Variable(const std::string &name, Variable *v)
    : DataHolder(name), data(v->data), reference(true), type(v->type), isConstant(v->isConstant)
{}

Variable::Variable(const std::string &name, PSC::DataType type, Value *data)
    : DataHolder(name), data(data), reference(true), type(type), isConstant(false)
{}

Variable::Variable(const std::string &name, PSC::DataType type, bool isConstant, Context *ctx, const Value *initialData)
    : DataHolder(name), data(nullptr), reference(false), type(type), isConstant(isConstant)
{
    if (initialData != nullptr) {
        set(const_cast<Value*>(initialData), true);
        return;
    }
    switch (type.type) {
        case PSC::DataType::INTEGER:
            data = new PSC::Integer();
            break;
        case PSC::DataType::REAL:
            data = new PSC::Real();
            break;
        case PSC::DataType::BOOLEAN:
            data = new PSC::Boolean();
            break;
        case PSC::DataType::CHAR:
            data = new PSC::Char();
            break;
        case PSC::DataType::STRING:
            data = new PSC::String();
            break;
        case PSC::DataType::ENUM:
            data = new PSC::Enum(*type.name);
            break;
        case PSC::DataType::POINTER:
            data = new PSC::Pointer(*type.name);
            break;
        case PSC::DataType::COMPOSITE:
            data = new PSC::Composite(*type.name, *ctx);
            break;
        case PSC::DataType::NONE:
            std::abort();
    }
}

Variable::Variable(const Variable &other)
    : Variable(other.name, other.type, other.isConstant, nullptr, other.data)
{}

Variable::~Variable() {
    if (!reference) delete data;
}

void Variable::set(Value *_data, bool copy) {
    if (data != nullptr) delete data;
    if (!copy) {
        data = _data;
        return;
    }

    switch (type.type) {
        case PSC::DataType::INTEGER:
            data = new PSC::Integer(*((const PSC::Integer*) _data));
            break;
        case PSC::DataType::REAL:
            data = new PSC::Real(*((const PSC::Real*) _data));
            break;
        case PSC::DataType::BOOLEAN:
            data = new PSC::Boolean(*((const PSC::Boolean*) _data));
            break;
        case PSC::DataType::CHAR:
            data = new PSC::Char(*((const PSC::Char*) _data));
            break;
        case PSC::DataType::STRING:
            data = new PSC::String(*((const PSC::String*) _data));
            break;
        case PSC::DataType::ENUM:
            data = new PSC::Enum(*((const PSC::Enum*) _data));
            break;
        case PSC::DataType::POINTER:
            data = new PSC::Pointer(*((const PSC::Pointer*) _data));
            break;
        case PSC::DataType::COMPOSITE:
            data = new PSC::Composite(*((const PSC::Composite*) _data));
            break;
        case PSC::DataType::NONE:
            std::abort();
    }
}

Variable *Variable::createReference(const std::string &refName) {
    return new Variable(refName, this);
}

Variable *Variable::createArrayElementReference(const std::string &refName, PSC::DataType type, Value *value) {
    return new Variable(refName, type, value);
}
