#include "pch.h"

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

    if (value.type == PSC::DataType::REAL && res->type == PSC::DataType::INTEGER)
        return value.get<PSC::Real>().value == res->get<PSC::Integer>().value;

    if (value.type == PSC::DataType::INTEGER && res->type == PSC::DataType::REAL)
        return value.get<PSC::Integer>().value == res->get<PSC::Real>().value;

    if (value.type != res->type) return false;

    switch (value.type.type) {
        case PSC::DataType::INTEGER:
            return value.get<PSC::Integer>().value == res->get<PSC::Integer>().value;
        case PSC::DataType::REAL:
            return value.get<PSC::Real>().value == res->get<PSC::Real>().value;
        case PSC::DataType::BOOLEAN:
            return value.get<PSC::Boolean>().value == res->get<PSC::Boolean>().value;
        case PSC::DataType::CHAR:
            return value.get<PSC::Char>().value == res->get<PSC::Char>().value;
        case PSC::DataType::STRING:
            return value.get<PSC::String>().value == res->get<PSC::String>().value;
        case PSC::DataType::ENUM:
            return *value.get<PSC::Enum>().value == *res->get<PSC::Enum>().value;
        case PSC::DataType::POINTER:
            return value.get<PSC::Pointer>().getValue() == res->get<PSC::Pointer>().getValue();
        case PSC::DataType::COMPOSITE:
            return false;
        case PSC::DataType::NONE: ;
    }
    std::abort();
}


RangeCaseComponent::RangeCaseComponent(PSC::Block &block, Node &lowerBound, Node &upperBound)
    : CaseComponent(block), lowerBound(lowerBound), upperBound(upperBound)
{}

bool RangeCaseComponent::match(const NodeResult &value, PSC::Context &ctx) {
    PSC::real_t testValue;
    if (value.type == PSC::DataType::INTEGER) testValue = value.get<PSC::Integer>().value;
    else if (value.type == PSC::DataType::REAL) testValue = value.get<PSC::Real>().value;
    else return false;

    PSC::real_t lowerVal;
    auto lower = lowerBound.evaluate(ctx);

    if (lower->type == PSC::DataType::INTEGER) lowerVal = lower->get<PSC::Integer>().value;
    else if (lower->type == PSC::DataType::REAL) lowerVal = lower->get<PSC::Real>().value;
    else throw PSC::RuntimeError(lowerBound.getToken(), ctx, "Lower bound must be of type INTEGER or REAL");

    PSC::real_t upperVal;
    auto upper = upperBound.evaluate(ctx);

    if (upper->type == PSC::DataType::INTEGER) upperVal = upper->get<PSC::Integer>().value;
    else if (upper->type == PSC::DataType::REAL) upperVal = upper->get<PSC::Real>().value;
    else throw PSC::RuntimeError(upperBound.getToken(), ctx, "Upper bound must be of type INTEGER or REAL");

    return (lowerVal <= testValue) && (testValue <= upperVal);
}


OtherwiseCaseComponent::OtherwiseCaseComponent(PSC::Block &block)
    : CaseComponent(block)
{}

bool OtherwiseCaseComponent::match(const NodeResult&, PSC::Context&) {
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

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
