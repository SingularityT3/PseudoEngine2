#include "psc/error.h"
#include "nodes/eval/arithmetic.h"

IntegerNode::IntegerNode(const Token &token)
    : Node(token), valueInt(std::stol(token.value))
{}

std::unique_ptr<NodeResult> IntegerNode::evaluate(PSC::Context &ctx) {
    return std::make_unique<NodeResult>(new PSC::Integer(valueInt), PSC::DT_INTEGER);
}


RealNode::RealNode(const Token &token)
    : Node(token), valueReal(std::stod(token.value))
{}

std::unique_ptr<NodeResult> RealNode::evaluate(PSC::Context &ctx) {
    return std::make_unique<NodeResult>(new PSC::Real(valueReal), PSC::DT_REAL);
}


CharNode::CharNode(const Token &token)
    : Node(token), valueChar(token.value[0])
{}

std::unique_ptr<NodeResult> CharNode::evaluate(PSC::Context &ctx) {
    return std::make_unique<NodeResult>(new PSC::Char(valueChar), PSC::DT_CHAR);
}

StringNode::StringNode(const Token &token)
    : Node(token), valueStr(token.value)
{}

std::unique_ptr<NodeResult> StringNode::evaluate(PSC::Context &ctx) {
    return std::make_unique<NodeResult>(new PSC::String(valueStr), PSC::DT_STRING);
}


std::unique_ptr<NodeResult> NegateNode::evaluate(PSC::Context &ctx) {
    auto nodeResult = node.evaluate(ctx);

    if (nodeResult->type != PSC::DT_INTEGER && nodeResult->type != PSC::DT_REAL) {
        throw PSC::InvalidUsageError(token, ctx, "'-' operator, operand must be of type Integer or Real");
    }

    const PSC::Number &num = nodeResult->get<PSC::Number>();
    PSC::Number *res = num * PSC::Integer(-1);

    PSC::DataType type = res->real ? PSC::DT_REAL : PSC::DT_INTEGER;
    return std::make_unique<NodeResult>(res, type);
}

ArithmeticOperationNode::ArithmeticOperationNode(const Token &token, Node &left, Node &right)
    : BinaryNode(token, left, right) {
    switch (token.type) {
        case TT_PLUS:
            op = "+";
            break;
        case TT_MINUS:
            op = "-";
            break;
        case TT_STAR:
            op = "*";
            break;
        case TT_SLASH:
            op = "/";
            break;
        case TT_MOD:
            op = "MOD";
            break;
        case TT_DIV:
            op = "DIV";
            break;
        default:
            throw 0;
    }
}

std::unique_ptr<NodeResult> ArithmeticOperationNode::evaluate(PSC::Context &ctx) {
    auto leftRes = left.evaluate(ctx);
    auto rightRes = right.evaluate(ctx);

    if ((leftRes->type != PSC::DT_INTEGER && leftRes->type != PSC::DT_REAL)
        || (rightRes->type != PSC::DT_INTEGER && rightRes->type != PSC::DT_REAL)
    ) {
        throw PSC::InvalidUsageError(token, ctx, "'" + op + "' operator, operands must be of type Integer or Real");
    }

    const PSC::Number &leftNum = leftRes->get<PSC::Number>();
    const PSC::Number &rightNum = rightRes->get<PSC::Number>();

    PSC::Number *resNum;
    switch (token.type) {
        case TT_PLUS:
            resNum = leftNum + rightNum;
            break;
        case TT_MINUS:
            resNum = leftNum - rightNum;
            break;
        case TT_STAR:
            resNum = leftNum * rightNum;
            break;
        case TT_SLASH:
            resNum = leftNum / rightNum;
            break;
        case TT_MOD:
            resNum = leftNum % rightNum;
            break;
        case TT_DIV:
            resNum = leftNum | rightNum;
            break;
        default:
            throw 0;
    }

    PSC::DataType type = resNum->real ? PSC::DT_REAL : PSC::DT_INTEGER;
    return std::make_unique<NodeResult>(resNum, type);
}
