#include "pch.h"

#include "nodes/cast.h"

CastNode::CastNode(const Token &token, Node &node, PSC::DataType target)
    : UnaryNode(token, node), target(target)
{}

std::unique_ptr<NodeResult> CastNode::evaluate(PSC::Context &ctx) {
    auto value = node.evaluate(ctx);

    if (value->type == target) return value;

    std::unique_ptr<PSC::Value> result;
    switch(target) {
        case PSC::DataType::INTEGER:
            result = value->data->toInteger();
            break;
        case PSC::DataType::REAL:
            result = value->data->toReal();
            break;
        case PSC::DataType::BOOLEAN:
            result = value->data->toBoolean();
            break;
        case PSC::DataType::CHAR:
            result = value->data->toChar();
            break;
        case PSC::DataType::STRING:
            result = value->data->toString();
            break;
        default:
            std::abort();
    }

    return std::make_unique<NodeResult>(std::move(result), target);
}
