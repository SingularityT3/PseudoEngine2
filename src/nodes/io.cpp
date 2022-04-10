#include <iostream>
#include "psc/error.h"
#include "nodes/io.h"

std::unique_ptr<NodeResult> OutputNode::evaluate(PSC::Context &ctx) {
    for (Node *node : nodes) {
        auto result = node->evaluate(ctx);

        switch (result->type) {
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
    if (var == nullptr) throw PSC::NotDefinedError(identifier, ctx, "Identifier '" + identifier.value + "'");

    PSC::String inputStr;
    std::getline(std::cin, inputStr.value);

    switch (var->type) {
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
        default:
            std::abort();
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
