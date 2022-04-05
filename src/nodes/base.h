#pragma once
#include <memory>
#include "tokens.h"
#include "psc/types.h"
#include "psc/context.h"
#include "nodes/nodeResult.h"

class Node {
protected:
    const Token &token;

public:
    Node(const Token &token);

    virtual ~Node() = default;

    virtual std::string toStr() const;

    virtual std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) = 0;

    const Token &getToken();
};

class UnaryNode : public Node {
protected:
    Node &node;

public:
    UnaryNode(const Token &token, Node &node);

    std::string toStr() const override;
};

class BinaryNode : public Node {
protected:
    Node &left;
    Node &right;

public:
    BinaryNode(const Token &token, Node &left, Node &right);

    std::string toStr() const override;
};
