#include "psc/error.h"
#include "nodes/selection/case.h"

CaseComponent::CaseComponent(PSC::Block &block)
    : block(block)
{}


EqualsCaseComponent::EqualsCaseComponent(PSC::Block &block, Node &node)
    : CaseComponent(block), node(node)
{}

bool EqualsCaseComponent::match(const NodeResult &value, PSC::Context &ctx) {
    auto res = node.evaluate(ctx);

    if (value.type == PSC::DT_REAL && res->type == PSC::DT_INTEGER)
        return value.get<PSC::Real>().value == res->get<PSC::Integer>().value;

    if (value.type == PSC::DT_INTEGER && res->type == PSC::DT_REAL)
        return value.get<PSC::Integer>().value == res->get<PSC::Real>().value;

    if (value.type != res->type) return false;

    switch (value.type) {
        case PSC::DT_INTEGER:
            return value.get<PSC::Integer>().value == res->get<PSC::Integer>().value;
        case PSC::DT_REAL:
            return value.get<PSC::Real>().value == res->get<PSC::Real>().value;
        case PSC::DT_BOOLEAN:
            return value.get<PSC::Boolean>().value == res->get<PSC::Boolean>().value;
        case PSC::DT_CHAR:
            return value.get<PSC::Char>().value == res->get<PSC::Char>().value;
        case PSC::DT_STRING:
            return value.get<PSC::String>().value == res->get<PSC::String>().value;
        default: ;
    }
    std::abort();
}


RangeCaseComponent::RangeCaseComponent(PSC::Block &block, Node &lowerBound, Node &upperBound)
    : CaseComponent(block), lowerBound(lowerBound), upperBound(upperBound)
{}

bool RangeCaseComponent::match(const NodeResult &value, PSC::Context &ctx) {
    PSC::real_t testValue;
    if (value.type == PSC::DT_INTEGER) testValue = value.get<PSC::Integer>().value;
    else if (value.type == PSC::DT_REAL) testValue = value.get<PSC::Real>().value;
    else return false;

    PSC::real_t lowerVal;
    auto lower = lowerBound.evaluate(ctx);

    if (lower->type == PSC::DT_INTEGER) lowerVal = lower->get<PSC::Integer>().value;
    else if (lower->type == PSC::DT_REAL) lowerVal = lower->get<PSC::Real>().value;
    else throw PSC::RuntimeError(lowerBound.getToken(), ctx, "Lower bound must be of type INTEGER or REAL");

    PSC::real_t upperVal;
    auto upper = upperBound.evaluate(ctx);

    if (upper->type == PSC::DT_INTEGER) upperVal = upper->get<PSC::Integer>().value;
    else if (upper->type == PSC::DT_REAL) upperVal = upper->get<PSC::Real>().value;
    else throw PSC::RuntimeError(upperBound.getToken(), ctx, "Upper bound must be of type INTEGER or REAL");

    return (lowerVal <= testValue) && (testValue <= upperVal);
}


OtherwiseCaseComponent::OtherwiseCaseComponent(PSC::Block &block)
    : CaseComponent(block)
{}

bool OtherwiseCaseComponent::match(const NodeResult &value, PSC::Context &ctx) {
    return true;
}


void CaseNode::addCase(CaseComponent *caseComponent) {
    cases.emplace_back(caseComponent);
}

std::unique_ptr<NodeResult> CaseNode::evaluate(PSC::Context &ctx) {
    auto value = node.evaluate(ctx);

    for (auto &c : cases) {
        if (c->match(*value, ctx)) {
            c->block.run(ctx);
            break;
        }
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DT_NONE);
}
