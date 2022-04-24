#pragma once
#include <vector>
#include "nodes/base.h"
#include "psc/scope/block.h"

struct IfConditionComponent {
    // nullptr represents ELSE component
    Node *condition;
    PSC::Block &block;

    IfConditionComponent(Node *condition, PSC::Block &block);
};

class IfStatementNode : public Node {
private:
    std::vector<IfConditionComponent> components;

public:
    IfStatementNode(const Token &token, std::vector<IfConditionComponent> &&components);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
