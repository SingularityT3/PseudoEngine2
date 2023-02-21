#include "pch.h"

#include "psc/error.h"
#include "nodes/variable/resolver.h"


AbstractVariableResolver::AbstractVariableResolver(const Token &token)
    : token(token) {}

PointerDereferencer::PointerDereferencer(const Token &token, std::unique_ptr<AbstractVariableResolver> &&resolver)
    : AbstractVariableResolver(token), resolver(std::move(resolver)) {}

PSC::DataHolder &PointerDereferencer::resolve(PSC::Context &ctx) const {
    auto &dh = resolver->resolve(ctx);
    if (dh.isArray())
        throw PSC::InvalidUsageError(token, ctx, "'^' operator: Attempting to dereference non-pointer");

    auto &var = *static_cast<PSC::Variable*>(&dh);
    if (var.type != PSC::DataType::POINTER)
        throw PSC::InvalidUsageError(token, ctx, "'^' operator: Attempting to dereference non-pointer");

    PSC::Pointer &ptr = var.get<PSC::Pointer>();
    const PSC::Context *ptrCtx = ptr.getCtx();
    PSC::Context *tempCtx = &ctx;
    while (tempCtx != ptrCtx) {
        tempCtx = tempCtx->getParent();
        if (tempCtx == nullptr)
            throw PSC::RuntimeError(token, ctx, "Attempting to access deleted object from pointer '" + var.name + "'");
    }

    PSC::Variable *ptrVar = ptr.getValue();
    if (ptrVar == nullptr)
        throw PSC::RuntimeError(token, ctx, "Attempt to access uninitalized pointer");

    return *ptrVar;
}

CompositeResolver::CompositeResolver(const Token &token, std::unique_ptr<AbstractVariableResolver> &&resolver, const Token &member)
    : AbstractVariableResolver(token),
    resolver(std::move(resolver)),
    member(member)
{}

PSC::DataHolder &CompositeResolver::resolve(PSC::Context &ctx) const {
    auto &dh = resolver->resolve(ctx);
    if (dh.isArray())
        throw PSC::InvalidUsageError(token, ctx, "'.' operator: Variable is not a composite type");

    auto &var = *static_cast<PSC::Variable*>(&dh);
    if (var.type != PSC::DataType::COMPOSITE)
        throw PSC::InvalidUsageError(token, ctx, "'.' operator: Variable is not a composite type");
    
    auto &composite = var.get<PSC::Composite>();
    auto *memberPtr = composite.getMember(member.value);
    if (memberPtr == nullptr)
        throw PSC::RuntimeError(token, ctx, "Type '" + composite.definitionName + "' has no member '" + member.value + "'");
    
    return *memberPtr;
}

ArrayElementResolver::ArrayElementResolver(const Token &token, std::unique_ptr<AbstractVariableResolver> &&resolver, std::vector<Node*> &&indices)
    : AbstractVariableResolver(token),
    indices(std::move(indices)),
    resolver(std::move(resolver))
{}

PSC::DataHolder &ArrayElementResolver::resolve(PSC::Context &ctx) const {
    auto &dh = resolver->resolve(ctx);
    if (!dh.isArray())
        throw PSC::RuntimeError(token, ctx, "Attempting to index non-array variable '" + dh.name + "'");

    PSC::Array &arr = *static_cast<PSC::Array*>(&dh);

    if (indices.size() != arr.dimensions.size())
        throw PSC::RuntimeError(token, ctx, "Invalid number of indices");

    std::vector<PSC::int_t> evaluatedIndices;
    evaluatedIndices.reserve(indices.size());

    for (size_t i = 0; i < indices.size(); i++) {
        Node *index = indices[i];
        auto result = index->evaluate(ctx);

        if (result->type != PSC::DataType::INTEGER)
            throw PSC::RuntimeError(index->getToken(), ctx, "Array indices must be of type INTEGER");

        PSC::int_t x = result->get<PSC::Integer>().value;
        if (!arr.dimensions[i].isValidIndex(x))
            throw PSC::RuntimeError(index->getToken(), ctx, "Index out of bounds");

        evaluatedIndices.push_back(x);
    }

    PSC::Variable &var = arr.getElement(evaluatedIndices);
    return var;
}

PSC::DataHolder &SimpleVariableSource::resolve(PSC::Context &ctx) const {
    PSC::Variable *var = ctx.getVariable(token.value);
    if (var != nullptr) return *var;

    PSC::Array *arr = ctx.getArray(token.value);
    if (arr != nullptr)
        return *arr;

    throw PSC::NotDefinedError(token, ctx, "Identifier '" + token.value + "'");
}

const std::string &SimpleVariableSource::getName() const {
    return token.value;
}

const Token &SimpleVariableSource::getToken() const {
    return token;
}
