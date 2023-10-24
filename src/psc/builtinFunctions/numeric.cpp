#include "pch.h"
#include <math.h>

#include "psc/types/types.h"
#include "psc/variable.h"
#include "psc/scope/context.h"
#include "psc/builtinFunctions/functions.h"


PSC::BuiltinFnRand::BuiltinFnRand()
    : Function("RAND", PSC::DataType::REAL)
{
    parameters.emplace_back("x", PSC::DataType::INTEGER, false);
}

void PSC::BuiltinFnRand::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::INTEGER) std::abort();

    PSC::int_t maxValue = x->get<PSC::Integer>().value;
    PSC::real_t random;

    if (maxValue > 0) {
        random = (rand() % maxValue) + (rand() / (PSC::real_t) RAND_MAX);
    } else {
        random = 0.0f;
    }

    auto ret = std::make_unique<PSC::Real>(random);

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}


PSC::BuiltinFnInt::BuiltinFnInt()
    : Function("INT", PSC::DataType::INTEGER)
{
    parameters.emplace_back("x", PSC::DataType::REAL, false);
}

void PSC::BuiltinFnInt::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    auto ret = std::make_unique<PSC::Integer>((int_t) floor(x->get<PSC::Real>().value));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::INTEGER);
}
