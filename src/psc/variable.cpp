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

Variable::Variable(const std::string &name, PSC::DataType type, bool isConstant, const Value *initialData)
    : DataHolder(name), reference(false), type(type), isConstant(isConstant)
{
    switch (type.type) {
        case PSC::DataType::INTEGER:
            if (initialData == nullptr) data = new PSC::Integer();
            else data = new PSC::Integer(*((const PSC::Integer*) initialData));
            break;
        case PSC::DataType::REAL:
            if (initialData == nullptr) data = new PSC::Real();
            else data = new PSC::Real(*((const PSC::Real*) initialData));
            break;
        case PSC::DataType::BOOLEAN:
            if (initialData == nullptr) data = new PSC::Boolean();
            else data = new PSC::Boolean(*((const PSC::Boolean*) initialData));
            break;
        case PSC::DataType::CHAR:
            if (initialData == nullptr) data = new PSC::Char();
            else data = new PSC::Char(*((const PSC::Char*) initialData));
            break;
        case PSC::DataType::STRING:
            if (initialData == nullptr) data = new PSC::String();
            else data = new PSC::String(*((const PSC::String*) initialData));
            break;
        case PSC::DataType::ENUM:
            if (initialData == nullptr) data = new PSC::Enum(*type.name);
            else data = new PSC::Enum(*((const PSC::Enum*) initialData));
            break;
        case PSC::DataType::POINTER:
            if (initialData == nullptr) data = new PSC::Pointer(*type.name);
            else data = new PSC::Pointer(*((const PSC::Pointer*) initialData));
            break;
        case PSC::DataType::COMPOSITE:
            if (initialData == nullptr) data = new PSC::Composite(*type.name);
            else data = new PSC::Composite(*((const PSC::Composite*) initialData));
            break;
        case PSC::DataType::NONE:
            std::abort();
    }
}

Variable::Variable(const Variable &other)
    : Variable(other.name, other.type, other.isConstant, other.data)
{}

Variable::~Variable() {
    if (!reference) delete data;
}

void Variable::set(Value *_data) {
    delete data;
    data = _data;
}

Variable *Variable::createReference(const std::string &refName) {
    return new Variable(refName, this);
}

Variable *Variable::createArrayElementReference(const std::string &refName, PSC::DataType type, Value *value) {
    return new Variable(refName, type, value);
}
