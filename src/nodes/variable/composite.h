#pragma once
#include "nodes/base.h"
#include "psc/scope/block.h"

class CompositeDefineNode : public Node {
private:
    const Token &name;
    PSC::Block &initBlock;

public:
    CompositeDefineNode(const Token &token, const Token &name, PSC::Block &initBlock);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
