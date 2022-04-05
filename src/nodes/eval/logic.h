#pragma once
#include "nodes/base.h"

class LogicNode : public BinaryNode {
private:
    std::string op;
public:
    LogicNode(const Token &token, Node &left, Node &right);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class NotNode : public UnaryNode {
public:
    using UnaryNode::UnaryNode;

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
