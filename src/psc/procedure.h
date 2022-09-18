#pragma once
#include <string>
#include <vector>
#include "psc/types/types.h"
#include "lexer/tokens.h"

namespace PSC {
    class Block;
    class Context;

    struct Parameter {
        const std::string name;
        const PSC::DataType type;

        Parameter(const std::string &name, PSC::DataType type);
    };

    struct Procedure {
        const std::string name;
        std::vector<Parameter> parameters;
        bool byRef = false;
        PSC::Block *block;

        Procedure(const std::string &name);

        virtual ~Procedure() = default;

        std::vector<DataType> getTypes() const;

        virtual void run(PSC::Context &ctx);
    };

    struct Function : public Procedure {
        PSC::DataType returnType;
        const Token *defToken; // For throwing 'missing return' errors

        using Procedure::Procedure;
    };
};
