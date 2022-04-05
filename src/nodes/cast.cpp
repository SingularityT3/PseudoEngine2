#include "nodes/cast.h"

CastNode::CastNode(const Token &token, Node &node, PSC::DataType target)
    : UnaryNode(token, node), target(target)
{}

std::unique_ptr<NodeResult> CastNode::evaluate(PSC::Context &ctx) {
    auto value = node.evaluate(ctx);

    if (value->type == target) return value;

    std::unique_ptr<PSC::Value> result;
    switch(target) {
        case PSC::DT_INTEGER:
            result = value->data->toInteger();
            break;
        case PSC::DT_REAL:
            result = value->data->toReal();
            break;
        case PSC::DT_BOOLEAN:
            result = value->data->toBoolean();
            break;
        case PSC::DT_CHAR:
            result = value->data->toChar();
            break;
        case PSC::DT_STRING:
            result = value->data->toString();
            break;
        default:
            throw 0;
    }

    return std::make_unique<NodeResult>(std::move(result), target);
}
