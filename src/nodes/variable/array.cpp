#include "pch.h"

#include "psc/error.h"
#include "nodes/variable/variable.h"
#include "nodes/variable/array.h"

ArrayDeclareNode::ArrayDeclareNode(const Token &token, std::vector<const Token*> &&identifiers, const Token &type, std::vector<Node*> &&bounds)
    : Node(token),
    identifiers(identifiers),
    type(type),
    bounds(std::move(bounds))
{}

std::unique_ptr<NodeResult> ArrayDeclareNode::evaluate(PSC::Context &ctx) {
    if (bounds.size() % 2 != 0 || bounds.size() == 0) std::abort();

    for (auto identifier : identifiers) {
        if (ctx.getArray(identifier->value, false) != nullptr)
            throw PSC::RedeclarationError(token, ctx, identifier->value);
    }

    std::vector<PSC::ArrayDimension> dimensions;

    for (size_t i = 0; i < bounds.size(); i += 2) {
        auto lowerRes = bounds[i]->evaluate(ctx);
        if (lowerRes->type != PSC::DataType::INTEGER)
            throw PSC::RuntimeError(bounds[i]->getToken(), ctx, "Array indices must be of type INTEGER");

        auto upperRes = bounds[i + 1]->evaluate(ctx);
        if (upperRes->type != PSC::DataType::INTEGER)
            throw PSC::RuntimeError(bounds[i + 1]->getToken(), ctx, "Array indices must be of type INTEGER");

        const PSC::int_t &lower = lowerRes->get<PSC::Integer>().value;
        const PSC::int_t &upper = upperRes->get<PSC::Integer>().value;

        if (upper < lower)
            throw PSC::RuntimeError(bounds[i + 1]->getToken(), ctx, "Array upper bound must be greater than lower bound");

        dimensions.emplace_back((PSC::int_t) (i / 2), lower, upper);
    }

    for (auto identifier : identifiers) {
        PSC::DataType dataType = ctx.getType(type);
        if (dataType.type == PSC::DataType::NONE)
            throw PSC::NotDefinedError(token, ctx, "Type '" + type.value + "'");

        auto array = std::make_unique<PSC::Array>(identifier->value, dataType, dimensions);
        array->init(ctx);
        ctx.addArray(std::move(array));
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
