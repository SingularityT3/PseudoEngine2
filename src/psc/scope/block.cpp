#include "pch.h"
#include <iostream>

#include "nodes/functions/function.h"
#include "nodes/loop/control.h"
#include "psc/error.h"
#include "psc/scope/block.h"

using namespace PSC;

extern bool REPLMode;

void Block::addNode(Node *node) {
    nodes.push_back(node);
}

void Block::runNodeREPL(Node *node, PSC::Context &ctx) {
    auto result = node->evaluate(ctx);

    switch (result->type.type) {
        case PSC::DataType::INTEGER:
            std::cout << result->get<PSC::Integer>();
            break;
        case PSC::DataType::REAL: {
            PSC::real_t value = result->get<PSC::Real>().value;
            std::cout << value;

            double integerValue;
            if (std::modf(value, &integerValue) == 0.0) std::cout << ".0";
            break;
        } case PSC::DataType::BOOLEAN:
            std::cout << (result->get<PSC::Boolean>() ? "TRUE" : "FALSE");
            break;
        case PSC::DataType::CHAR:
            std::cout << "'" << result->get<PSC::Char>() << "'";
            break;
        case PSC::DataType::STRING:
            std::cout << "\"" << result->get<PSC::String>().value << "\"";
            break;
        case PSC::DataType::DATE: {
            auto str = result->get<PSC::Date>().toString();
            std::cout << str->value;
            break;
        } case PSC::DataType::ENUM: {
            auto &resEnum = result->get<PSC::Enum>();
            std::cout << resEnum.definitionName << ": " << resEnum.getString(ctx);
            break;
        } case PSC::DataType::POINTER: {
            auto &resPtr = result->get<PSC::Pointer>();

            const PSC::Context *ptrCtx = resPtr.getCtx();
            PSC::Context *tempCtx = &ctx;
            bool valid = true;
            while (valid && tempCtx != ptrCtx) {
                tempCtx = tempCtx->getParent();
                if (tempCtx == nullptr) valid = false;
            }

            std::cout << resPtr.definitionName << ": ";
            if (valid) {
                PSC::Variable *ptrValue = resPtr.getValue();
                const std::string &valueStr = ptrValue == nullptr ? "null" : ptrValue->name;
                std::cout << valueStr;
            } else {
                std::cout << "{DELETED}";
            }
            break;
        } case PSC::DataType::COMPOSITE:
            std::cout << result->get<PSC::Composite>().definitionName << " object";
            break;
        case PSC::DataType::NONE:
            return;
    }
    std::cout << std::endl;
}

void Block::_run(PSC::Context &ctx) {
    for (Node *node : nodes) {
        node->evaluate(ctx);
    }
}

void Block::_runREPL(PSC::Context &ctx) {
    for (Node *node : nodes) {
        runNodeREPL(node, ctx);
    }
}

void Block::run(PSC::Context &ctx) {
    if (REPLMode) _runREPL(ctx);
    else _run(ctx);
}

void MainBlock::run(PSC::Context &ctx) {
    try {
        Block::run(ctx);
    } catch (BreakErrSignal &e) {
        throw PSC::InvalidUsageError(e.token, ctx, "'BREAK' statement");
    } catch (ContinueErrSignal &e) {
        throw PSC::InvalidUsageError(e.token, ctx, "'CONTINUE' statement");
    }
}
