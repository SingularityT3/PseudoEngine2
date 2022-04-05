#include "nodeResult.h"

NodeResult::NodeResult(const PSC::Value *data, PSC::DataType type)
    : data(data), type(type)
{}

NodeResult::NodeResult(std::unique_ptr<const PSC::Value> &&data, PSC::DataType type)
    : type(type)
{
    this->data = std::move(data);
}
