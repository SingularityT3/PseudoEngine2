#pragma once
#include <vector>
#include "psc/scope/context.h"
#include "nodes/base.h"

namespace PSC {
    class Block {
    private:
        std::vector<Node*> nodes;

        void runNodeREPL(Node *node, PSC::Context &ctx);

        void _run(PSC::Context &ctx);

        void _runREPL(PSC::Context &ctx);

    public:
        void addNode(Node *node);

        void run(PSC::Context &ctx);
    };
}
