#include "pch.h"

#include "psc/error.h"
#include "psc/scope/context.h"
#include "psc/builtinFunctions/functions.h"

using namespace std::chrono;

PSC::BuiltinFnDAY::BuiltinFnDAY()
    : Function("DAY", PSC::DataType::INTEGER)
{
    parameters.emplace_back("Date", PSC::DataType::DATE);
}

void PSC::BuiltinFnDAY::run(PSC::Context &ctx) {
    PSC::Variable *var = ctx.getVariable("Date");
    if (var == nullptr || var->type != PSC::DataType::DATE) std::abort();

    int_t day = static_cast<unsigned int>(var->get<PSC::Date>().date.day());
    auto ret = std::make_unique<PSC::Integer>(day);

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::INTEGER);
}

PSC::BuiltinFnMONTH::BuiltinFnMONTH()
    : Function("MONTH", PSC::DataType::INTEGER)
{
    parameters.emplace_back("Date", PSC::DataType::DATE);
}

void PSC::BuiltinFnMONTH::run(PSC::Context &ctx) {
    PSC::Variable *var = ctx.getVariable("Date");
    if (var == nullptr || var->type != PSC::DataType::DATE) std::abort();

    int_t day = static_cast<unsigned int>(var->get<PSC::Date>().date.month());
    auto ret = std::make_unique<PSC::Integer>(day);

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::INTEGER);
}

PSC::BuiltinFnYEAR::BuiltinFnYEAR()
    : Function("YEAR", PSC::DataType::INTEGER)
{
    parameters.emplace_back("Date", PSC::DataType::DATE);
}

void PSC::BuiltinFnYEAR::run(PSC::Context &ctx) {
    PSC::Variable *var = ctx.getVariable("Date");
    if (var == nullptr || var->type != PSC::DataType::DATE) std::abort();

    int_t day = static_cast<int>(var->get<PSC::Date>().date.year());
    auto ret = std::make_unique<PSC::Integer>(day);

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::INTEGER);
}

PSC::BuiltinFnDAYINDEX::BuiltinFnDAYINDEX()
    : Function("DAYINDEX", PSC::DataType::INTEGER)
{
    parameters.emplace_back("Date", PSC::DataType::DATE);
}

void PSC::BuiltinFnDAYINDEX::run(PSC::Context &ctx) {
    PSC::Variable *var = ctx.getVariable("Date");
    if (var == nullptr || var->type != PSC::DataType::DATE) std::abort();

    auto date = var->get<PSC::Date>().date;
    weekday weekday(date);
    int_t day = weekday.c_encoding() + 1;
    auto ret = std::make_unique<PSC::Integer>(day);

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::INTEGER);
}

PSC::BuiltinFnSETDATE::BuiltinFnSETDATE()
    : Function("SETDATE", PSC::DataType::DATE)
{
    parameters.emplace_back("Day", PSC::DataType::INTEGER);
    parameters.emplace_back("Month", PSC::DataType::INTEGER);
    parameters.emplace_back("Year", PSC::DataType::INTEGER);
}

Token PSC::BuiltinFnSETDATE::errToken {TokenType::FUNCTION, 0, 0};

void PSC::BuiltinFnSETDATE::run(PSC::Context &ctx) {
    PSC::Variable *dayVar = ctx.getVariable("Day");
    if (dayVar == nullptr || dayVar->type != PSC::DataType::INTEGER) std::abort();

    PSC::Variable *monthVar = ctx.getVariable("Month");
    if (monthVar == nullptr || monthVar->type != PSC::DataType::INTEGER) std::abort();

    PSC::Variable *yearVar = ctx.getVariable("Year");
    if (yearVar == nullptr || yearVar->type != PSC::DataType::INTEGER) std::abort();

    day day(dayVar->get<PSC::Integer>().value);
    month month(monthVar->get<PSC::Integer>().value);
    year year(yearVar->get<PSC::Integer>().value);

    year_month_day ymd(year, month, day);
    if (!ymd.ok())
        throw PSC::RuntimeError(errToken, ctx, "Invalid Date!");

    auto ret = std::make_unique<PSC::Date>(ymd);

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::DATE);
}

PSC::BuiltinFnTODAY::BuiltinFnTODAY()
    : Function("TODAY", PSC::DataType::DATE)
{}

void PSC::BuiltinFnTODAY::run(PSC::Context &ctx) {
    auto tp = system_clock::now();
    time_t tt = system_clock::to_time_t(tp);
    tm local_tm = *localtime(&tt);

    day day(local_tm.tm_mday);
    month month(local_tm.tm_mon + 1);
    year year(local_tm.tm_year + 1900);

    auto ret = std::make_unique<PSC::Date>(year_month_day(year, month, day));

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::DATE);
}
