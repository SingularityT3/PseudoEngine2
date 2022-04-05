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
        std::vector<Variable*> variables;
        std::vector<Procedure*> procedures;
        std::vector<Function*> functions;

    public:
        const Token *switchToken = nullptr;

        const bool isFunctionCtx;
        std::unique_ptr<NodeResult> returnValue;
        const PSC::DataType returnType;

        Context(Context *parent, const std::string &name);

        Context(Context *parent, const std::string &name, bool isFunctionCtx, PSC::DataType returnType);

        ~Context();

        Context *getParent() const;

        const std::string &getName() const;

        void addVariable(Variable *variable);

        Variable *getVariable(const std::string &varName);

        void addProcedure(Procedure *procedure);

        Procedure *getProcedure(const std::string &procedureName);

        void addFunction(Function *function);

        Function *getFunction(const std::string &functionName);
    };
}
