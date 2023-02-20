#pragma once
#include <string>
#include <vector>
#include "nodes/base.h"
#include "psc/scope/block.h"

class ProcedureNode : public Node {
private:
    const std::string procedureName;
    const std::vector<std::string> parameterNames;
    const std::vector<const Token*> parameterTypes;
    const bool byRef;
    PSC::Block &block;

public:
    ProcedureNode(
        const Token &token,
        const std::string &procedureName,
        std::vector<std::string> &&parameterNames,
        std::vector<const Token*> &&parameterTypes,
        bool byRef,
        PSC::Block &block
    );

    // Adds procedure to ctx
    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class CallNode : public Node {
private:
    const std::string procedureName;
    const std::vector<Node*> args;

public:
    CallNode(const Token &token, const std::string &procedureName, std::vector<Node*> &&args);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
