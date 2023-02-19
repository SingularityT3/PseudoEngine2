#include "pch.h"

#include "psc/types/type_definitions.h"
#include "psc/types/types.h"
#include "psc/scope/block.h"

using namespace PSC;

Enum::Enum(const std::string &name) : definitionName(name) {}

void Enum::operator=(const Enum &other) {
    if (definitionName != other.definitionName) std::abort();
    value = other.value;
}

const EnumTypeDefinition &Enum::getDefinition(Context &ctx) const {
    return *ctx.getEnumDefinition(definitionName);
}

Pointer::Pointer(const std::string &name)
    : definitionName(name) {}

void Pointer::setValue(Variable *value) {
    ptr = value;
}

void Pointer::operator=(const Pointer &other) {
    if (definitionName != other.definitionName) std::abort();
    ptr = other.ptr;
}

Variable *Pointer::getValue() const {
    return ptr;
}

const PointerTypeDefinition &Pointer::getDefinition(Context &ctx) const {
    return *ctx.getPointerDefinition(definitionName);
}

Composite::Composite(const std::string &name, Context &parent)
    : definitionName(name),
    ctx(std::make_unique<Context>(&parent, definitionName))
{
    getDefinition(*ctx).initBlock.run(*ctx);
}

Composite::Composite(const Composite &other)
    : definitionName(other.definitionName),
    ctx(std::make_unique<Context>(*other.ctx))
{}

void Composite::operator=(const Composite &other) {
    if (definitionName != other.definitionName) std::abort();
    // ctx = std::make_unique<Context>(*other.ctx);
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
