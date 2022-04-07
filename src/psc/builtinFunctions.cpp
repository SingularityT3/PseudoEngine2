#include <ctype.h>
#include "psc/variable.h"
#include "psc/scope/context.h"
#include "psc/builtinFunctions.h"

PSC::BuiltinFnLength::BuiltinFnLength()
    : Function("LENGTH")
{
    returnType = PSC::DT_INTEGER;
    parameters.emplace_back("String", PSC::DT_STRING);
}

void PSC::BuiltinFnLength::run(PSC::Context &ctx) {
    PSC::Variable *var = ctx.getVariable("String");
    if (var == nullptr || var->type != PSC::DT_STRING) std::abort();

    int_t len = var->get<PSC::String>().value.size();
    auto ret = std::make_unique<PSC::Integer>(len);

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DT_INTEGER);
}


PSC::BuiltinFnRight::BuiltinFnRight()
    : Function("RIGHT")
{
    returnType = PSC::DT_STRING;
    parameters.reserve(2);
    parameters.emplace_back("String", PSC::DT_STRING);
    parameters.emplace_back("x", PSC::DT_INTEGER);
}

void PSC::BuiltinFnRight::run(PSC::Context &ctx) {
    PSC::Variable *str = ctx.getVariable("String");
    if (str == nullptr || str->type != PSC::DT_STRING) std::abort();

    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DT_INTEGER) std::abort();

    int_t xVal = x->get<PSC::Integer>().value;
    std::string strVal = str->get<PSC::String>().value;
    int strLen = strVal.size();
    auto ret = std::make_unique<PSC::String>();

    int start = strLen - xVal;
    for (int i = start > 0 ? start : 0; i < strLen; i++) {
        ret->value += strVal[i];
    }

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DT_STRING);
}


PSC::BuiltinFnMid::BuiltinFnMid()
    : Function("MID")
{
    returnType = PSC::DT_STRING;
    parameters.reserve(3);
    parameters.emplace_back("String", PSC::DT_STRING);
    parameters.emplace_back("x", PSC::DT_INTEGER);
    parameters.emplace_back("y", PSC::DT_INTEGER);
}

void PSC::BuiltinFnMid::run(PSC::Context &ctx) {
    PSC::Variable *str = ctx.getVariable("String");
    if (str == nullptr || str->type != PSC::DT_STRING) std::abort();

    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DT_INTEGER) std::abort();

    PSC::Variable *y = ctx.getVariable("y");
    if (y == nullptr || y->type != PSC::DT_INTEGER) std::abort();


    auto ret = std::make_unique<PSC::String>();
    std::string strVal = str->get<PSC::String>().value;

    int_t xVal = x->get<PSC::Integer>().value - 1;
    if (xVal < 0) xVal = 0;
    int_t yVal = y->get<PSC::Integer>().value;
    if (yVal < 0) yVal = 0;

    ret->value = std::move(strVal.substr(xVal, yVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DT_STRING);
}


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
