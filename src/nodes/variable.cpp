#include "psc/error.h"
#include "nodes/variable.h"

DeclareNode::DeclareNode(const Token &token, const Token &identifier, PSC::DataType type)
    : Node(token), identifier(identifier), type(type)
{}

std::unique_ptr<NodeResult> DeclareNode::evaluate(PSC::Context &ctx) {
    if (ctx.getVariable(identifier.value) != nullptr)
        throw PSC::RedeclarationError(token, ctx, identifier.value);

    ctx.addVariable(new PSC::Variable(identifier.value, type, false));

    return std::make_unique<NodeResult>(nullptr, PSC::DT_NONE);
}


ConstDeclareNode::ConstDeclareNode(const Token &token, Node &node, const Token &identifier)
    : UnaryNode(token, node), identifier(identifier)
{}

std::unique_ptr<NodeResult> ConstDeclareNode::evaluate(PSC::Context &ctx) {
    auto value = node.evaluate(ctx);

    if (ctx.getVariable(identifier.value) != nullptr)
        throw PSC::RedeclarationError(token, ctx, identifier.value);

    ctx.addVariable(new PSC::Variable(identifier.value, value->type, true, value->data.get()));

    return std::make_unique<NodeResult>(nullptr, PSC::DT_NONE);
}


AssignNode::AssignNode(const Token &token, Node &node, const Token &identifier)
    : UnaryNode(token, node), identifier(identifier)
{}

std::unique_ptr<NodeResult> AssignNode::evaluate(PSC::Context &ctx) {
    auto valueRes = node.evaluate(ctx);

    PSC::Variable *var = ctx.getVariable(identifier.value);
    if (var == nullptr) {
        var = new PSC::Variable(identifier.value, valueRes->type, false);
        ctx.addVariable(var);
    }

    if (var->isConstant)
        throw PSC::ConstAssignError(token, ctx, var->name);

    PSC::InvalidUsageError err(token, ctx, "assignment operator: incompatible data types");

    // TODO: casting
    if (var->type != valueRes->type)
        throw err;

    switch (var->type) {
        case PSC::DT_INTEGER:
            var->get<PSC::Integer>() = valueRes->get<PSC::Integer>();
            break;
        case PSC::DT_REAL:
            var->get<PSC::Real>() = valueRes->get<PSC::Real>();
            break;
        case PSC::DT_BOOLEAN:
            var->get<PSC::Boolean>() = valueRes->get<PSC::Boolean>();
            break;
        case PSC::DT_CHAR:
            var->get<PSC::Char>() = valueRes->get<PSC::Char>();
            break;
        case PSC::DT_STRING:
            var->get<PSC::String>() = valueRes->get<PSC::String>();
            break;
        default:
            throw 0;
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DT_NONE);
}


std::unique_ptr<NodeResult> AccessNode::evaluate(PSC::Context &ctx) {
    PSC::Variable *var = ctx.getVariable(token.value);

    if (var == nullptr)
        throw PSC::NotDefinedError(token, ctx, "Identifier '" + token.value + "'");

    // TODO: Optimize to avoid copying
    PSC::Value *data;
    switch (var->type) {
        case PSC::DT_INTEGER:
            data = new PSC::Integer(var->get<PSC::Integer>());
            break;
        case PSC::DT_REAL:
            data = new PSC::Real(var->get<PSC::Real>());
            break;
        case PSC::DT_BOOLEAN:
            data = new PSC::Boolean(var->get<PSC::Boolean>());
            break;
        case PSC::DT_CHAR:
            data = new PSC::Char(var->get<PSC::Char>());
            break;
        case PSC::DT_STRING:
            data = new PSC::String(var->get<PSC::String>());
            break;
        default:
            throw 0;
    }

    return std::make_unique<NodeResult>(data, var->type);
}
