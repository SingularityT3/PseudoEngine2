#pragma once
#include "nodes/base.h"

class OutputNode : public Node {
private:
    const std::vector<Node*> nodes;

public:
    OutputNode(const Token &token, std::vector<Node*> &&nodes);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class InputNode : public Node {
private:
    const Token &identifier;

public:
    InputNode(const Token &token, const Token &identifier);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
