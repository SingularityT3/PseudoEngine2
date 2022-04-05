#pragma once
#include "nodes/base.h"

class StringConcatenationNode : public BinaryNode {
public:
    using BinaryNode::BinaryNode;

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
