#pragma once
#include "nodes/base.h"
#include "psc/procedure.h"

class FunctionNode : public Node {
private:
    PSC::Function *function;
    bool defined = false;

public:
    FunctionNode(const Token &token, PSC::Function *function);

    ~FunctionNode();

    // Adds function to ctx
    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class FunctionCallNode : public Node {
private:
    const std::string functionName;

public:
    std::vector<Node*> args;

    FunctionCallNode(const Token &token);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class ReturnNode : public UnaryNode {
public:
    using UnaryNode::UnaryNode;

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
