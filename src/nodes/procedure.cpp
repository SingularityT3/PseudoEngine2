#include "psc/error.h"
#include "psc/block.h"
#include "nodes/variable.h"
#include "nodes/procedure.h"

ProcedureNode::ProcedureNode(const Token &token, PSC::Procedure *procedure)
    : Node(token), procedure(procedure)
{}

ProcedureNode::~ProcedureNode() {
    if (!defined) delete procedure;
}

std::unique_ptr<NodeResult> ProcedureNode::evaluate(PSC::Context &ctx) {
    if (ctx.getProcedure(procedure->name) != nullptr)
        throw PSC::RedeclarationError(token, ctx, procedure->name);

    ctx.addProcedure(procedure);
    defined = true;

    return std::make_unique<NodeResult>(nullptr, PSC::DT_NONE);
}


CallNode::CallNode(const Token &token, const std::string &procedureName)
    : Node(token), procedureName(procedureName)
{}

std::unique_ptr<NodeResult> CallNode::evaluate(PSC::Context &ctx) {
    PSC::Procedure *procedure = ctx.getProcedure(procedureName);

    if (procedure == nullptr)
        throw PSC::NotDefinedError(token, ctx, "Procedure '" + procedureName + "'");

    int nArgs = procedure->parameters.size();
    if ((int) args.size() != nArgs)
        throw PSC::InvalidArgsError(token, ctx);

    auto procedureCtx = std::make_unique<PSC::Context>(&ctx, procedureName);
    ctx.switchToken = &token;

    for (int i = 0; i < (int) args.size(); i++) {
        std::unique_ptr<NodeResult> argRes = args[i]->evaluate(ctx);

        if (procedure->parameters[i].type != argRes->type)
            throw PSC::InvalidArgsError(token, ctx);

        PSC::Variable *var;
        if (procedure->byRef) {
            AccessNode *accsNode = dynamic_cast<AccessNode*>(args[i]);
            if (!accsNode)
                throw PSC::RuntimeError(token, ctx, "Only variables can be used as arguements when passing by reference");

            PSC::Variable *original = ctx.getVariable(accsNode->getToken().value);
            if (original == nullptr) throw 0;
            var = original->createReference(procedure->parameters[i].name);
        } else {
            var = new PSC::Variable(procedure->parameters[i].name, argRes->type, false);

            switch (var->type) {
                case PSC::DT_INTEGER:
                    var->get<PSC::Integer>() = argRes->get<PSC::Integer>();
                    break;
                case PSC::DT_REAL:
                    var->get<PSC::Real>() = argRes->get<PSC::Real>();
                    break;
                case PSC::DT_BOOLEAN:
                    var->get<PSC::Boolean>() = argRes->get<PSC::Boolean>();
                    break;
                case PSC::DT_CHAR:
                    var->get<PSC::Char>() = argRes->get<PSC::Char>();
                    break;
                case PSC::DT_STRING:
                    var->get<PSC::String>() = argRes->get<PSC::String>();
                    break;
                default:
                    throw 0;
            }
        }

        procedureCtx->addVariable(var);
    }

    procedure->block->run(*procedureCtx);
    ctx.switchToken = nullptr;

    return std::make_unique<NodeResult>(nullptr, PSC::DT_NONE);
}
