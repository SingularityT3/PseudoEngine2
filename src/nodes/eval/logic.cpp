#include "psc/error.h"
#include "psc/types/types.h"
#include "nodes/eval/logic.h"

LogicNode::LogicNode(const Token &token, Node &left, Node &right)
    : BinaryNode(token, left, right)
{
    switch (token.type) {
        case TT_AND:
            op = "AND";
            break;
        case TT_OR:
            op = "OR";
            break;
        default:
            throw 0;
    }
}

std::unique_ptr<NodeResult> LogicNode::evaluate(PSC::Context &ctx) {
    auto leftRes = left.evaluate(ctx);
    auto rightRes = right.evaluate(ctx);

    if (leftRes->type != PSC::DT_BOOLEAN || rightRes->type != PSC::DT_BOOLEAN) {
        throw PSC::InvalidUsageError(token, ctx, "'" + op + "' operator, operands must be of type Boolean");
    }

    const PSC::Boolean &leftBool = leftRes->get<PSC::Boolean>();
    const PSC::Boolean &rightBool = rightRes->get<PSC::Boolean>();

    PSC::Boolean *res = new PSC::Boolean();
    switch (token.type) {
        case TT_AND:
            *res = leftBool && rightBool;
            break;
        case TT_OR:
            *res = leftBool || rightBool;
            break;
        default:
            throw 0;
    }

    return std::make_unique<NodeResult>(res, PSC::DT_BOOLEAN);
}


std::unique_ptr<NodeResult> NotNode::evaluate(PSC::Context &ctx) {
    auto nodeRes = node.evaluate(ctx);

    if (nodeRes->type != PSC::DT_BOOLEAN) {
        throw PSC::InvalidUsageError(token, ctx, "'NOT' operator, operand must be of type Boolean");
    }

    PSC::Boolean *res = new PSC::Boolean(!nodeRes->get<PSC::Boolean>());
    return std::make_unique<NodeResult>(res, PSC::DT_BOOLEAN);
}
