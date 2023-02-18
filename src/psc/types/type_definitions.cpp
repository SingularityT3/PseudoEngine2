#include "pch.h"

#include "psc/types/types.h"
#include "psc/types/type_definitions.h"

using namespace PSC;

AbstractTypeDefinition::AbstractTypeDefinition(const std::string &name)
    : name(name) {}

EnumTypeDefinition::EnumTypeDefinition(const std::string &name, std::vector<std::string> &&values)
    : AbstractTypeDefinition(name), values(std::move(values)) {}

PointerTypeDefinition::PointerTypeDefinition(const std::string &name, DataType type)
    : AbstractTypeDefinition(name), type(type) {}

CompositeTypeDefinition::CompositeTypeDefinition(const std::string &name, PSC::Block &initBlock)
    : AbstractTypeDefinition(name), initBlock(initBlock) {}


std::vector<std::unique_ptr<EnumTypeDefinition>> TypeDefinitions::enums;
std::vector<std::unique_ptr<PointerTypeDefinition>> TypeDefinitions::pointers;
std::vector<std::unique_ptr<CompositeTypeDefinition>> TypeDefinitions::composites;

Enum *TypeDefinitions::isEnumElement(const std::string &value) {
    for (auto &definition : enums) {
        for (size_t i = 0; i < definition->values.size(); i++) {
            if (definition->values[i] == value) {
                auto ptr = new Enum(definition->name);
                ptr->value = &definition->values[i];
                return ptr;
            }
        }
    }
    return nullptr;
}

void TypeDefinitions::createEnumDefinition(EnumTypeDefinition &&definition) {
    enums.emplace_back(std::make_unique<EnumTypeDefinition>(std::move(definition)));
}

void TypeDefinitions::createPointerDefinition(PointerTypeDefinition &&definition) {
    pointers.emplace_back(std::make_unique<PointerTypeDefinition>(std::move(definition)));
}

void TypeDefinitions::createCompositeDefinition(CompositeTypeDefinition &&definition) {
    composites.emplace_back(std::make_unique<CompositeTypeDefinition>(std::move(definition)));
}

const EnumTypeDefinition *TypeDefinitions::getEnumDefinition(const std::string &name) {
    for (const auto &e : enums) {
        if (e->name == name) return e.get();
    }
    return nullptr;
}

const PointerTypeDefinition *TypeDefinitions::getPointerDefinition(const std::string &name) {
    for (const auto &p : pointers) {
        if (p->name == name) return p.get();
    }
    return nullptr;
}

const CompositeTypeDefinition *TypeDefinitions::getCompositeDefinition(const std::string &name) {
    for (const auto &c : composites) {
        if (c->name == name) return c.get();
    }
    return nullptr;
}
