#include "nodes/eval/stringcat.h"

std::unique_ptr<NodeResult> StringConcatenationNode::evaluate(PSC::Context &ctx) {
    auto leftRes = left.evaluate(ctx);
    auto rightRes = right.evaluate(ctx);
    // TODO
    return std::make_unique<NodeResult>(nullptr, PSC::DT_NONE);
}
