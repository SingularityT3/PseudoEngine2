#pragma once
#include <vector>
#include <utility>
#include "nodes/base.h"
#include "psc/array.h"

class ArrayDeclareNode : public Node {
private:
    const std::vector<const Token*> identifiers;
    const Token &type;
    std::vector<Node*> bounds;

public:
    ArrayDeclareNode(const Token &token, std::vector<const Token*> &&identifiers, const Token &type, std::vector<Node*> &&bounds);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
