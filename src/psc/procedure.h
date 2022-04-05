#pragma once
#include <string>
#include <vector>
#include "psc/types/types.h"
#include "tokens.h"

namespace PSC {
    class Block;

    struct Parameter {
        const std::string name;
        const PSC::DataType type;

        Parameter(const std::string &name, PSC::DataType type);
    };

    struct Procedure {
        const std::string name;
        std::vector<Parameter> parameters;
        bool byRef;
        PSC::Block *block;

        Procedure(const std::string &name);
    };

    struct Function : public Procedure {
        PSC::DataType returnType;
        const Token *defToken; // For throwing 'missing return' errors

        using Procedure::Procedure;
    };
};
