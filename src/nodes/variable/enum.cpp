#include "pch.h"

#include "psc/error.h"
#include "nodes/variable/variable.h"
#include "nodes/variable/enum.h"

EnumDefineNode::EnumDefineNode(const Token &token, const Token &name, std::vector<std::string> &&values)
    : Node(token), name(name), values(std::move(values)) {}

std::unique_ptr<NodeResult> EnumDefineNode::evaluate(PSC::Context &ctx) {
    if (ctx.isIdentifierType(name, false))
        throw PSC::RedefinitionError(token, ctx, name.value);

    PSC::EnumTypeDefinition definition(name.value, std::move(values));
    ctx.createEnumDefinition(std::move(definition));
    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
