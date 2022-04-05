#pragma once
#include "nodes/base.h"
#include "psc/types/types.h"

class IntegerNode : public Node {
private:
    const PSC::Integer valueInt;

public:
    IntegerNode(const Token &token);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class RealNode : public Node {
private:
    const PSC::Real valueReal;

public:
    RealNode(const Token &token);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class CharNode : public Node {
private:
    const PSC::Char valueChar;

public:
    CharNode(const Token &token);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class StringNode : public Node {
private:
    const PSC::String valueStr;

public:
    StringNode(const Token &token);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};


class NegateNode : public UnaryNode {
public:
    using UnaryNode::UnaryNode;

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class ArithmeticOperationNode : public BinaryNode {
private:
    std::string op;

public:
    ArithmeticOperationNode(const Token &token, Node &left, Node &right);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
