#include "pch.h"

#include "psc/error.h"
#include "psc/scope/block.h"
#include "nodes/variable/variable.h"
#include "nodes/variable/array.h"
#include "nodes/functions/procedure.h"

ProcedureNode::ProcedureNode(
    const Token &token,
    const std::string &procedureName,
    std::vector<std::string> &&parameterNames,
    std::vector<const Token*> &&parameterTypes,
    bool byRef,
    PSC::Block &block
)
: Node(token),
    procedureName(procedureName),
    parameterNames(std::move(parameterNames)),
    parameterTypes(std::move(parameterTypes)),
    byRef(byRef),
    block(block)
{}

std::unique_ptr<NodeResult> ProcedureNode::evaluate(PSC::Context &ctx) {
    if (ctx.getProcedure(procedureName) != nullptr)
        throw PSC::RedeclarationError(token, ctx, procedureName);

    size_t parametersSize = parameterNames.size();
    std::vector<PSC::Parameter> parameters;
    parameters.reserve(parametersSize);
    for (size_t i = 0; i < parametersSize; i++) {
        const Token *typeToken = parameterTypes[i];
        PSC::DataType type = ctx.getType(*typeToken);
        if (type == PSC::DataType::NONE)
            throw PSC::NotDefinedError(*typeToken, ctx, "Type '" + typeToken->value + "'");
        parameters.emplace_back(parameterNames[i], type);
    }

    auto procedure = std::make_unique<PSC::Procedure>(procedureName, std::move(parameters), byRef, &block);
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

    std::vector<std::unique_ptr<NodeResult>> argResults;
    std::vector<PSC::DataType> argTypes;
    argResults.reserve(args.size());
    argTypes.reserve(args.size());

    for (size_t i = 0; i < args.size(); i++) {
        argResults.push_back(args[i]->evaluate(ctx));
        argTypes.push_back(argResults[i]->type);
    }

    size_t nArgs = procedure->parameters.size();
    if (args.size() != nArgs)
        throw PSC::InvalidArgsError(token, ctx, procedure->getTypes(), std::move(argTypes));

    auto procedureCtx = std::make_unique<PSC::Context>(&ctx, procedureName);
    ctx.switchToken = &token;

    for (size_t i = 0; i < args.size(); i++) {
        auto &argRes = argResults[i];

        if (!procedure->byRef) argRes->implicitCast(procedure->parameters[i].type);
        if (procedure->parameters[i].type != argRes->type)
            throw PSC::InvalidArgsError(token, ctx, procedure->getTypes(), std::move(argTypes));

        PSC::Variable *var;
        if (procedure->byRef) {
            AccessNode *accsNode = dynamic_cast<AccessNode*>(args[i]);
            if (!accsNode)
                throw PSC::RuntimeError(token, ctx, "Only variables and array elements can be used as arguements when passing by reference");

            auto &holder = accsNode->getResolver().resolve(ctx);
            if (holder.isArray())
                throw PSC::RuntimeError(token, ctx, "Expected indices for array");

            PSC::Variable &original = *static_cast<PSC::Variable*>(&holder);
            var = original.createReference(procedure->parameters[i].name);
        } else {
            var = new PSC::Variable(procedure->parameters[i].name, argRes->type, false, &ctx);

            switch (var->type.type) {
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
                case PSC::DataType::ENUM:
                    var->get<PSC::Enum>() = argRes->get<PSC::Enum>();
                    break;
                case PSC::DataType::POINTER:
                    var->get<PSC::Pointer>() = argRes->get<PSC::Pointer>();
                    break;
                case PSC::DataType::COMPOSITE:
                    var->get<PSC::Composite>() = argRes->get<PSC::Composite>();
                    break;
                case PSC::DataType::NONE:
                    std::abort();
            }
        }

        procedureCtx->addVariable(var);
    }

    procedure->run(*procedureCtx);
    ctx.switchToken = nullptr;

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
