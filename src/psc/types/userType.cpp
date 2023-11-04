#include "pch.h"

#include "psc/types/type_definitions.h"
#include "psc/types/types.h"
#include "psc/scope/block.h"

using namespace PSC;

Enum::Enum(const std::string &name) : definitionName(name) {}

void Enum::operator=(const Enum &other) {
    if (definitionName != other.definitionName) std::abort();
    idx = other.idx;
}

const EnumTypeDefinition &Enum::getDefinition(Context &ctx) const {
    return *ctx.getEnumDefinition(definitionName);
}

std::string Enum::getString(Context &ctx) const {
    const EnumTypeDefinition &definition = getDefinition(ctx);
    return definition.values[idx];
}

void Enum::dump(std::ostream &out) const {
    out << "ENUM " << definitionName << " " << idx;
}

bool Enum::load(std::istream &in, Context &ctx) {
    std::string s;
    in >> s;
    if (s != "ENUM") return false;

    in >> s;
    const EnumTypeDefinition *def = ctx.getEnumDefinition(s);
    if (def == nullptr || (def->name != definitionName)) return false;

    size_t idx1;
    in >> idx1;
    if (in.fail() || idx1 >= def->values.size()) return false;

    idx = idx1;
    return true;
}

Pointer::Pointer(const std::string &name)
    : definitionName(name) {}

Pointer::Pointer(const Pointer &other)
    : ptr(other.ptr), varCtx(other.varCtx), definitionName(other.definitionName)
{}

void Pointer::setValue(Variable *value) {
    ptr = value;
    varCtx = value->parent;
    while (varCtx->isCompositeCtx) {
        varCtx = varCtx->getParent();
    }
}

void Pointer::operator=(const Pointer &other) {
    if (definitionName != other.definitionName) std::abort();
    ptr = other.ptr;
    varCtx = other.varCtx;
}

Variable *Pointer::getValue() const {
    return ptr;
}

const Context *Pointer::getCtx() const {
    return varCtx;
}

const PointerTypeDefinition &Pointer::getDefinition(Context &ctx) const {
    return *ctx.getPointerDefinition(definitionName);
}

void Pointer::dump(std::ostream&) const {}

bool Pointer::load(std::istream&, Context&) {
    return false;
}


Composite::Composite(const std::string &name, Context &parent)
    : definitionName(name),
    ctx(std::make_unique<Context>(&parent, definitionName, true))
{
    getDefinition(*ctx).initBlock.run(*ctx);
}

Composite::Composite(const Composite &other)
    : definitionName(other.definitionName),
    ctx(std::make_unique<Context>(*other.ctx))
{}

void Composite::operator=(const Composite &other) {
    if (definitionName != other.definitionName) std::abort();
    ctx->copyVariableData(*other.ctx);
}

DataHolder *Composite::getMember(const std::string &name) {
    PSC::Variable *var = ctx->getVariable(name);
    if (var != nullptr) return var;

    PSC::Array *arr = ctx->getArray(name);
    return arr;
}

const CompositeTypeDefinition &Composite::getDefinition(PSC::Context &ctx) const {
    return *ctx.getCompositeDefinition(definitionName);
}

void Composite::dump(std::ostream &out) const {
    out << "COMPOSITE " << definitionName << " ";
    const auto &vars = ctx->getVariables();
    for (size_t i = 0; i < vars.size(); i++) {
        vars[i]->getRawValue().dump(out);
        if (i < vars.size() - 1) out << " ";
    }
}

bool Composite::load(std::istream &in, Context &ctx) {
    std::string s;
    in >> s;
    if (s != "COMPOSITE") return false;

    in >> s;
    const CompositeTypeDefinition *def = ctx.getCompositeDefinition(s);
    if (def == nullptr || (def->name != definitionName)) return false;

    for (const auto &var : this->ctx->getVariables()) {
        if (!var->getRawValue().load(in, ctx)) return false;
    }

    return true;
}
