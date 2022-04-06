#pragma once
#include "nodes/base.h"
#include "psc/procedure.h"

class ProcedureNode : public Node {
private:
    std::unique_ptr<PSC::Procedure> procedure;

public:
    ProcedureNode(const Token &token, PSC::Procedure *procedure);

    // Adds procedure to ctx
    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};

class CallNode : public Node {
private:
    const std::string procedureName;

public:
    std::vector<Node*> args;

    CallNode(const Token &token, const std::string &procedureName);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
