#include "nodeResult.h"

NodeResult::NodeResult(const PSC::Value *data, PSC::DataType type)
    : data(data), type(type)
{}

NodeResult::NodeResult(std::unique_ptr<const PSC::Value> &&data, PSC::DataType type)
    : type(type)
{
    this->data = std::move(data);
}

void NodeResult::implicitCast(PSC::DataType target) {
    // REAL -> INTEGER
    if (target == PSC::DataType::REAL && type == PSC::DataType::INTEGER) {
        type = PSC::DataType::REAL;
        data = data->toReal();
    }

    // STRING -> CHAR
    else if (target == PSC::DataType::CHAR && type == PSC::DataType::STRING
        && ((const PSC::String*) data.get())->value.length() == 1
    ) {
        type = PSC::DataType::CHAR;
        data = std::make_unique<PSC::Char>(((const PSC::String*) data.get())->value[0]);
    }

    // CHAR -> STRING
    else if (target == PSC::DataType::STRING && type == PSC::DataType::CHAR) {
        type = PSC::DataType::STRING;
        data = data->toString();
    }
}
