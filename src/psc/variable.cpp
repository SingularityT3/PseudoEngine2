#include "psc/variable.h"

using namespace PSC;

Variable::Variable(const std::string &name, Variable *v)
    : data(v->data), reference(true), name(name), type(v->type), isConstant(v->isConstant)
{}

Variable::Variable(const std::string &name, PSC::DataType type, bool isConstant, const void *initialData)
    : reference(false), name(name), type(type), isConstant(isConstant)
{
    switch (type) {
        case PSC::DT_INTEGER:
            if (initialData == nullptr) data = new PSC::Integer();
            else data = new PSC::Integer(*((const PSC::Integer*) initialData));
            break;
        case PSC::DT_REAL:
            if (initialData == nullptr) data = new PSC::Real();
            else data = new PSC::Real(*((const PSC::Real*) initialData));
            break;
        case PSC::DT_BOOLEAN:
            if (initialData == nullptr) data = new PSC::Boolean();
            else data = new PSC::Boolean(*((const PSC::Boolean*) initialData));
            break;
        case PSC::DT_CHAR:
            if (initialData == nullptr) data = new PSC::Char();
            else data = new PSC::Char(*((const PSC::Char*) initialData));
            break;
        case PSC::DT_STRING:
            if (initialData == nullptr) data = new PSC::String();
            else data = new PSC::String(*((const PSC::String*) initialData));
            break;
        default:
            throw 0;
    }
}

Variable::~Variable() {
    if (!reference) delete data;
}

void Variable::set(Value *data) {
    delete data;
    this->data = data;
}

Variable *Variable::createReference(const std::string &refName) {
    return new Variable(refName, this);
}
