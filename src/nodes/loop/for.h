#pragma once
#include "nodes/base.h"
#include "psc/scope/block.h"

class ForLoopNode : public Node {
private:
    const Token &identifier;
    Node &start, &stop, *step;
    PSC::Block *block;

public:
    ForLoopNode(const Token &token, const Token &identifier, Node &start, Node &stop, Node *step, PSC::Block *block);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
