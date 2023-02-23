#pragma once
#include "nodes/variable/resolver.h"
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
    const std::unique_ptr<AbstractVariableResolver> resolver;

public:
    InputNode(const Token &token, std::unique_ptr<AbstractVariableResolver> &&resolver);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
