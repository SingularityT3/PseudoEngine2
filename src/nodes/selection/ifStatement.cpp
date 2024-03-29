#include "pch.h"

#include "psc/error.h"
#include "nodes/selection/ifStatement.h"

IfConditionComponent::IfConditionComponent(Node *condition, PSC::Block &block)
    : condition(condition), block(block)
{}


IfStatementNode::IfStatementNode(const Token &token, std::vector<IfConditionComponent> &&components)
    : Node(token), components(std::move(components))
{}

std::unique_ptr<NodeResult> IfStatementNode::evaluate(PSC::Context &ctx) {
    for (IfConditionComponent &component : components) {
        if (component.condition == nullptr) {
            component.block.run(ctx);
            break;
        }

        std::unique_ptr<NodeResult> conditionRes = component.condition->evaluate(ctx);
        if (conditionRes->type != PSC::DataType::BOOLEAN)
            throw PSC::ConditionTypeError(token, ctx);

        if (conditionRes->get<PSC::Boolean>()) {
            component.block.run(ctx);
            break;
        }
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
