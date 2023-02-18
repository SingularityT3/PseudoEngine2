#include "pch.h"

#include "psc/error.h"
#include "nodes/variable/variable.h"
#include "nodes/variable/composite.h"

CompositeDefineNode::CompositeDefineNode(const Token &token, const Token &name, PSC::Block &initBlock)
    : Node(token), name(name), initBlock(initBlock) {}

std::unique_ptr<NodeResult> CompositeDefineNode::evaluate(PSC::Context &ctx) {
    if (isIdentifierType(name))
        throw PSC::RuntimeError(token, ctx, "Redefinition of type '" + name.value + "'");
    
    PSC::CompositeTypeDefinition definition(name.value, initBlock);
    PSC::TypeDefinitions::createCompositeDefinition(std::move(definition));
    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
