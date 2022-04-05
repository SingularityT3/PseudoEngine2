#pragma once
#include "nodes/base.h"
#include "psc/block.h"

class RepeatUntilNode : public UnaryNode {
private:
    PSC::Block &block;

public:
    RepeatUntilNode(const Token &token, Node &condition, PSC::Block &block);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
