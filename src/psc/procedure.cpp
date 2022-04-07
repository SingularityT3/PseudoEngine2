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

void Procedure::run(PSC::Context &ctx) {
    block->run(ctx);
}
