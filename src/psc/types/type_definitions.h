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

    class Enum;
    class TypeDefinitions {
    private:
        static std::vector<std::unique_ptr<EnumTypeDefinition>> enums;
        static std::vector<std::unique_ptr<PointerTypeDefinition>> pointers;
        static std::vector<std::unique_ptr<CompositeTypeDefinition>> composites;
    
    public:
        static Enum *isEnumElement(const std::string &value);

        static void createEnumDefinition(EnumTypeDefinition &&definition);

        static void createPointerDefinition(PointerTypeDefinition &&definition);

        static void createCompositeDefinition(CompositeTypeDefinition &&definition);

        static const EnumTypeDefinition *getEnumDefinition(const std::string &name);

        static const PointerTypeDefinition *getPointerDefinition(const std::string &name);

        static const CompositeTypeDefinition *getCompositeDefinition(const std::string &name);
    };
}
