#pragma once
#include <memory>
#include "lexer/tokens.h"
#include "psc/types/types.h"
#include "psc/scope/context.h"
#include "nodes/nodeResult.h"

class Node {
public:
    const Token &token;

    Node(const Token &token);

    virtual ~Node() = default;

    virtual std::string toStr() const;

    virtual std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) = 0;

    const Token &getToken();
};

class UnaryNode : public Node {
public:
    Node &node;
    
    UnaryNode(const Token &token, Node &node);

    std::string toStr() const override;
};

class BinaryNode : public Node {
public:
    Node &left;
    Node &right;

    BinaryNode(const Token &token, Node &left, Node &right);

    std::string toStr() const override;
};
