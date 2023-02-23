#include "pch.h"

#include "psc/error.h"
#include "psc/types/types.h"
#include "nodes/eval/comparison.h"

BooleanNode::BooleanNode(const Token &token)
    : Node(token), value(token.type == TokenType::TRUE)
{
    if (!this->value && token.type != TokenType::FALSE) std::abort();
}

std::unique_ptr<NodeResult> BooleanNode::evaluate(PSC::Context&) {
    return std::make_unique<NodeResult>(new PSC::Boolean(value), PSC::DataType::BOOLEAN);
}


ComparisonNode::ComparisonNode(const Token &token, Node &left, Node &right)
    : BinaryNode(token, left, right)
{
    switch (token.type) {
        case TokenType::EQUALS:
            op = "=";
            break;
        case TokenType::NOT_EQUALS:
            op = "<>";
            break;
        case TokenType::GREATER:
            op = ">";
            break;
        case TokenType::LESSER:
            op = "<";
            break;
        case TokenType::GREATER_EQUAL:
            op = ">=";
            break;
        case TokenType::LESSER_EQUAL:
            op = "<=";
            break;
        default:
            std::abort();
    }
}

std::unique_ptr<NodeResult> ComparisonNode::evaluate(PSC::Context &ctx) {
    auto leftRes = left.evaluate(ctx);
    auto rightRes = right.evaluate(ctx);

    if (leftRes->type == PSC::DataType::CHAR && rightRes->type == PSC::DataType::CHAR) {
        leftRes->type = rightRes->type = PSC::DataType::INTEGER;
        leftRes->data = leftRes->get<PSC::Char>().toInteger();
        rightRes->data = rightRes->get<PSC::Char>().toInteger();
    }

    if ((leftRes->type != PSC::DataType::INTEGER && leftRes->type != PSC::DataType::REAL)
        || (rightRes->type != PSC::DataType::INTEGER && rightRes->type != PSC::DataType::REAL)
    ) {
        bool eq = token.type == TokenType::EQUALS;
        if (!eq && token.type != TokenType::NOT_EQUALS)
            throw PSC::InvalidUsageError(token, ctx, "'" + op + "' operator, operands must be of numeric data type");

        if (leftRes->type != rightRes->type) {
            return std::make_unique<NodeResult>(new PSC::Boolean(!eq), PSC::DataType::BOOLEAN);
        } else if (leftRes->type == PSC::DataType::BOOLEAN) {
            bool comparisonEq = leftRes->get<PSC::Boolean>() == rightRes->get<PSC::Boolean>();
            bool res = (!eq && !comparisonEq) || (eq && comparisonEq);
            return std::make_unique<NodeResult>(new PSC::Boolean(res), PSC::DataType::BOOLEAN);
        } else if (leftRes->type == PSC::DataType::STRING) {
            bool comparisonEq = leftRes->get<PSC::String>().value == rightRes->get<PSC::String>().value;
            bool res = (!eq && !comparisonEq) || (eq && comparisonEq);
            return std::make_unique<NodeResult>(new PSC::Boolean(res), PSC::DataType::BOOLEAN);
        } else if (leftRes->type == PSC::DataType::ENUM) {
            bool comparisonEq = *leftRes->get<PSC::Enum>().value == *rightRes->get<PSC::Enum>().value;
            bool res = (!eq && !comparisonEq) || (eq && comparisonEq);
            return std::make_unique<NodeResult>(new PSC::Boolean(res), PSC::DataType::BOOLEAN);
        } else {
            throw PSC::InvalidUsageError(token, ctx, "'" + op + "' operator, operands must be of comparable type");
        }
    }

    const PSC::Number &leftNum = leftRes->get<PSC::Number>();
    const PSC::Number &rightNum = rightRes->get<PSC::Number>();

    std::unique_ptr<PSC::Boolean> res;
    switch (token.type) {
        case TokenType::EQUALS:
            res = leftNum == rightNum;
            break;
        case TokenType::NOT_EQUALS:
            res = leftNum != rightNum;
            break;
        case TokenType::GREATER:
            res = leftNum > rightNum;
            break;
        case TokenType::LESSER:
            res = leftNum < rightNum;
            break;
        case TokenType::GREATER_EQUAL:
            res = leftNum >= rightNum;
            break;
        case TokenType::LESSER_EQUAL:
            res = leftNum <= rightNum;
            break;
        default:
            std::abort();
    }

    return std::make_unique<NodeResult>(std::move(res), PSC::DataType::BOOLEAN);
}
