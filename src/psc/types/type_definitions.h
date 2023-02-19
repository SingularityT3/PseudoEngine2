#pragma once
#include <vector>
#include <string>

#include "psc/types/datatypes.h"

namespace PSC {
    class Block;

    struct AbstractTypeDefinition {
        const std::string name;

        AbstractTypeDefinition(const std::string &name);
    };

    struct EnumTypeDefinition : AbstractTypeDefinition {
        const std::vector<std::string> values;

        EnumTypeDefinition(const std::string &name, std::vector<std::string> &&values);
    };

    struct PointerTypeDefinition : AbstractTypeDefinition {
        const DataType type;

        PointerTypeDefinition(const std::string &name, DataType type);
    };

    struct CompositeTypeDefinition : AbstractTypeDefinition {
        PSC::Block &initBlock;

        CompositeTypeDefinition(const std::string &name, PSC::Block &initBlock);
    };
}
