#include "pch.h"

#include "psc/error.h"
#include "nodes/eval/arithmetic.h"

IntegerNode::IntegerNode(const Token &token)
    : Node(token), valueInt(std::stol(token.value))
{}

std::unique_ptr<NodeResult> IntegerNode::evaluate(PSC::Context&) {
    return std::make_unique<NodeResult>(new PSC::Integer(valueInt), PSC::DataType::INTEGER);
}


RealNode::RealNode(const Token &token)
    : Node(token), valueReal(std::stod(token.value))
{}

std::unique_ptr<NodeResult> RealNode::evaluate(PSC::Context&) {
    return std::make_unique<NodeResult>(new PSC::Real(valueReal), PSC::DataType::REAL);
}


CharNode::CharNode(const Token &token)
    : Node(token), valueChar(token.value[0])
{}

std::unique_ptr<NodeResult> CharNode::evaluate(PSC::Context&) {
    return std::make_unique<NodeResult>(new PSC::Char(valueChar), PSC::DataType::CHAR);
}

StringNode::StringNode(const Token &token)
    : Node(token), valueStr(token.value)
{}

std::unique_ptr<NodeResult> StringNode::evaluate(PSC::Context&) {
    return std::make_unique<NodeResult>(new PSC::String(valueStr), PSC::DataType::STRING);
}


std::unique_ptr<NodeResult> NegateNode::evaluate(PSC::Context &ctx) {
    auto nodeResult = node.evaluate(ctx);

    if (nodeResult->type != PSC::DataType::INTEGER && nodeResult->type != PSC::DataType::REAL) {
        throw PSC::InvalidUsageError(token, ctx, "'-' operator, operand must be of type Integer or Real");
    }

    const PSC::Number &num = nodeResult->get<PSC::Number>();
    auto res = num * PSC::Integer(-1);

    PSC::DataType type = res->real ? PSC::DataType::REAL : PSC::DataType::INTEGER;
    return std::make_unique<NodeResult>(std::move(res), type);
}

ArithmeticOperationNode::ArithmeticOperationNode(const Token &token, Node &left, Node &right)
    : BinaryNode(token, left, right) {
    switch (token.type) {
        case TokenType::PLUS:
            op = "+";
            break;
        case TokenType::MINUS:
            op = "-";
            break;
        case TokenType::STAR:
            op = "*";
            break;
        case TokenType::SLASH:
            op = "/";
            break;
        case TokenType::MOD:
            op = "MOD";
            break;
        case TokenType::DIV:
            op = "DIV";
            break;
        default:
            std::abort();
    }
}

std::unique_ptr<NodeResult> ArithmeticOperationNode::evaluate(PSC::Context &ctx) {
    auto leftRes = left.evaluate(ctx);
    auto rightRes = right.evaluate(ctx);

    if ((leftRes->type != PSC::DataType::INTEGER && leftRes->type != PSC::DataType::REAL)
        || (rightRes->type != PSC::DataType::INTEGER && rightRes->type != PSC::DataType::REAL)
    ) {
        throw PSC::InvalidUsageError(token, ctx, "'" + op + "' operator, operands must be of type Integer or Real");
    }

    const PSC::Number &leftNum = leftRes->get<PSC::Number>();
    const PSC::Number &rightNum = rightRes->get<PSC::Number>();

    std::unique_ptr<PSC::Number> resNum;
    switch (token.type) {
        case TokenType::PLUS:
            resNum = leftNum + rightNum;
            break;
        case TokenType::MINUS:
            resNum = leftNum - rightNum;
            break;
        case TokenType::STAR:
            resNum = leftNum * rightNum;
            break;
        case TokenType::SLASH:
            if ((rightNum.real && ((PSC::Real&) rightNum).value == 0)
                || (!rightNum.real && ((PSC::Integer&) rightNum).value == 0)
            ) throw PSC::RuntimeError(token, ctx, "Division by 0");
            resNum = leftNum / rightNum;
            break;
        case TokenType::MOD:
            if ((rightNum.real && ((PSC::Real&) rightNum).value == 0)
                || (!rightNum.real && ((PSC::Integer&) rightNum).value == 0)
            ) throw PSC::RuntimeError(token, ctx, "Modulus by 0");
            resNum = leftNum % rightNum;
            break;
        case TokenType::DIV:
        if ((rightNum.real && ((PSC::Real&) rightNum).value == 0)
                || (!rightNum.real && ((PSC::Integer&) rightNum).value == 0)
            ) throw PSC::RuntimeError(token, ctx, "Division by 0");
            resNum = leftNum | rightNum;
            break;
        default:
            std::abort();
    }

    PSC::DataType type = resNum->real ? PSC::DataType::REAL : PSC::DataType::INTEGER;
    return std::make_unique<NodeResult>(std::move(resNum), type);
}
