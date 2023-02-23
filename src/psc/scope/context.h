#pragma once
#include <string>
#include <vector>
#include <memory>
#include "lexer/tokens.h"
#include "psc/variable.h"
#include "psc/array.h"
#include "psc/procedure.h"
#include "nodes/nodeResult.h"

namespace PSC {
    class Context {
    private:
        Context *parent;
        std::string name;
        std::vector<std::unique_ptr<Variable>> variables;
        std::vector<std::unique_ptr<Array>> arrays;
        std::vector<std::unique_ptr<Procedure>> procedures;
        std::vector<std::unique_ptr<Function>> functions;
        std::vector<std::unique_ptr<EnumTypeDefinition>> enums;
        std::vector<std::unique_ptr<PointerTypeDefinition>> pointers;
        std::vector<std::unique_ptr<CompositeTypeDefinition>> composites;

    public:
        const Token *switchToken = nullptr;

        const bool isFunctionCtx, isCompositeCtx;
        std::unique_ptr<NodeResult> returnValue;
        const PSC::DataType returnType;

        Context(Context *parent, const std::string &name);

        // Functions
        Context(Context *parent, const std::string &name, bool isFunctionCtx, PSC::DataType returnType);

        // Composites
        Context(Context *parent, const std::string &name, bool isCompositeCtx);

        // For copying composites
        explicit Context(const Context &other);

        // Only for composites
        void copyVariableData(const Context &other);

        static std::unique_ptr<Context> createGlobalContext();

        Context *getParent() const;

        Context *getGlobalContext();

        const std::string &getName() const;

        void addVariable(Variable *variable);

        Variable *getVariable(const std::string &varName, bool global = true);

        void addProcedure(std::unique_ptr<Procedure> &&procedure);

        Procedure *getProcedure(const std::string &procedureName);

        void addFunction(std::unique_ptr<Function> &&function);

        Function *getFunction(const std::string &functionName);

        void addArray(std::unique_ptr<Array> &&array);

        Array *getArray(const std::string &arrayName, bool global = true);

        PSC::DataType getType(const Token &token, bool global = true);

        bool isIdentifierType(const Token &identifier, bool global = true);

        Enum *getEnumElement(const std::string &value, bool global = true);

        void createEnumDefinition(EnumTypeDefinition &&definition);

        void createPointerDefinition(PointerTypeDefinition &&definition);

        void createCompositeDefinition(CompositeTypeDefinition &&definition);

        const EnumTypeDefinition *getEnumDefinition(const std::string &name, bool global = true);

        const PointerTypeDefinition *getPointerDefinition(const std::string &name, bool global = true);

        const CompositeTypeDefinition *getCompositeDefinition(const std::string &name, bool global = true);
    };
}
