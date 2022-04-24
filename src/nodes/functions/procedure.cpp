#include "psc/error.h"
#include "psc/scope/block.h"
#include "nodes/variable.h"
#include "nodes/array.h"
#include "nodes/functions/procedure.h"

ProcedureNode::ProcedureNode(const Token &token, PSC::Procedure *procedure)
    : Node(token), procedure(procedure)
{}

std::unique_ptr<NodeResult> ProcedureNode::evaluate(PSC::Context &ctx) {
    if (ctx.getProcedure(procedure->name) != nullptr)
        throw PSC::RedeclarationError(token, ctx, procedure->name);

    ctx.addProcedure(std::move(procedure));

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


CallNode::CallNode(const Token &token, const std::string &procedureName, std::vector<Node*> &&args)
    : Node(token), procedureName(procedureName), args(std::move(args))
{}

std::unique_ptr<NodeResult> CallNode::evaluate(PSC::Context &ctx) {
    PSC::Procedure *procedure = ctx.getProcedure(procedureName);

    if (procedure == nullptr)
        throw PSC::NotDefinedError(token, ctx, "Procedure '" + procedureName + "'");

    size_t nArgs = procedure->parameters.size();
    if (args.size() != nArgs)
        throw PSC::InvalidArgsError(token, ctx);

    auto procedureCtx = std::make_unique<PSC::Context>(&ctx, procedureName);
    ctx.switchToken = &token;

    for (size_t i = 0; i < args.size(); i++) {
        auto argRes = args[i]->evaluate(ctx);

        if (!procedure->byRef) argRes->implicitCast(procedure->parameters[i].type);
        if (procedure->parameters[i].type != argRes->type)
            throw PSC::InvalidArgsError(token, ctx);

        PSC::Variable *var;
        if (procedure->byRef) {
            AccessNode *accsNode = dynamic_cast<AccessNode*>(args[i]);
            ArrayAccessNode *arrAccsNode = dynamic_cast<ArrayAccessNode*>(args[i]);
            if (!accsNode && !arrAccsNode)
                throw PSC::RuntimeError(token, ctx, "Only variables and array elements can be used as arguements when passing by reference");

            if (accsNode) {
                PSC::Variable *original = ctx.getVariable(accsNode->getToken().value);
                if (original == nullptr) std::abort();
                var = original->createReference(procedure->parameters[i].name);
            } else {
                var = PSC::Variable::createArrayElementReference(procedure->parameters[i].name, argRes->type, arrAccsNode->getValue(ctx).first);
            }
        } else {
            var = new PSC::Variable(procedure->parameters[i].name, argRes->type, false);

            switch (var->type) {
                case PSC::DataType::INTEGER:
                    var->get<PSC::Integer>() = argRes->get<PSC::Integer>();
                    break;
                case PSC::DataType::REAL:
                    var->get<PSC::Real>() = argRes->get<PSC::Real>();
                    break;
                case PSC::DataType::BOOLEAN:
                    var->get<PSC::Boolean>() = argRes->get<PSC::Boolean>();
                    break;
                case PSC::DataType::CHAR:
                    var->get<PSC::Char>() = argRes->get<PSC::Char>();
                    break;
                case PSC::DataType::STRING:
                    var->get<PSC::String>() = argRes->get<PSC::String>();
                    break;
                default:
                    std::abort();
            }
        }

        procedureCtx->addVariable(var);
    }

    procedure->run(*procedureCtx);
    ctx.switchToken = nullptr;

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
