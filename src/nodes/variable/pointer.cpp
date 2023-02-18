#include "pch.h"

#include "psc/error.h"
#include "nodes/variable/variable.h"
#include "nodes/variable/pointer.h"

PointerDefineNode::PointerDefineNode(const Token &token, const Token &name, const Token &type)
    : Node(token), name(name), type(type) {}

std::unique_ptr<NodeResult> PointerDefineNode::evaluate(PSC::Context &ctx) {
    PSC::DataType pointerType = getType(type);
    if (pointerType == PSC::DataType::NONE)
        throw PSC::NotDefinedError(token, ctx, "Type '" + type.value + "'");

    if (isIdentifierType(name))
        throw PSC::RuntimeError(token, ctx, "Redefinition of type '" + name.value + "'");

    PSC::PointerTypeDefinition definition(name.value, pointerType);
    PSC::TypeDefinitions::createPointerDefinition(std::move(definition));
    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}

PointerAssignNode::PointerAssignNode(
    const Token &token,
    std::unique_ptr<AbstractVariableResolver> &&pointerResolver,
    std::unique_ptr<AbstractVariableResolver> &&valueResolver
) : Node(token),
    pointerResolver(std::move(pointerResolver)),
    valueResolver(std::move(valueResolver))
{}

std::unique_ptr<NodeResult> PointerAssignNode::evaluate(PSC::Context &ctx) {
    auto &pointerHolder = pointerResolver->resolve(ctx);
    if (pointerHolder.isArray())
        throw PSC::RuntimeError(token, ctx, "Expected indices for array");

    auto &valueHolder = valueResolver->resolve(ctx);
    if (valueHolder.isArray())
        throw PSC::RuntimeError(token, ctx, "Cannot store pointer to array");
    
    auto &pointerVar = *static_cast<PSC::Variable*>(&pointerHolder);
    auto &value = *static_cast<PSC::Variable*>(&valueHolder);

    if (pointerVar.type.type != PSC::DataType::POINTER)
        throw PSC::RuntimeError(token, ctx, "Cannot assign pointer to variable not of type pointer");

    auto &pointer = pointerVar.get<PSC::Pointer>();
    auto &pointerType = pointer.getDefinition().type;
    if (pointerType != value.type)
        throw PSC::RuntimeError(token, ctx, "Assignment Error: Incompatible data types");
    
    pointer.setValue(&value);

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
