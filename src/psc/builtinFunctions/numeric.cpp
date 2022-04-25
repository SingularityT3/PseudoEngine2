#include "pch.h"
#include <math.h>

#include "psc/variable.h"
#include "psc/scope/context.h"
#include "psc/builtinFunctions/functions.h"


PSC::BuiltinFnRand::BuiltinFnRand()
    : Function("RAND")
{
    returnType = PSC::DataType::REAL;
    parameters.emplace_back("x", PSC::DataType::INTEGER);
}

void PSC::BuiltinFnRand::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::INTEGER) std::abort();

    auto ret = std::make_unique<PSC::Real>(rand() % x->get<PSC::Integer>().value);
    ret->value += rand() / (real_t) RAND_MAX;

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}


PSC::BuiltinFnInt::BuiltinFnInt()
    : Function("INT")
{
    returnType = PSC::DataType::INTEGER;
    parameters.emplace_back("x", PSC::DataType::REAL);
}

void PSC::BuiltinFnInt::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    auto ret = std::make_unique<PSC::Integer>((int_t) floor(x->get<PSC::Real>().value));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::INTEGER);
}
