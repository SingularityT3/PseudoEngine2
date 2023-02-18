#include "pch.h"
#include <math.h>

#include "psc/builtinFunctions/functions.h"
#include "psc/scope/context.h"

PSC::BuiltinFnPow::BuiltinFnPow()
    : Function("POW")
{
    returnType = PSC::DataType::REAL;
    parameters.emplace_back("x", PSC::DataType::REAL);
    parameters.emplace_back("y", PSC::DataType::REAL);
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
    : Function("EXP")
{
    returnType = PSC::DataType::REAL;
    parameters.emplace_back("x", PSC::DataType::REAL);
}

void PSC::BuiltinFnExp::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) exp(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnSin::BuiltinFnSin()
    : Function("SIN")
{
    returnType = PSC::DataType::REAL;
    parameters.emplace_back("x", PSC::DataType::REAL);
}

void PSC::BuiltinFnSin::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) sin(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnCos::BuiltinFnCos()
    : Function("COS")
{
    returnType = PSC::DataType::REAL;
    parameters.emplace_back("x", PSC::DataType::REAL);
}

void PSC::BuiltinFnCos::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) cos(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnTan::BuiltinFnTan()
    : Function("TAN")
{
    returnType = PSC::DataType::REAL;
    parameters.emplace_back("x", PSC::DataType::REAL);
}

void PSC::BuiltinFnTan::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) tan(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnASin::BuiltinFnASin()
    : Function("ASIN")
{
    returnType = PSC::DataType::REAL;
    parameters.emplace_back("x", PSC::DataType::REAL);
}

void PSC::BuiltinFnASin::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) asin(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnACos::BuiltinFnACos()
    : Function("ACOS")
{
    returnType = PSC::DataType::REAL;
    parameters.emplace_back("x", PSC::DataType::REAL);
}

void PSC::BuiltinFnACos::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) acos(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnATan::BuiltinFnATan()
    : Function("ATAN")
{
    returnType = PSC::DataType::REAL;
    parameters.emplace_back("x", PSC::DataType::REAL);
}

void PSC::BuiltinFnATan::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) atan(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnATan2::BuiltinFnATan2()
    : Function("ATAN2")
{
    returnType = PSC::DataType::REAL;
    parameters.emplace_back("y", PSC::DataType::REAL);
    parameters.emplace_back("x", PSC::DataType::REAL);
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
    : Function("SQRT")
{
    returnType = PSC::DataType::REAL;
    parameters.emplace_back("x", PSC::DataType::REAL);
}

void PSC::BuiltinFnSqrt::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) sqrt(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnLog::BuiltinFnLog()
    : Function("LOG")
{
    returnType = PSC::DataType::REAL;
    parameters.emplace_back("x", PSC::DataType::REAL);
}

void PSC::BuiltinFnLog::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) log10(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}

PSC::BuiltinFnLn::BuiltinFnLn()
    : Function("LN")
{
    returnType = PSC::DataType::REAL;
    parameters.emplace_back("x", PSC::DataType::REAL);
}

void PSC::BuiltinFnLn::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    PSC::Real xVal = x->get<PSC::Real>();
    auto ret = std::make_unique<PSC::Real>((PSC::real_t) log(xVal));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}
