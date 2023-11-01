#pragma once
#include <memory>

#include "lexer/tokens.h"
#include "psc/variable.h"
#include "psc/scope/context.h"
#include "nodes/base.h"

class ResolverCache {
private:
    PSC::DataHolder *cached = nullptr;
    PSC::Context *cacheCtx = nullptr;

public:
    inline PSC::DataHolder *get(PSC::Context &ctx);

    void set(PSC::DataHolder *holder, PSC::Context &ctx);

    void clear();
};

class AbstractVariableResolver {
protected:
    const Token &token;
    ResolverCache cache;
    bool isDynamic;

public:
    AbstractVariableResolver(const Token &token);

    bool getDynamic();

    virtual ~AbstractVariableResolver() = default;

    virtual PSC::DataHolder &resolve(PSC::Context &ctx) = 0;
};

class PointerDereferencer : public AbstractVariableResolver {
private:
    std::unique_ptr<AbstractVariableResolver> resolver;

public:
    PointerDereferencer(const Token &token, std::unique_ptr<AbstractVariableResolver> &&resolver);

    PSC::DataHolder &resolve(PSC::Context &ctx) override;
};

class CompositeResolver : public AbstractVariableResolver {
private:
    std::unique_ptr<AbstractVariableResolver> resolver;
    const Token &member;

public:
    CompositeResolver(const Token &token, std::unique_ptr<AbstractVariableResolver> &&resolver, const Token &member);

    PSC::DataHolder &resolve(PSC::Context &ctx) override;
};

class ArrayElementResolver : public AbstractVariableResolver {
private:
    std::vector<Node*> indices;
    std::unique_ptr<AbstractVariableResolver> resolver;

public:
    ArrayElementResolver(const Token &token, std::unique_ptr<AbstractVariableResolver> &&resolver, std::vector<Node*> &&indicies);

    PSC::DataHolder &resolve(PSC::Context &ctx) override;
};

class SimpleVariableSource : public AbstractVariableResolver {
public:
    SimpleVariableSource(const Token &token);

    PSC::DataHolder &resolve(PSC::Context &ctx) override;

    const std::string &getName() const;

    const Token &getToken() const;
};
