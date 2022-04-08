#include <ctype.h>
#include "psc/variable.h"
#include "psc/scope/context.h"
#include "psc/builtinFunctions/functions.h"

PSC::BuiltinFnLCase::BuiltinFnLCase()
    : Function("LCASE")
{
    returnType = PSC::DT_CHAR;
    parameters.emplace_back("Char", PSC::DT_CHAR);
}

void PSC::BuiltinFnLCase::run(PSC::Context &ctx) {
    PSC::Variable *ch = ctx.getVariable("Char");
    if (ch == nullptr || ch->type != PSC::DT_CHAR) std::abort();

    auto ret = std::make_unique<PSC::Char>();
    ret->value = std::tolower(ch->get<PSC::Char>().value);

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DT_CHAR);
}


PSC::BuiltinFnUCase::BuiltinFnUCase()
    : Function("UCASE")
{
    returnType = PSC::DT_CHAR;
    parameters.emplace_back("Char", PSC::DT_CHAR);
}

void PSC::BuiltinFnUCase::run(PSC::Context &ctx) {
    PSC::Variable *ch = ctx.getVariable("Char");
    if (ch == nullptr || ch->type != PSC::DT_CHAR) std::abort();

    auto ret = std::make_unique<PSC::Char>();
    ret->value = std::toupper(ch->get<PSC::Char>().value);

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DT_CHAR);
}


PSC::BuiltinFnASC::BuiltinFnASC()
    : Function("ASC")
{
    returnType = PSC::DT_INTEGER;
    parameters.emplace_back("Char", PSC::DT_CHAR);
}

void PSC::BuiltinFnASC::run(PSC::Context &ctx) {
    PSC::Variable *ch = ctx.getVariable("Char");
    if (ch == nullptr || ch->type != PSC::DT_CHAR) std::abort();

    auto ret = std::make_unique<PSC::Integer>();
    ret->value = (int_t) ch->get<PSC::Char>().value;

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DT_INTEGER);
}


PSC::BuiltinFnCHR::BuiltinFnCHR()
    : Function("CHR")
{
    returnType = PSC::DT_CHAR;
    parameters.emplace_back("x", PSC::DT_INTEGER);
}

void PSC::BuiltinFnCHR::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DT_INTEGER) std::abort();

    auto ret = std::make_unique<PSC::Char>();
    ret->value = (char) x->get<PSC::Integer>().value;

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DT_CHAR);
}
