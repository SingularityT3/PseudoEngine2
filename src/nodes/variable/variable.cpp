#include "pch.h"

#include "psc/error.h"
#include "nodes/variable/variable.h"

DeclareNode::DeclareNode(const Token &token, std::vector<const Token*> &&identifiers, const Token &type)
    : Node(token), identifiers(std::move(identifiers)), type(type)
{}

std::unique_ptr<NodeResult> DeclareNode::evaluate(PSC::Context &ctx) {
    for (auto identifier : identifiers) {
        if (ctx.getVariable(identifier->value, false) != nullptr)
            throw PSC::RedeclarationError(token, ctx, identifier->value);

        if (ctx.isIdentifierType(*identifier))
            throw PSC::RuntimeError(token, ctx, "Redefinition of type '" + identifier->value + "' as variable");

        PSC::DataType dataType = ctx.getType(type);
        if (dataType == PSC::DataType::NONE)
            throw PSC::NotDefinedError(token, ctx, "Type '" + type.value + "'");

        ctx.addVariable(new PSC::Variable(identifier->value, dataType, false, &ctx));
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


ConstDeclareNode::ConstDeclareNode(const Token &token, Node &node, const Token &identifier)
    : UnaryNode(token, node), identifier(identifier)
{}

std::unique_ptr<NodeResult> ConstDeclareNode::evaluate(PSC::Context &ctx) {
    auto value = node.evaluate(ctx);

    if (ctx.getVariable(identifier.value, false) != nullptr)
        throw PSC::RedeclarationError(token, ctx, identifier.value);

    ctx.addVariable(new PSC::Variable(identifier.value, value->type, true, &ctx, value->data.get()));

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


AssignNode::AssignNode(const Token &token, Node &node, std::unique_ptr<AbstractVariableResolver> &&resolver)
    : UnaryNode(token, node), resolver(std::move(resolver))
{}

void AssignNode::assignArray(PSC::Context &ctx, const PSC::ArrayDirectAccessError &e) {
    AccessNode *accsNode = dynamic_cast<AccessNode*>(&node);
    if (accsNode == nullptr) throw e;
    auto array = static_cast<PSC::Array*>(&accsNode->resolver->resolve(ctx));
    
    PSC::DataHolder &holder = resolver->resolve(ctx);
    if (!holder.isArray()) throw e;
    
    PSC::Array *arr = static_cast<PSC::Array*>(&holder);
    if (arr->type != array->type)
        throw PSC::RuntimeError(token, ctx, "Cannot assign arrays of different data type");
    if (arr->dimensions != array->dimensions)
        throw PSC::RuntimeError(token, ctx, "Cannot assign arrays of different dimensions");

    arr->copyData(*array);
}

std::unique_ptr<NodeResult> AssignNode::evaluate(PSC::Context &ctx) {
    std::unique_ptr<NodeResult> valueRes;
    try {
        valueRes = node.evaluate(ctx);
    } catch (PSC::ArrayDirectAccessError &e) {
        if (&e.context != &ctx) throw e;
        assignArray(ctx, e);
        return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
    }

    PSC::Variable *var;
    try {
        PSC::DataHolder &holder = resolver->resolve(ctx);
        if (holder.isArray())
            throw PSC::ArrayDirectAccessError(token, ctx);

        var = static_cast<PSC::Variable*>(&holder);
    } catch (PSC::NotDefinedError &e) {
        const SimpleVariableSource *simpleSource = dynamic_cast<const SimpleVariableSource*>(resolver.get());
        if (simpleSource == nullptr) throw e;
        if (ctx.isIdentifierType(simpleSource->getToken())) throw e;

        var = new PSC::Variable(simpleSource->getName(), valueRes->type, false, &ctx);
        ctx.addVariable(var);
    }

    if (var->isConstant)
        throw PSC::ConstAssignError(token, ctx, var->name);

    valueRes->implicitCast(var->type);
    if (var->type != valueRes->type)
        throw PSC::InvalidUsageError(token, ctx, "assignment operator: incompatible data types");

    switch (var->type.type) {
        case PSC::DataType::INTEGER:
            var->get<PSC::Integer>() = valueRes->get<PSC::Integer>();
            break;
        case PSC::DataType::REAL:
            var->get<PSC::Real>() = valueRes->get<PSC::Real>();
            break;
        case PSC::DataType::BOOLEAN:
            var->get<PSC::Boolean>() = valueRes->get<PSC::Boolean>();
            break;
        case PSC::DataType::CHAR:
            var->get<PSC::Char>() = valueRes->get<PSC::Char>();
            break;
        case PSC::DataType::STRING:
            var->get<PSC::String>() = valueRes->get<PSC::String>();
            break;
        case PSC::DataType::ENUM:
            var->get<PSC::Enum>() = valueRes->get<PSC::Enum>();
            break;
        case PSC::DataType::POINTER:
            var->get<PSC::Pointer>() = valueRes->get<PSC::Pointer>();
            break;
        case PSC::DataType::COMPOSITE:
            var->get<PSC::Composite>() = valueRes->get<PSC::Composite>();
            break;
        case PSC::DataType::NONE:
            std::abort();
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


AccessNode::AccessNode(const Token &token, std::unique_ptr<AbstractVariableResolver> &&resolver)
    : Node(token), resolver(std::move(resolver)) {}

std::unique_ptr<NodeResult> AccessNode::evaluate(PSC::Context &ctx) {
    PSC::DataHolder *holder;
    try {
        holder = &resolver->resolve(ctx);
    } catch (PSC::NotDefinedError &e) {
        auto def = ctx.getEnumElement(token.value);
        if (def != nullptr)
            return std::make_unique<NodeResult>(def, PSC::DataType(PSC::DataType::ENUM, &def->definitionName));
        throw e;
    }

    if (holder->isArray())
        throw PSC::ArrayDirectAccessError(token, ctx);
    
    PSC::Variable &var = *static_cast<PSC::Variable*>(holder);

    PSC::Value *data;
    switch (var.type.type) {
        case PSC::DataType::INTEGER:
            data = new PSC::Integer(var.get<PSC::Integer>());
            break;
        case PSC::DataType::REAL:
            data = new PSC::Real(var.get<PSC::Real>());
            break;
        case PSC::DataType::BOOLEAN:
            data = new PSC::Boolean(var.get<PSC::Boolean>());
            break;
        case PSC::DataType::CHAR:
            data = new PSC::Char(var.get<PSC::Char>());
            break;
        case PSC::DataType::STRING:
            data = new PSC::String(var.get<PSC::String>());
            break;
        case PSC::DataType::ENUM:
            data = new PSC::Enum(var.get<PSC::Enum>());
            break;
        case PSC::DataType::POINTER:
            data = new PSC::Pointer(var.get<PSC::Pointer>());
            break;
        case PSC::DataType::COMPOSITE:
            data = new PSC::Composite(var.get<PSC::Composite>());
            break;
        case PSC::DataType::NONE:
            std::abort();
    }

    return std::make_unique<NodeResult>(data, var.type);
}

const AbstractVariableResolver &AccessNode::getResolver() const {
    return *resolver;
}
