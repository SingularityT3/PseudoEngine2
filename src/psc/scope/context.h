#pragma once
#include <string>
#include <vector>
#include <memory>
#include "tokens.h"
#include "psc/variable.h"
#include "psc/procedure.h"
#include "nodes/nodeResult.h"

namespace PSC {
    class Context {
    private:
        Context *parent;
        std::string name;
        std::vector<std::unique_ptr<Variable>> variables;
        std::vector<std::unique_ptr<Procedure>> procedures;
        std::vector<std::unique_ptr<Function>> functions;

    public:
        const Token *switchToken = nullptr;

        const bool isFunctionCtx;
        std::unique_ptr<NodeResult> returnValue;
        const PSC::DataType returnType;

        Context(Context *parent, const std::string &name);

        Context(Context *parent, const std::string &name, bool isFunctionCtx, PSC::DataType returnType);

        static std::unique_ptr<Context> createGlobalContext();

        Context *getParent() const;

        const std::string &getName() const;

        void addVariable(Variable *variable);

        Variable *getVariable(const std::string &varName);

        void addProcedure(std::unique_ptr<Procedure> &&procedure);

        Procedure *getProcedure(const std::string &procedureName);

        void addFunction(std::unique_ptr<Function> &&function);

        Function *getFunction(const std::string &functionName);
    };
}
