#include "pch.h"

#include "psc/error.h"
#include "nodes/eval/stringcat.h"

std::unique_ptr<NodeResult> StringConcatenationNode::evaluate(PSC::Context &ctx) {
    auto leftRes = left.evaluate(ctx);
    auto rightRes = right.evaluate(ctx);

    if (!leftRes->data->isPrimitive() || !rightRes->data->isPrimitive())
        throw PSC::TypeOperationError(token, ctx, "'&'");

    auto leftStr = static_cast<const PSC::Primitive*>(leftRes->data.get())->toString();
    auto rightStr = static_cast<const PSC::Primitive*>(rightRes->data.get())->toString();

    auto res = (*leftStr) & (*rightStr);

    return std::make_unique<NodeResult>(std::move(res), PSC::DataType::STRING);
}
