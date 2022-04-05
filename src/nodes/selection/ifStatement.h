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
public:
    std::vector<IfConditionComponent> components;

    using Node::Node;

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
