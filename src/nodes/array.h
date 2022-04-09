#pragma once
#include <vector>
#include <utility>
#include "nodes/base.h"
#include "psc/array.h"

class ArrayDeclareNode : public Node {
private:
    const std::string name;
    const PSC::DataType type;
    std::vector<Node*> bounds;

public:
    ArrayDeclareNode(const Token &token, const std::string &name, PSC::DataType type, std::vector<Node*> &&bounds);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class ArrayAccessNode : public Node {
private:
    std::vector<Node*> indices;

public:
    ArrayAccessNode(const Token &token, std::vector<Node*> &&indices);

    std::pair<PSC::Value*, PSC::DataType> getValue(PSC::Context &ctx);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class ArrayAssignNode : public UnaryNode {
private:
    std::string arrayName;
    std::vector<Node*> indices;

public:
    ArrayAssignNode(const Token &token, Node &node, const std::string &arrayName, std::vector<Node*> &&indices);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
