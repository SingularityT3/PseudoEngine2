#include "pch.h"

#include "psc/scope/block.h"
#include "psc/scope/context.h"
#include "procedure.h"

using namespace PSC;

Parameter::Parameter(const std::string &name, PSC::DataType type)
    : name(name), type(type)
{}

Procedure::Procedure(
    const std::string &name,
    std::vector<Parameter> &&parameters,
    bool byRef,
    PSC::Block *block
)
: name(name),
    parameters(std::move(parameters)),
    byRef(byRef),
    block(block)
{}

Procedure::Procedure(const std::string &name)
    : name(name), parameters(), byRef(false), block(nullptr)
{}

std::vector<PSC::DataType> Procedure::getTypes() const {
    std::vector<PSC::DataType> types;
    types.reserve(parameters.size());
    for (auto p : parameters) types.push_back(p.type);
    return types;
}

void Procedure::run(PSC::Context &ctx) {
    block->run(ctx);
}

Function::Function(
    const std::string &name,
    std::vector<Parameter> &&parameters,
    bool byRef,
    PSC::Block *block,
    PSC::DataType returnType,
    const Token *defToken
)
: Procedure(name, std::move(parameters), byRef, block),
    returnType(returnType),
    defToken(defToken)
{}

Function::Function(const std::string &name, PSC::DataType returnType)
    : Procedure(name),
    returnType(returnType),
    defToken(nullptr)
{}
