#include "pch.h"
#include <algorithm>

#include "psc/variable.h"
#include "psc/file.h"
#include "psc/error.h"
#include "psc/scope/context.h"
#include "psc/builtinFunctions/functions.h"

PSC::BuiltinFnLength::BuiltinFnLength()
    : Function("LENGTH", PSC::DataType::INTEGER)
{
    parameters.emplace_back("String", PSC::DataType::STRING);
}

void PSC::BuiltinFnLength::run(PSC::Context &ctx) {
    PSC::Variable *var = ctx.getVariable("String");
    if (var == nullptr || var->type != PSC::DataType::STRING) std::abort();

    int_t len = (int_t) var->get<PSC::String>().value.size();
    auto ret = std::make_unique<PSC::Integer>(len);

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::INTEGER);
}


PSC::BuiltinFnRight::BuiltinFnRight()
    : Function("RIGHT", PSC::DataType::STRING)
{
    parameters.reserve(2);
    parameters.emplace_back("String", PSC::DataType::STRING);
    parameters.emplace_back("x", PSC::DataType::INTEGER);
}

void PSC::BuiltinFnRight::run(PSC::Context &ctx) {
    PSC::Variable *str = ctx.getVariable("String");
    if (str == nullptr || str->type != PSC::DataType::STRING) std::abort();

    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::INTEGER) std::abort();

    std::string &strVal = str->get<PSC::String>().value;
    size_t strLen = strVal.size();

    int_t xVal = x->get<PSC::Integer>().value;
    if (xVal < 0)
        throw PSC::RuntimeError(PSC::errToken, ctx, "Length for 'RIGHT' function cannot be negative");
    if (static_cast<size_t>(xVal) > strLen)
        throw PSC::RuntimeError(PSC::errToken, ctx, "Length for 'RIGHT' function cannot exceed string length");

    auto ret = std::make_unique<PSC::String>();

    size_t start = strLen - xVal;
    for (size_t i = start > 0 ? start : 0; i < strLen; i++) {
        ret->value += strVal[i];
    }

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::STRING);
}


PSC::BuiltinFnMid::BuiltinFnMid()
    : Function("MID", PSC::DataType::STRING)
{
    parameters.reserve(3);
    parameters.emplace_back("String", PSC::DataType::STRING);
    parameters.emplace_back("x", PSC::DataType::INTEGER);
    parameters.emplace_back("y", PSC::DataType::INTEGER);
}

void PSC::BuiltinFnMid::run(PSC::Context &ctx) {
    PSC::Variable *str = ctx.getVariable("String");
    if (str == nullptr || str->type != PSC::DataType::STRING) std::abort();

    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::INTEGER) std::abort();

    PSC::Variable *y = ctx.getVariable("y");
    if (y == nullptr || y->type != PSC::DataType::INTEGER) std::abort();


    auto ret = std::make_unique<PSC::String>();
    std::string &strVal = str->get<PSC::String>().value;
    size_t strLen = strVal.size();

    int_t xVal = x->get<PSC::Integer>().value - 1;
    if (xVal < 0)
        throw PSC::RuntimeError(PSC::errToken, ctx, "Index for 'MID' function cannot be negative");
    if (static_cast<size_t>(xVal) >= strLen)
        throw PSC::RuntimeError(PSC::errToken, ctx, "Index for 'MID' function cannot exceed string length");

    int_t yVal = y->get<PSC::Integer>().value;
    if (yVal < 0)
        throw PSC::RuntimeError(PSC::errToken, ctx, "Length for 'MID' function cannot be negative");
    if (static_cast<size_t>(yVal + xVal) > strLen)
        throw PSC::RuntimeError(PSC::errToken, ctx, "Substring length in 'MID' function cannot exceed string length");

    ret->value = strVal.substr(xVal, yVal);

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::STRING);
}


PSC::BuiltinFnLeft::BuiltinFnLeft()
    : Function("LEFT", PSC::DataType::STRING)
{
    parameters.reserve(2);
    parameters.emplace_back("String", PSC::DataType::STRING);
    parameters.emplace_back("x", PSC::DataType::INTEGER);
}

void PSC::BuiltinFnLeft::run(PSC::Context &ctx) {
    PSC::Variable *str = ctx.getVariable("String");
    if (str == nullptr || str->type != PSC::DataType::STRING) std::abort();

    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::INTEGER) std::abort();

    std::string &strVal = str->get<PSC::String>().value;

    int_t xVal = x->get<PSC::Integer>().value;
    if (xVal < 0)
        throw PSC::RuntimeError(PSC::errToken, ctx, "Length for 'LEFT' function cannot be negative");
    if (static_cast<size_t>(xVal) > strVal.size())
        throw PSC::RuntimeError(PSC::errToken, ctx, "Length for 'LEFT' function cannot exceed string length");

    auto ret = std::make_unique<PSC::String>();
    ret->value = strVal.substr(0, xVal);

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::STRING);
}


PSC::BuiltinFnToUpper::BuiltinFnToUpper()
    : Function("TO_UPPER", PSC::DataType::STRING)
{
    parameters.emplace_back("String", PSC::DataType::STRING);
}

void PSC::BuiltinFnToUpper::run(PSC::Context &ctx) {
    PSC::Variable *str = ctx.getVariable("String");
    if (str == nullptr || str->type != PSC::DataType::STRING) std::abort();

    auto ret = std::make_unique<PSC::String>();

    ret->value = str->get<PSC::String>().value;
    std::transform(ret->value.begin(), ret->value.end(), ret->value.begin(), toupper);

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::STRING);
}


PSC::BuiltinFnToLower::BuiltinFnToLower()
    : Function("TO_LOWER", PSC::DataType::STRING)
{
    parameters.emplace_back("String", PSC::DataType::STRING);
}

void PSC::BuiltinFnToLower::run(PSC::Context &ctx) {
    PSC::Variable *str = ctx.getVariable("String");
    if (str == nullptr || str->type != PSC::DataType::STRING) std::abort();

    auto ret = std::make_unique<PSC::String>();

    ret->value = str->get<PSC::String>().value;
    std::transform(ret->value.begin(), ret->value.end(), ret->value.begin(), tolower);

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::STRING);
}


PSC::BuiltinFnNumToStr::BuiltinFnNumToStr()
    : Function("NUM_TO_STR", PSC::DataType::STRING)
{
    parameters.emplace_back("x", PSC::DataType::REAL);
}

void PSC::BuiltinFnNumToStr::run(PSC::Context &ctx) {
    PSC::Variable *x = ctx.getVariable("x");
    if (x == nullptr || x->type != PSC::DataType::REAL) std::abort();

    auto ret = x->get<PSC::Real>().toString();

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::STRING);
}


PSC::BuiltinFnStrToNum::BuiltinFnStrToNum()
    : Function("STR_TO_NUM", PSC::DataType::REAL)
{
    parameters.emplace_back("String", PSC::DataType::STRING);
}

void PSC::BuiltinFnStrToNum::run(PSC::Context &ctx) {
    PSC::Variable *str = ctx.getVariable("String");
    if (str == nullptr || str->type != PSC::DataType::STRING) std::abort();

    auto ret = str->get<PSC::String>().toReal();

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::REAL);
}


PSC::BuiltinFnIsNum::BuiltinFnIsNum()
    : Function("IS_NUM", PSC::DataType::BOOLEAN)
{
    parameters.emplace_back("String", PSC::DataType::STRING);
}

void PSC::BuiltinFnIsNum::run(PSC::Context &ctx) {
    PSC::Variable *str = ctx.getVariable("String");
    if (str == nullptr || str->type != PSC::DataType::STRING) std::abort();

    std::string &strVal = str->get<PSC::String>().value;
    auto ret = std::make_unique<PSC::Boolean>(true);
    bool decimal = false;

    for (char &c : strVal) {
        if (c == '.') {
            if (decimal) {
                ret->value = false;
                break;
            } else {
                decimal = true;
            }
        } else if (c < '0' || c > '9') {
            ret->value = false;
            break;
        }
    }

    ctx.returnValue = std::make_unique<NodeResult>(std::move(ret), PSC::DataType::BOOLEAN);
}

PSC::BuiltinFnEOF::BuiltinFnEOF()
    : Function("EOF", PSC::DataType::BOOLEAN)
{
    parameters.emplace_back("File", PSC::DataType::STRING);
}

void PSC::BuiltinFnEOF::run(PSC::Context &ctx) {
    PSC::Variable *fileName = ctx.getVariable("File");
    if (fileName == nullptr || fileName->type != PSC::DataType::STRING) std::abort();

    auto &filenameStr = fileName->get<PSC::String>();
    PSC::File *file = PSC::FileManager::getFile(filenameStr);
    
    if (file == nullptr)
        throw PSC::FileNotOpenError(PSC::errToken, ctx, filenameStr.value);
    if (file->getMode() != FileMode::READ)
        throw PSC::RuntimeError(PSC::errToken, ctx, "File is not open in READ mode");
    
    PSC::Boolean *eof = new PSC::Boolean;
    *eof = file->eof();

    ctx.returnValue = std::make_unique<NodeResult>(eof, PSC::DataType::BOOLEAN);
}
