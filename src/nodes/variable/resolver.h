#pragma once
#include <memory>

#include "lexer/tokens.h"
#include "psc/variable.h"
#include "psc/scope/context.h"

class AbstractVariableResolver {
protected:
    const Token &token;

public:
    AbstractVariableResolver(const Token &token);

    virtual ~AbstractVariableResolver() = default;

    virtual PSC::DataHolder &resolve(PSC::Context &ctx) const = 0;
};

class PointerDereferencer : public AbstractVariableResolver {
private:
    std::unique_ptr<AbstractVariableResolver> resolver;

public:
    PointerDereferencer(const Token &token, std::unique_ptr<AbstractVariableResolver> &&resolver);

    PSC::DataHolder &resolve(PSC::Context &ctx) const override;
};

class CompositeResolver : public AbstractVariableResolver {
private:
    std::unique_ptr<AbstractVariableResolver> resolver;
    const Token &member;

public:
    CompositeResolver(const Token &token, std::unique_ptr<AbstractVariableResolver> &&resolver, const Token &member);

    PSC::DataHolder &resolve(PSC::Context &ctx) const override;
};

class ArrayElementResolver : public AbstractVariableResolver {
private:
    std::vector<Node*> indices;
    std::unique_ptr<AbstractVariableResolver> resolver;

public:
    ArrayElementResolver(const Token &token, std::unique_ptr<AbstractVariableResolver> &&resolver, std::vector<Node*> &&indicies);

    PSC::DataHolder &resolve(PSC::Context &ctx) const override;
};

class SimpleVariableSource : public AbstractVariableResolver {
public:
    using AbstractVariableResolver::AbstractVariableResolver;

    PSC::DataHolder &resolve(PSC::Context &ctx) const override;

    const std::string &getName() const;

    const Token &getToken() const;
};
