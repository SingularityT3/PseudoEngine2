#pragma once
#include "nodes/base.h"
#include "psc/scope/block.h"

class WhileLoopNode : public UnaryNode {
private:
    PSC::Block &block;

public:
    WhileLoopNode(const Token &token, Node &condition, PSC::Block &block);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
