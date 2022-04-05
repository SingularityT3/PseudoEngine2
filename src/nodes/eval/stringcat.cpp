#include "nodes/eval/stringcat.h"

std::unique_ptr<NodeResult> StringConcatenationNode::evaluate(PSC::Context &ctx) {
    auto leftRes = left.evaluate(ctx);
    auto rightRes = right.evaluate(ctx);

    auto leftStr = leftRes->data->toString();
    auto rightStr = rightRes->data->toString();

    auto res = (*leftStr) & (*rightStr);

    return std::make_unique<NodeResult>(std::move(res), PSC::DT_STRING);
}
