#pragma once
#include "nodes/base.h"

class BreakErrSignal {
public:
    const Token &token;

    BreakErrSignal(const Token &token);
};

class ContinueErrSignal {
public:
    const Token &token;

    ContinueErrSignal(const Token &token);
};

class BreakNode : public Node {
public:
    using Node::Node;

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class ContinueNode : public Node {
public:
    using Node::Node;

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
