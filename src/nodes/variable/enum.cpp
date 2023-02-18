#include "pch.h"

#include "psc/error.h"
#include "nodes/variable/variable.h"
#include "nodes/variable/enum.h"

EnumDefineNode::EnumDefineNode(const Token &token, const Token &name, std::vector<std::string> &&values)
    : Node(token), name(name), values(std::move(values)) {}

std::unique_ptr<NodeResult> EnumDefineNode::evaluate(PSC::Context &ctx) {
    if (isIdentifierType(name))
        throw PSC::RuntimeError(token, ctx, "Redefinition of type '" + name.value + "'");

    PSC::EnumTypeDefinition definition(name.value, std::move(values));
    PSC::TypeDefinitions::createEnumDefinition(std::move(definition));
    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
