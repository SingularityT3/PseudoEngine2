#include <math.h>
#include "psc/variable.h"
#include "psc/scope/context.h"
#include "psc/builtinFunctions/functions.h"


PSC::BuiltinFnRand::BuiltinFnRand()
    : Function("RAND")
{
    returnType = PSC::DT_REAL;
    parameters.emplace_back("x", PSC::DT_INTEGER);
}

void PSC::BuiltinFnRand::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DT_INTEGER) std::abort();

    auto ret = std::make_unique<PSC::Real>(rand() % x->get<PSC::Integer>().value);
    ret->value += rand() / (real_t) RAND_MAX;

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DT_REAL);
}


PSC::BuiltinFnInt::BuiltinFnInt()
    : Function("INT")
{
    returnType = PSC::DT_INTEGER;
    parameters.emplace_back("x", PSC::DT_REAL);
}

void PSC::BuiltinFnInt::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DT_REAL) std::abort();

    auto ret = std::make_unique<PSC::Integer>((int_t) std::floor(x->get<PSC::Real>().value));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DT_INTEGER);
}
