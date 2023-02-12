#include "pch.h"

#include "psc/builtinFunctions/functions.h"
#include "context.h"

using namespace PSC;

Context::Context(Context *parent, const std::string &name)
    : Context(parent, name, false, PSC::DataType::NONE)
{}

Context::Context(Context *parent, const std::string &name, bool isFunctionCtx, PSC::DataType returnType)
    : parent(parent), name(name), isFunctionCtx(isFunctionCtx), returnType(returnType)
{}

std::unique_ptr<Context> Context::createGlobalContext() {
    auto ctx = std::make_unique<Context>(nullptr, "Program");

    ctx->functions.reserve(28);

    ctx->addFunction(std::make_unique<PSC::BuiltinFnLength>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnRight>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnMid>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnLeft>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnToUpper>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnToLower>());

    ctx->addFunction(std::make_unique<PSC::BuiltinFnNumToStr>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnStrToNum>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnIsNum>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnEOF>());

    ctx->addFunction(std::make_unique<PSC::BuiltinFnLCase>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnUCase>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnASC>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnCHR>());

    ctx->addFunction(std::make_unique<PSC::BuiltinFnRand>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnInt>());

    ctx->addFunction(std::make_unique<PSC::BuiltinFnPow>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnExp>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnSin>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnCos>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnTan>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnASin>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnACos>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnATan>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnATan2>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnSqrt>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnLog>());
    ctx->addFunction(std::make_unique<PSC::BuiltinFnLn>());

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

    return nullptr;
}

void Context::addProcedure(std::unique_ptr<Procedure> &&procedure) {
    procedures.emplace_back(std::move(procedure));
}

Procedure *Context::getProcedure(const std::string &procedureName) {
    if (parent != nullptr) return parent->getProcedure(procedureName);

    for (auto &procedure : procedures) {
        if (procedureName == procedure->name) {
            return procedure.get();
        }
    }

    return nullptr;
}

void Context::addFunction(std::unique_ptr<Function> &&function) {
    functions.emplace_back(std::move(function));
}

Function *Context::getFunction(const std::string &functionName) {
    if (parent != nullptr) return parent->getFunction(functionName);

    for (auto &function : functions) {
        if (functionName == function->name) {
            return function.get();
        }
    }

    return nullptr;
}

void Context::addArray(std::unique_ptr<Array> &&array) {
    arrays.emplace_back(std::move(array));
}

Array *Context::getArray(const std::string &arrayName) {
    for (auto &array : arrays) {
        if (arrayName == array->name) {
            return array.get();
        }
    }

    return nullptr;
}
