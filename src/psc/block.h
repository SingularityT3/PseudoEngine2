#pragma once
#include <vector>
#include "psc/context.h"
#include "nodes/base.h"

namespace PSC {
    class Block {
    private:
        std::vector<Node*> nodes;

        void runNode(Node *node, PSC::Context &ctx);

    public:
        void addNode(Node *node);

        void run(PSC::Context &ctx);
    };
}
