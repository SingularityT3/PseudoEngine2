#include "pch.h"

#include "psc/error.h"
#include "nodes/variable/variable.h"

PSC::DataType getType(const Token &token) {
    if (token.type == TokenType::DATA_TYPE) {
        if (token.value == "INTEGER") return PSC::DataType(PSC::DataType::INTEGER);
        else if (token.value == "REAL") return PSC::DataType(PSC::DataType::REAL);
        else if (token.value == "BOOLEAN") return PSC::DataType(PSC::DataType::BOOLEAN);
        else if (token.value == "CHAR") return PSC::DataType(PSC::DataType::CHAR);
        else if (token.value == "STRING") return PSC::DataType(PSC::DataType::STRING);
        else std::abort();
    } else if (token.type == TokenType::IDENTIFIER) {
        auto *enumDefinition = PSC::TypeDefinitions::getEnumDefinition(token.value);
        if (enumDefinition != nullptr)
            return PSC::DataType(PSC::DataType::ENUM, &enumDefinition->name);
        
        auto *pointerDefinition = PSC::TypeDefinitions::getPointerDefinition(token.value);
        if (pointerDefinition != nullptr)
            return PSC::DataType(PSC::DataType::POINTER, &pointerDefinition->name);
        
        auto *compositeDefinition = PSC::TypeDefinitions::getCompositeDefinition(token.value);
        if (compositeDefinition != nullptr)
            return PSC::DataType(PSC::DataType::COMPOSITE, &compositeDefinition->name);
        
        return PSC::DataType(PSC::DataType::NONE);
    }
    std::abort();
}

bool isIdentifierType(const Token &token) {
    PSC::DataType dataType = getType(token);
    if (dataType != PSC::DataType::NONE)
        return true;

    std::unique_ptr<PSC::Enum> enumEl(PSC::TypeDefinitions::isEnumElement(token.value));
    if (enumEl.get() != nullptr)
        return true;

    return false;
}


DeclareNode::DeclareNode(const Token &token, std::vector<const Token*> &&identifiers, const Token &type)
    : Node(token), identifiers(std::move(identifiers)), type(type)
{}

std::unique_ptr<NodeResult> DeclareNode::evaluate(PSC::Context &ctx) {
    for (auto identifier : identifiers) {
        if (ctx.getVariable(identifier->value) != nullptr)
            throw PSC::RedeclarationError(token, ctx, identifier->value);

        if (isIdentifierType(*identifier))
            throw PSC::RuntimeError(token, ctx, "Redefinition of type '" + identifier->value + "' as variable");

        PSC::DataType dataType = getType(type);
        if (dataType == PSC::DataType::NONE)
            throw PSC::NotDefinedError(token, ctx, "Type '" + type.value + "'");

        ctx.addVariable(new PSC::Variable(identifier->value, dataType, false));
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


ConstDeclareNode::ConstDeclareNode(const Token &token, Node &node, const Token &identifier)
    : UnaryNode(token, node), identifier(identifier)
{}

std::unique_ptr<NodeResult> ConstDeclareNode::evaluate(PSC::Context &ctx) {
    auto value = node.evaluate(ctx);

    if (ctx.getVariable(identifier.value) != nullptr)
        throw PSC::RedeclarationError(token, ctx, identifier.value);

    ctx.addVariable(new PSC::Variable(identifier.value, value->type, true, value->data.get()));

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


AssignNode::AssignNode(const Token &token, Node &node, std::unique_ptr<AbstractVariableResolver> &&resolver)
    : UnaryNode(token, node), resolver(std::move(resolver))
{}

std::unique_ptr<NodeResult> AssignNode::evaluate(PSC::Context &ctx) {
    auto valueRes = node.evaluate(ctx);

    PSC::Variable *var;
    try {
        PSC::DataHolder &holder = resolver->resolve(ctx);
        if (holder.isArray())
            throw PSC::RuntimeError(token, ctx, "Expected indices for array");

        var = static_cast<PSC::Variable*>(&holder);
    } catch (PSC::NotDefinedError &e) {
        const SimpleVariableSource *simpleSource = dynamic_cast<const SimpleVariableSource*>(resolver.get());
        if (simpleSource == nullptr) throw e;
        if (isIdentifierType(simpleSource->getToken())) throw e;

        var = new PSC::Variable(simpleSource->getName(), valueRes->type, false);
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
        auto def = PSC::TypeDefinitions::isEnumElement(token.value);
        if (def != nullptr)
            return std::make_unique<NodeResult>(def, PSC::DataType(PSC::DataType::ENUM, &def->definitionName));
        throw e;
    }

    if (holder->isArray())
        throw PSC::RuntimeError(token, ctx, "Expected indices for array");
    
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
