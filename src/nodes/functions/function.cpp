#include "psc/error.h"
#include "psc/scope/block.h"
#include "nodes/variable.h"
#include "nodes/functions/function.h"

FunctionNode::FunctionNode(const Token &token, PSC::Function *function)
    : Node(token), function(function)
{}

std::unique_ptr<NodeResult> FunctionNode::evaluate(PSC::Context &ctx) {
    if (ctx.getFunction(function->name) != nullptr)
        throw PSC::RedeclarationError(token, ctx, function->name);

    ctx.addFunction(std::move(function));

    return std::make_unique<NodeResult>(nullptr, PSC::DT_NONE);
}


FunctionCallNode::FunctionCallNode(const Token &token)
    : Node(token), functionName(token.value)
{}

std::unique_ptr<NodeResult> FunctionCallNode::evaluate(PSC::Context &ctx) {
    PSC::Function *function = ctx.getFunction(functionName);

    if (function == nullptr)
        throw PSC::NotDefinedError(token, ctx, "Function '" + functionName + "'");

    int nArgs = function->parameters.size();
    if ((int) args.size() != nArgs)
        throw PSC::InvalidArgsError(token, ctx);

    auto functionCtx = std::make_unique<PSC::Context>(&ctx, functionName, true, function->returnType);
    ctx.switchToken = &token;

    for (int i = 0; i < (int) args.size(); i++) {
        std::unique_ptr<NodeResult> argRes = args[i]->evaluate(ctx);

        if (function->parameters[i].type != argRes->type) {
            throw PSC::InvalidArgsError(token, ctx);
        }

        PSC::Variable *var;
        if (function->byRef) {
            AccessNode *accsNode = dynamic_cast<AccessNode*>(args[i]);
            if (!accsNode) {
                throw PSC::RuntimeError(token, ctx, "Only variables can be used as arguements when passing by reference");
            }

            PSC::Variable *original = ctx.getVariable(accsNode->getToken().value);
            if (original == nullptr) std::abort();
            var = original->createReference(function->parameters[i].name);
        } else {
            var = new PSC::Variable(function->parameters[i].name, argRes->type, false);

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
                    std::abort();
            }
        }

        functionCtx->addVariable(var);
    }

    try {
        function->run(*functionCtx);
    } catch (ReturnErrSignal &e) {}

    if (!functionCtx->returnValue)
        throw PSC::RuntimeError(*(function->defToken), *functionCtx, "Missing RETURN statement");

    ctx.switchToken = nullptr;

    return std::move(functionCtx->returnValue);
}

std::unique_ptr<NodeResult> ReturnNode::evaluate(PSC::Context &ctx) {
    if (!ctx.isFunctionCtx) 
        throw PSC::InvalidUsageError(token, ctx, "RETURN statement");

    ctx.returnValue = node.evaluate(ctx);

    if (ctx.returnValue->type != ctx.returnType)
        throw PSC::RuntimeError(token, ctx, "Invalid return type");

    throw ReturnErrSignal();
}
