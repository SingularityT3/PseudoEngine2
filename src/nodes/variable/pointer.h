#pragma once
#include "nodes/base.h"
#include "nodes/variable/resolver.h"

class PointerDefineNode : public Node {
private:
    const Token &name;
    const Token &type;

public:
    PointerDefineNode(const Token &token, const Token &name, const Token &type);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class PointerAssignNode : public Node {
private:
    std::unique_ptr<AbstractVariableResolver> pointerResolver, valueResolver;

public:
    PointerAssignNode(
        const Token &token,
        std::unique_ptr<AbstractVariableResolver> &&pointerResolver,
        std::unique_ptr<AbstractVariableResolver> &&valueResolver
    );

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
