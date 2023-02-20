#pragma once
#include "nodes/base.h"

class ReturnErrSignal {};

class FunctionNode : public Node {
private:
    const std::string functionName;
    const std::vector<std::string> parameterNames;
    const std::vector<const Token*> parameterTypes;
    const bool byRef;
    PSC::Block &block;
    const Token &returnType;

public:
    FunctionNode(
        const Token &token,
        const std::string &functionName,
        std::vector<std::string> &&parameterNames,
        std::vector<const Token*> &&parameterTypes,
        bool byRef,
        PSC::Block &block,
        const Token &returnType
    );

    // Adds function to ctx
    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class FunctionCallNode : public Node {
private:
    const std::string functionName;
    const std::vector<Node*> args;

public:
    FunctionCallNode(const Token &token, std::vector<Node*> &&args);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class ReturnNode : public UnaryNode {
public:
    using UnaryNode::UnaryNode;

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
