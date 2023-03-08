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
        const bool byRef;

        Parameter(const std::string &name, PSC::DataType type, bool byRef);
    };

    struct Procedure {
        const std::string name;
        std::vector<Parameter> parameters;
        PSC::Block *const block;

        Procedure(
            const std::string &name,
            std::vector<Parameter> &&parameters,
            PSC::Block *block
        );

        Procedure(const std::string &name);

        virtual ~Procedure() = default;

        std::vector<DataType> getTypes() const;

        virtual void run(PSC::Context &ctx);
    };

    struct Function : public Procedure {
        const PSC::DataType returnType;
        const Token *defToken; // For throwing 'missing return' errors

        Function(
            const std::string &name,
            std::vector<Parameter> &&parameters,
            PSC::Block *block,
            PSC::DataType returnType,
            const Token *defToken
        );

        // Builtin functions
        Function(const std::string &name, PSC::DataType returnType);
    };
};
