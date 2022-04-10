#include "psc/error.h"
#include "nodes/loop/while.h"

WhileLoopNode::WhileLoopNode(const Token &token, Node &condition, PSC::Block &block)
    : UnaryNode(token, condition), block(block)
{}

std::unique_ptr<NodeResult> WhileLoopNode::evaluate(PSC::Context &ctx) {
    while (true) {
        auto conditionRes = node.evaluate(ctx);

        if (conditionRes->type != PSC::DataType::BOOLEAN)
            throw PSC::ConditionTypeError(token, ctx);

        if (!conditionRes->get<PSC::Boolean>()) break;
        block.run(ctx);
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
