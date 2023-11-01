#include "pch.h"

#include "psc/error.h"
#include "nodes/variable/resolver.h"

inline PSC::DataHolder *ResolverCache::get(PSC::Context &ctx) {
    return &ctx == cacheCtx ? cached : nullptr;
}

void ResolverCache::set(PSC::DataHolder *holder, PSC::Context &ctx) {
    cacheCtx = &ctx;
    cached = holder;
    ctx.addCache(this);
}

void ResolverCache::clear() {
    cached = nullptr;
    cacheCtx = nullptr;
}

AbstractVariableResolver::AbstractVariableResolver(const Token &token)
    : token(token) {}

bool AbstractVariableResolver::getDynamic() {
    return isDynamic;
}

PointerDereferencer::PointerDereferencer(const Token &token, std::unique_ptr<AbstractVariableResolver> &&resolver)
    : AbstractVariableResolver(token), resolver(std::move(resolver))
{
    isDynamic = this->resolver->getDynamic();
}

PSC::DataHolder &PointerDereferencer::resolve(PSC::Context &ctx) {
    PSC::Variable *ptrVar;

    PSC::DataHolder *cached;
    if (isDynamic && (cached = cache.get(ctx)) != nullptr) {
        ptrVar = (PSC::Variable*) cached;
    } else {
        auto &dh = resolver->resolve(ctx);
        if (dh.isArray())
            throw PSC::InvalidUsageError(token, ctx, "'^' operator: Attempting to dereference non-pointer");

        auto &var = *static_cast<PSC::Variable*>(&dh);
        if (var.type != PSC::DataType::POINTER)
            throw PSC::InvalidUsageError(token, ctx, "'^' operator: Attempting to dereference non-pointer");

        ptrVar = &var;
        if (isDynamic) cache.set(ptrVar, ctx);
    }

    PSC::Pointer &ptr = ptrVar->get<PSC::Pointer>();
    const PSC::Context *ptrCtx = ptr.getCtx();
    PSC::Context *tempCtx = &ctx;
    while (tempCtx != ptrCtx) {
        tempCtx = tempCtx->getParent();
        if (tempCtx == nullptr)
            throw PSC::RuntimeError(token, ctx, "Attempting to access deleted object from pointer '" + ptrVar->name + "'");
    }

    PSC::Variable *ptrRefVar = ptr.getValue();
    if (ptrRefVar == nullptr)
        throw PSC::RuntimeError(token, ctx, "Attempt to access uninitalized pointer");

    return *ptrRefVar;
}

CompositeResolver::CompositeResolver(const Token &token, std::unique_ptr<AbstractVariableResolver> &&resolver, const Token &member)
    : AbstractVariableResolver(token),
    resolver(std::move(resolver)),
    member(member)
{
    isDynamic = this->resolver->getDynamic();
}

PSC::DataHolder &CompositeResolver::resolve(PSC::Context &ctx) {
    PSC::DataHolder *cached;
    if (isDynamic && (cached = cache.get(ctx)) != nullptr) return *cached;

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
    
    if (isDynamic) cache.set(memberPtr, ctx);
    return *memberPtr;
}

ArrayElementResolver::ArrayElementResolver(const Token &token, std::unique_ptr<AbstractVariableResolver> &&resolver, std::vector<Node*> &&indices)
    : AbstractVariableResolver(token),
    indices(std::move(indices)),
    resolver(std::move(resolver))
{
    isDynamic = false;
    for (Node* node : indices) {
        if (node->getToken().type != TokenType::INTEGER) {
            isDynamic = true;
            break;
        }
    }
}

PSC::DataHolder &ArrayElementResolver::resolve(PSC::Context &ctx) {
    PSC::Array *arr;

    PSC::DataHolder *cached;
    if (isDynamic && (cached = cache.get(ctx)) != nullptr) {
        arr = (PSC::Array*) cached;
    } else {
        auto &dh = resolver->resolve(ctx);
        if (!dh.isArray())
            throw PSC::RuntimeError(token, ctx, "Attempting to index non-array variable '" + dh.name + "'");

        arr = static_cast<PSC::Array*>(&dh);
        if (isDynamic) cache.set(arr, ctx);
    }

    if (indices.size() != arr->dimensions.size())
        throw PSC::RuntimeError(token, ctx, "Invalid number of indices");

    std::vector<PSC::int_t> evaluatedIndices;
    evaluatedIndices.reserve(indices.size());

    for (size_t i = 0; i < indices.size(); i++) {
        Node *index = indices[i];
        auto result = index->evaluate(ctx);

        if (result->type != PSC::DataType::INTEGER)
            throw PSC::RuntimeError(index->getToken(), ctx, "Array indices must be of type INTEGER");

        PSC::int_t x = result->get<PSC::Integer>().value;
        if (!arr->dimensions[i].isValidIndex(x))
            throw PSC::RuntimeError(index->getToken(), ctx, "Index out of bounds");

        evaluatedIndices.push_back(x);
    }

    PSC::Variable &var = arr->getElement(evaluatedIndices);
    return var;
}

SimpleVariableSource::SimpleVariableSource(const Token &token)
    : AbstractVariableResolver(token)
{
    isDynamic = false;
}

PSC::DataHolder &SimpleVariableSource::resolve(PSC::Context &ctx) {
    PSC::DataHolder *cached = cache.get(ctx);
    if (cached != nullptr) return *cached;

    PSC::Variable *var = ctx.getVariable(token.value);
    if (var != nullptr) {
        cache.set(var, ctx);
        return *var;
    }

    PSC::Array *arr = ctx.getArray(token.value);
    if (arr != nullptr) {
        cache.set(arr, ctx);
        return *arr;
    }

    throw PSC::NotDefinedError(token, ctx, "Identifier '" + token.value + "'");
}

const std::string &SimpleVariableSource::getName() const {
    return token.value;
}

const Token &SimpleVariableSource::getToken() const {
    return token;
}
