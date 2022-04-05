#include "psc/error.h"
#include "psc/types.h"
#include "nodes/eval/comparison.h"

BooleanNode::BooleanNode(const Token &token)
    : Node(token),
    value(token.type == TT_TRUE ? true : (token.type == TT_FALSE ? false : throw 0))
{}

std::unique_ptr<NodeResult> BooleanNode::evaluate(PSC::Context &ctx) {
    return std::make_unique<NodeResult>(new PSC::Boolean(value), PSC::DT_BOOLEAN);
}


ComparisonNode::ComparisonNode(const Token &token, Node &left, Node &right)
    : BinaryNode(token, left, right)
{
    switch (token.type) {
        case TT_EQUALS:
            op = "=";
            break;
        case TT_NOT_EQUALS:
            op = "<>";
            break;
        case TT_GREATER:
            op = ">";
            break;
        case TT_LESSER:
            op = "<";
            break;
        case TT_GREATER_EQUAL:
            op = ">=";
            break;
        case TT_LESSER_EQUAL:
            op = "<=";
            break;
        default:
            throw 0;
    }
}

std::unique_ptr<NodeResult> ComparisonNode::evaluate(PSC::Context &ctx) {
    auto leftRes = left.evaluate(ctx);
    auto rightRes = right.evaluate(ctx);

    if ((leftRes->type != PSC::DT_INTEGER && leftRes->type != PSC::DT_REAL)
        || (rightRes->type != PSC::DT_INTEGER && rightRes->type != PSC::DT_REAL)
    ) {
        bool eq = token.type == TT_EQUALS;
        if (!eq && token.type != TT_NOT_EQUALS)
            throw PSC::InvalidUsageError(token, ctx, "'" + op + "' operator, operands must be of type Integer or Real");

        if (leftRes->type != rightRes->type) {
            return std::make_unique<NodeResult>(new PSC::Boolean(!eq), PSC::DT_BOOLEAN);
        } else if (leftRes->type == PSC::DT_BOOLEAN) {
            bool comparisonEq = leftRes->get<PSC::Boolean>() == rightRes->get<PSC::Boolean>();
            bool res = (!eq && !comparisonEq) || (eq && comparisonEq);
            return std::make_unique<NodeResult>(new PSC::Boolean(res), PSC::DT_BOOLEAN);
        } else {
            throw PSC::InvalidUsageError(token, ctx, "'" + op + "' operator, operands must be of type Integer, Real or Boolean");
        }
    }

    const PSC::Number &leftNum = leftRes->get<PSC::Number>();
    const PSC::Number &rightNum = rightRes->get<PSC::Number>();

    const PSC::Boolean *res;
    switch (token.type) {
        case TT_EQUALS:
            res = leftNum == rightNum;
            break;
        case TT_NOT_EQUALS:
            res = leftNum != rightNum;
            break;
        case TT_GREATER:
            res = leftNum > rightNum;
            break;
        case TT_LESSER:
            res = leftNum < rightNum;
            break;
        case TT_GREATER_EQUAL:
            res = leftNum >= rightNum;
            break;
        case TT_LESSER_EQUAL:
            res = leftNum <= rightNum;
            break;
        default:
            throw 0;
    }

    return std::make_unique<NodeResult>(res, PSC::DT_BOOLEAN);
}
