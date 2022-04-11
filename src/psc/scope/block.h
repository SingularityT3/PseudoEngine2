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
        virtual ~Block() = default;

        void addNode(Node *node);

        virtual void run(PSC::Context &ctx);
    };

    class MainBlock final : public Block {
    public:
        void run(PSC::Context &ctx) override;
    };
}
