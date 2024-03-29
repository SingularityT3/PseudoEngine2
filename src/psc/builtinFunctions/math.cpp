#include "pch.h"
#include <math.h>

#include "psc/builtinFunctions/functions.h"
#include "psc/scope/context.h"

PSC::BuiltinFnPow::BuiltinFnPow()
    : Function("POW", PSC::DataType::REAL)
{
    parameters.emplace_back("x", PSC::DataType::REAL, false);
    parameters.emplace_back("y", PSC::DataType::REAL, false);
}

void PSC::BuiltinFnPow::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Variable *y = ctx.getVariable("y");
    if (y == nullptr || y->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    PSC::Real yVal = y->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) pow(xVal, yVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnExp::BuiltinFnExp()
    : Function("EXP", PSC::DataType::REAL)
{
    parameters.emplace_back("x", PSC::DataType::REAL, false);
}

void PSC::BuiltinFnExp::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) exp(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnSin::BuiltinFnSin()
    : Function("SIN", PSC::DataType::REAL)
{
    parameters.emplace_back("x", PSC::DataType::REAL, false);
}

void PSC::BuiltinFnSin::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) sin(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnCos::BuiltinFnCos()
    : Function("COS", PSC::DataType::REAL)
{
    parameters.emplace_back("x", PSC::DataType::REAL, false);
}

void PSC::BuiltinFnCos::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) cos(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnTan::BuiltinFnTan()
    : Function("TAN", PSC::DataType::REAL)
{
    parameters.emplace_back("x", PSC::DataType::REAL, false);
}

void PSC::BuiltinFnTan::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) tan(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnASin::BuiltinFnASin()
    : Function("ASIN", PSC::DataType::REAL)
{
    parameters.emplace_back("x", PSC::DataType::REAL, false);
}

void PSC::BuiltinFnASin::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) asin(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnACos::BuiltinFnACos()
    : Function("ACOS", PSC::DataType::REAL)
{
    parameters.emplace_back("x", PSC::DataType::REAL, false);
}

void PSC::BuiltinFnACos::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) acos(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnATan::BuiltinFnATan()
    : Function("ATAN", PSC::DataType::REAL)
{
    parameters.emplace_back("x", PSC::DataType::REAL, false);
}

void PSC::BuiltinFnATan::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) atan(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnATan2::BuiltinFnATan2()
    : Function("ATAN2", PSC::DataType::REAL)
{
    parameters.emplace_back("y", PSC::DataType::REAL, false);
    parameters.emplace_back("x", PSC::DataType::REAL, false);
}

void PSC::BuiltinFnATan2::run(PSC::Context &ctx) {
    PSC::Variable *y = ctx.getVariable("y");
    if (y == nullptr || y->type != PSC::DataType::REAL) std::abort();

    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real yVal = y->get<PSC::Real>();
    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) atan2(yVal, xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnSqrt::BuiltinFnSqrt()
    : Function("SQRT", PSC::DataType::REAL)
{
    parameters.emplace_back("x", PSC::DataType::REAL, false);
}

void PSC::BuiltinFnSqrt::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) sqrt(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnLog::BuiltinFnLog()
    : Function("LOG", PSC::DataType::REAL)
{
    parameters.emplace_back("x", PSC::DataType::REAL, false);
}

void PSC::BuiltinFnLog::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) log10(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnLn::BuiltinFnLn()
    : Function("LN", PSC::DataType::REAL)
{
    parameters.emplace_back("x", PSC::DataType::REAL, false);
}

void PSC::BuiltinFnLn::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) log(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}
