#pragma once
#include <vector>
#include "nodes/base.h"
#include "psc/array.h"

class ArrayDeclareNode : public Node {
private:
    std::unique_ptr<PSC::Array> array;

public:
    ArrayDeclareNode(const Token &token, std::unique_ptr<PSC::Array> &&array);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class ArrayAccessNode : public Node {
private:
    std::string arrayName;

public:
    std::vector<Node*> indices;

    ArrayAccessNode(const Token &token, const std::string &arrayName);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class ArrayAssignNode : public Node {
private:
    std::string arrayName;

public:
    std::vector<Node*> indices;
    Node *node;

    ArrayAssignNode(const Token &token, const std::string &arrayName);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
