#include "psc/error.h"
#include "nodes/loop/repeatUntil.h"

RepeatUntilNode::RepeatUntilNode(const Token &token, Node &condition, PSC::Block &block)
    : UnaryNode(token, condition), block(block)
{}

std::unique_ptr<NodeResult> RepeatUntilNode::evaluate(PSC::Context &ctx) {
    while (true) {
        block.run(ctx);

        auto conditionRes = node.evaluate(ctx);
        
        if (conditionRes->type != PSC::DataType::BOOLEAN)
            throw PSC::ConditionTypeError(token, ctx);

        if (conditionRes->get<PSC::Boolean>()) break;
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
