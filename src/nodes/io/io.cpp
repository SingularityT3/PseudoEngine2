#include "pch.h"
#include <iostream>

#include "psc/error.h"
#include "nodes/io/io.h"

OutputNode::OutputNode(const Token &token, std::vector<Node*> &&nodes)
    : Node(token), nodes(std::move(nodes))
{}

std::unique_ptr<NodeResult> OutputNode::evaluate(PSC::Context &ctx) {
    for (Node *node : nodes) {
        auto result = node->evaluate(ctx);

        switch (result->type.type) {
            case PSC::DataType::INTEGER:
                std::cout << result->get<PSC::Integer>();
                break;
            case PSC::DataType::REAL:
                std::cout << result->get<PSC::Real>();
                break;
            case PSC::DataType::BOOLEAN:
                std::cout << (result->get<PSC::Boolean>() ? "TRUE" : "FALSE");
                break;
            case PSC::DataType::CHAR:
                std::cout << result->get<PSC::Char>();
                break;
            case PSC::DataType::STRING:
                std::cout << result->get<PSC::String>().value;
                break;
            case PSC::DataType::ENUM:
                std::cout << *result->get<PSC::Enum>().value;
                break;
            case PSC::DataType::POINTER:
                std::cout << result->get<PSC::Pointer>().definitionName << " object";
                break;
            case PSC::DataType::COMPOSITE:
                std::cout << result->get<PSC::Composite>().definitionName << " object";
                break;
            case PSC::DataType::NONE:
                std::cout.flush();
                throw PSC::RuntimeError(node->getToken(), ctx, "Expected a value for OUTPUT/PRINT");
        }
    }
    std::cout << std::endl;

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


InputNode::InputNode(const Token &token, const Token &identifier)
    : Node(token), identifier(identifier)
{}

std::unique_ptr<NodeResult> InputNode::evaluate(PSC::Context &ctx) {
    PSC::Variable *var = ctx.getVariable(identifier.value);
    if (var == nullptr) {
        var = new PSC::Variable(identifier.value, PSC::DataType::STRING, false, &ctx);
        ctx.addVariable(var);
    }

    PSC::String inputStr;
    std::getline(std::cin, inputStr.value);

    switch (var->type.type) {
        case PSC::DataType::INTEGER:
            var->get<PSC::Integer>() = inputStr.toInteger()->value;
            break;
        case PSC::DataType::REAL:
            var->get<PSC::Real>() = inputStr.toReal()->value;
            break;
        case PSC::DataType::BOOLEAN:
            var->get<PSC::Boolean>() = (inputStr.value == "TRUE");
            break;
        case PSC::DataType::CHAR:
            var->get<PSC::Char>() = inputStr.value.front();
            break;
        case PSC::DataType::STRING:
            var->get<PSC::String>().value = std::move(inputStr.value);
            break;
        case PSC::DataType::ENUM:
        case PSC::DataType::POINTER:
        case PSC::DataType::COMPOSITE:
            throw PSC::RuntimeError(token, ctx, "Cannot input non-primitive types");
        case PSC::DataType::NONE:
            std::abort();
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
