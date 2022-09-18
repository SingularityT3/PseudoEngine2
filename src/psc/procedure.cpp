#include "pch.h"

#include "psc/scope/block.h"
#include "psc/scope/context.h"
#include "procedure.h"

using namespace PSC;

Parameter::Parameter(const std::string &name, PSC::DataType type)
    : name(name), type(type)
{}

Procedure::Procedure(const std::string &name)
    : name(name)
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
