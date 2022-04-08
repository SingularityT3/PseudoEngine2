#include "psc/builtinFunctions/functions.h"
#include "context.h"

using namespace PSC;

Context::Context(Context *parent, const std::string &name)
    : Context(parent, name, false, PSC::DT_NONE)
{}

Context::Context(Context *parent, const std::string &name, bool isFunctionCtx, PSC::DataType returnType)
    : parent(parent), name(name), isFunctionCtx(isFunctionCtx), returnType(returnType)
{}

std::unique_ptr<Context> Context::createGlobalContext() {
    auto ctx = std::make_unique<Context>(nullptr, "Program");

    ctx->functions.reserve(15);

    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnLength>()));
    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnRight>()));
    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnMid>()));
    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnLeft>()));
    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnToUpper>()));
    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnToLower>()));

    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnNumToStr>()));
    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnStrToNum>()));
    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnIsNum>()));

    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnLCase>()));
    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnUCase>()));
    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnASC>()));
    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnCHR>()));

    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnRand>()));
    ctx->addFunction(std::move(std::make_unique<PSC::BuiltinFnInt>()));

    return ctx;
}

Context *Context::getParent() const {
    return parent;
}

const std::string &Context::getName() const {
    return name;
}

void Context::addVariable(Variable *variable) {
    variables.emplace_back(variable);
}

Variable *Context::getVariable(const std::string &varName) {
    for (auto &var : variables) {
        if (var->name == varName) return var.get();
    }

    if (parent != nullptr) return parent->getVariable(varName);

    return nullptr;
}

void Context::addProcedure(std::unique_ptr<Procedure> &&procedure) {
    procedures.emplace_back(std::move(procedure));
}

Procedure *Context::getProcedure(const std::string &procedureName) {
    for (auto &procedure : procedures) {
        if (procedureName == procedure->name) {
            return procedure.get();
        }
    }

    if (parent != nullptr) return parent->getProcedure(procedureName);

    return nullptr;
}

void Context::addFunction(std::unique_ptr<Function> &&function) {
    functions.emplace_back(std::move(function));
}

Function *Context::getFunction(const std::string &functionName) {
    for (auto &function : functions) {
        if (functionName == function->name) {
            return function.get();
        }
    }

    if (parent != nullptr) return parent->getFunction(functionName);

    return nullptr;
}
