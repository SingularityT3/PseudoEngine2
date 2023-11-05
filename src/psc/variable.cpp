#include "pch.h"

#include "psc/variable.h"

using namespace PSC;

DataHolder::DataHolder(const std::string &name) : name(name) {}

Variable::Variable(const std::string &name, Variable *v)
    : DataHolder(name), data(v->data), ref(v), type(v->type), isConstant(v->isConstant), parent(v->parent)
{}

Variable::Variable(const std::string &name, DataType type, bool isConstant, Context *ctx, const Value *initialData)
    : DataHolder(name), data(nullptr), ref(nullptr), type(type), isConstant(isConstant), parent(ctx)
{
    if (initialData != nullptr) {
        set(const_cast<Value*>(initialData), true);
        return;
    }
    switch (type.type) {
        case DataType::INTEGER:
            data = new PSC::Integer();
            break;
        case DataType::REAL:
            data = new PSC::Real();
            break;
        case DataType::BOOLEAN:
            data = new PSC::Boolean();
            break;
        case DataType::CHAR:
            data = new PSC::Char();
            break;
        case DataType::STRING:
            data = new PSC::String();
            break;
        case DataType::DATE:
            data = new PSC::Date();
            break;
        case DataType::ENUM:
            data = new PSC::Enum(*type.name);
            break;
        case DataType::POINTER:
            data = new PSC::Pointer(*type.name);
            break;
        case DataType::COMPOSITE:
            data = new PSC::Composite(*type.name, *ctx);
            break;
        case DataType::NONE:
            std::abort();
    }
}

Variable::Variable(const Variable &other, Context *ctx)
    : Variable(other.name, other.type, other.isConstant, ctx, other.data)
{}

Variable::~Variable() {
    if (ref == nullptr) delete data;
}

void Variable::set(Value *_data, bool copy) {
    if (data != nullptr) delete data;
    if (!copy) {
        data = _data;
        return;
    }

    switch (type.type) {
        case DataType::INTEGER:
            data = new PSC::Integer(*((const PSC::Integer*) _data));
            break;
        case DataType::REAL:
            data = new PSC::Real(*((const PSC::Real*) _data));
            break;
        case DataType::BOOLEAN:
            data = new PSC::Boolean(*((const PSC::Boolean*) _data));
            break;
        case DataType::CHAR:
            data = new PSC::Char(*((const PSC::Char*) _data));
            break;
        case DataType::STRING:
            data = new PSC::String(*((const PSC::String*) _data));
            break;
        case DataType::DATE:
            data = new PSC::Date(*((const PSC::Date*) _data));
            break;
        case DataType::ENUM:
            data = new PSC::Enum(*((const PSC::Enum*) _data));
            break;
        case DataType::POINTER:
            data = new PSC::Pointer(*((const PSC::Pointer*) _data));
            break;
        case DataType::COMPOSITE:
            data = new PSC::Composite(*((const PSC::Composite*) _data));
            break;
        case DataType::NONE:
            std::abort();
    }
}

Variable *Variable::createReference(const std::string &refName) {
    Variable *v = ref == nullptr ? this : ref;
    return new Variable(refName, v);
}

Variable *Variable::getReference() {
    return ref;
}

void Variable::dump(std::ostream &out) const {
    data->dump(out);
}

bool Variable::load(std::istream &in, Context &ctx) {
    return data->load(in, ctx);
}
