#include <iostream>
#include <limits>
#include "nodes/functions/function.h"
#include "psc/error.h"
#include "psc/scope/block.h"

using namespace PSC;

extern bool REPLMode;

void Block::addNode(Node *node) {
    nodes.push_back(node);
}

void Block::runNodeREPL(Node *node, PSC::Context &ctx) {
    auto result = node->evaluate(ctx);

    switch (result->type) {
        case PSC::DT_INTEGER:
            std::cout << result->get<PSC::Integer>();
            break;
        case PSC::DT_REAL:
            std::cout << result->get<PSC::Real>();
            break;
        case PSC::DT_BOOLEAN:
            std::cout << (result->get<PSC::Boolean>() ? "TRUE" : "FALSE");
            break;
        case PSC::DT_CHAR:
            std::cout << "'" << result->get<PSC::Char>() << "'";
            break;
        case PSC::DT_STRING:
            std::cout << "\"" << result->get<PSC::String>().value << "\"";
            break;
        case PSC::DT_NONE:
            return;
    }
    std::cout << std::endl;
}

void Block::_run(PSC::Context &ctx) {
    for (Node *node : nodes) {
        node->evaluate(ctx);
        if (ctx.isFunctionCtx && ctx.returnValue != nullptr) break;
    }
}

void Block::_runREPL(PSC::Context &ctx) {
    for (Node *node : nodes) {
        runNodeREPL(node, ctx);
        if (ctx.isFunctionCtx && ctx.returnValue != nullptr) break;
    }
}

void Block::run(PSC::Context &ctx) {
    if (REPLMode) _runREPL(ctx);
    else _run(ctx);
}
