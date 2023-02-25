#include "pch.h"

#include "psc/error.h"
#include "nodes/cast.h"

CastNode::CastNode(const Token &token, Node &node, PSC::DataType target)
    : UnaryNode(token, node), target(target)
{}

std::unique_ptr<NodeResult> CastNode::evaluate(PSC::Context &ctx) {
    auto value = node.evaluate(ctx);

    if (!value->data->isPrimitive())
        throw PSC::TypeOperationError(token, ctx, "Cast");
    if (value->type == target) return value;

    auto pvalue = static_cast<const PSC::Primitive*>(value->data.get());

    std::unique_ptr<PSC::Value> result;
    switch(target.type) {
        case PSC::DataType::INTEGER:
            result = pvalue->toInteger();
            break;
        case PSC::DataType::REAL:
            result = pvalue->toReal();
            break;
        case PSC::DataType::BOOLEAN:
            result = pvalue->toBoolean();
            break;
        case PSC::DataType::CHAR:
            result = pvalue->toChar();
            break;
        case PSC::DataType::STRING:
            result = pvalue->toString();
            break;
        case PSC::DataType::DATE:
        case PSC::DataType::ENUM:
        case PSC::DataType::POINTER:
        case PSC::DataType::COMPOSITE:
            throw PSC::TypeOperationError(token, ctx, "Cast");
        case PSC::DataType::NONE:
            std::abort();
    }

    return std::make_unique<NodeResult>(std::move(result), target);
}
