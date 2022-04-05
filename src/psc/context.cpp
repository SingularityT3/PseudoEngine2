#include "context.h"

using namespace PSC;

Context::Context(Context *parent, const std::string &name)
    : Context(parent, name, false, PSC::DT_NONE)
{}

Context::Context(Context *parent, const std::string &name, bool isFunctionCtx, PSC::DataType returnType)
    : parent(parent), name(name), isFunctionCtx(isFunctionCtx), returnType(returnType)
{}

Context::~Context() {
    for (Variable *v : variables) {
        delete v;
    }

    for (Procedure *p : procedures) {
        delete p;
    }

    for (Function *f : functions) {
        delete f;
    }
}

Context *Context::getParent() const {
    return parent;
}

const std::string &Context::getName() const {
    return name;
}

void Context::addVariable(Variable *variable) {
    variables.push_back(variable);
}

Variable *Context::getVariable(const std::string &varName) {
    for (Variable *var : variables) {
        if (var->name == varName) return var;
    }

    if (parent != nullptr) return parent->getVariable(varName);

    return nullptr;
}

void Context::addProcedure(Procedure *procedure) {
    procedures.push_back(procedure);
}

Procedure *Context::getProcedure(const std::string &procedureName) {
    for (Procedure *procedure : procedures) {
        if (procedureName == procedure->name) {
            return procedure;
        }
    }

    if (parent != nullptr) return parent->getProcedure(procedureName);

    return nullptr;
}

void Context::addFunction(Function *function) {
    functions.push_back(function);
}

Function *Context::getFunction(const std::string &functionName) {
    for (Function *function : functions) {
        if (functionName == function->name) {
            return function;
        }
    }

    if (parent != nullptr) return parent->getFunction(functionName);

    return nullptr;
}
