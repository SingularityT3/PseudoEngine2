#pragma once
#include "nodes/base.h"

class CastNode : public UnaryNode {
private:
    PSC::DataType target;

public:
    CastNode(const Token &token, Node &node, PSC::DataType target);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
