#pragma once
#include "nodes/base.h"

class OutputNode : public Node {
public:
    std::vector<Node*> nodes;

    using Node::Node;

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class InputNode : public Node {
private:
    const Token &identifier;

public:
    InputNode(const Token &token, const Token &identifier);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
