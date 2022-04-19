#pragma once
#include "nodes/base.h"

class DeclareNode : public Node {
private:
    const std::vector<const Token*> identifiers;
    const PSC::DataType type;

public:
    // token: DECLARE
    DeclareNode(const Token &token, std::vector<const Token*> &&identifiers, PSC::DataType type);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class ConstDeclareNode : public UnaryNode {
private:
    const Token &identifier;

public:
    // token: CONST
    ConstDeclareNode(const Token &token, Node &node, const Token &identifier);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class AssignNode : public UnaryNode {
private:
    const Token &identifier;

public:
    // token: ASSIGNMENT
    AssignNode(const Token &token, Node &node, const Token &identifier);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class AccessNode : public Node {
public:
    // token: IDENTIFIER
    using Node::Node;

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
