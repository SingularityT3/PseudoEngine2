#include "pch.h"

#include "psc/error.h"
#include "psc/scope/block.h"
#include "nodes/variable/variable.h"
#include "nodes/variable/array.h"
#include "nodes/functions/function.h"

FunctionNode::FunctionNode(
    const Token &token,
    const std::string &functionName,
    std::vector<std::string> &&parameterNames,
    std::vector<const Token*> &&parameterTypes,
    std::vector<bool> &&parameterPassTypes,
    PSC::Block &block,
    const Token &returnType
)
: Node(token),
    functionName(functionName),
    parameterNames(std::move(parameterNames)),
    parameterTypes(std::move(parameterTypes)),
    parameterPassTypes(std::move(parameterPassTypes)),
    block(block),
    returnType(returnType)
{}

std::unique_ptr<NodeResult> FunctionNode::evaluate(PSC::Context &ctx) {
    if (ctx.getFunction(functionName) != nullptr)
        throw PSC::RedefinitionError(token, ctx, functionName);

    PSC::DataType returnDataType = ctx.getType(returnType);
    if (returnDataType == PSC::DataType::NONE)
        throw PSC::TypeNotDefinedError(returnType, ctx, returnType.value);

    size_t parametersSize = parameterNames.size();
    std::vector<PSC::Parameter> parameters;
    parameters.reserve(parametersSize);
    for (size_t i = 0; i < parametersSize; i++) {
        const Token *typeToken = parameterTypes[i];
        PSC::DataType type = ctx.getType(*typeToken);
        if (type == PSC::DataType::NONE)
            throw PSC::TypeNotDefinedError(*typeToken, ctx, typeToken->value);
        parameters.emplace_back(parameterNames[i], type, parameterPassTypes[i]);
    }

    auto function = std::make_unique<PSC::Function>(functionName, std::move(parameters), &block, returnDataType, &token);
    ctx.addFunction(std::move(function));

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


FunctionCallNode::FunctionCallNode(const Token &token, std::vector<Node*> &&args)
    : Node(token), functionName(token.value), args(std::move(args))
{}

std::unique_ptr<NodeResult> FunctionCallNode::evaluate(PSC::Context &ctx) {
    PSC::Function *function = ctx.getFunction(functionName);

    if (function == nullptr)
        throw PSC::NotDefinedError(token, ctx, "Function '" + functionName + "'");

    std::vector<std::unique_ptr<NodeResult>> argResults;
    std::vector<PSC::DataType> argTypes;
    argResults.reserve(args.size());
    argTypes.reserve(args.size());

    for (size_t i = 0; i < args.size(); i++) {
        argResults.push_back(args[i]->evaluate(ctx));
        argTypes.push_back(argResults[i]->type);
    }

    size_t nArgs = function->parameters.size();
    if (args.size() != nArgs)
        throw PSC::InvalidArgsError(token, ctx, function->getTypes(), std::move(argTypes));

    auto functionCtx = std::make_unique<PSC::Context>(&ctx, functionName, true, function->returnType);
    ctx.switchToken = &token;

    for (size_t i = 0; i < args.size(); i++) {
        auto &argRes = argResults[i];
        const PSC::Parameter &parameter = function->parameters[i];

        if (!parameter.byRef) argRes->implicitCast(parameter.type);
        if (parameter.type != argRes->type) {
            throw PSC::InvalidArgsError(token, ctx, function->getTypes(), std::move(argTypes));
        }

        PSC::Variable *var;
        if (parameter.byRef) {
            AccessNode *accsNode = dynamic_cast<AccessNode*>(args[i]);
            if (!accsNode) {
                throw PSC::RuntimeError(token, ctx, "Only variables can be used as arguements when passing by reference");
            }

            auto &holder = accsNode->getResolver().resolve(ctx);
            if (holder.isArray())
                throw PSC::ArrayDirectAccessError(token, ctx);

            PSC::Variable &original = *static_cast<PSC::Variable*>(&holder);
            var = original.createReference(parameter.name);
        } else {
            var = new PSC::Variable(parameter.name, argRes->type, false, functionCtx.get());

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
                case PSC::DataType::DATE:
                    var->get<PSC::Date>() = argRes->get<PSC::Date>();
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

        functionCtx->addVariable(var);
    }

    try {
        function->run(*functionCtx);
    } catch (ReturnErrSignal&) {}

    if (!functionCtx->returnValue)
        throw PSC::RuntimeError(*(function->defToken), *functionCtx, "Missing RETURN statement");

    ctx.switchToken = nullptr;

    return std::move(functionCtx->returnValue);
}

std::unique_ptr<NodeResult> ReturnNode::evaluate(PSC::Context &ctx) {
    if (!ctx.isFunctionCtx) 
        throw PSC::InvalidUsageError(token, ctx, "RETURN statement");

    ctx.returnValue = node.evaluate(ctx);
    ctx.returnValue->implicitCast(ctx.returnType);

    if (ctx.returnValue->type != ctx.returnType)
        throw PSC::RuntimeError(token, ctx, "Invalid return type");

    throw ReturnErrSignal();
}
