#pragma once
#include <vector>
#include "nodes/base.h"
#include "psc/scope/block.h"

class CaseComponent {
public:
    PSC::Block &block;

    CaseComponent(PSC::Block &block);

    virtual bool match(const NodeResult &value, PSC::Context &ctx) = 0;
};

class EqualsCaseComponent : public CaseComponent {
private:
    Node &node;

public:
    EqualsCaseComponent(PSC::Block &block, Node &node);

    bool match(const NodeResult &value, PSC::Context &ctx) override;
};

class RangeCaseComponent : public CaseComponent {
private:
    Node &lowerBound, &upperBound;

public:
    RangeCaseComponent(PSC::Block &block, Node &lowerBound, Node &upperBound);

    bool match(const NodeResult &value, PSC::Context &ctx) override;
};

class OtherwiseCaseComponent : public CaseComponent {
public:
    OtherwiseCaseComponent(PSC::Block &block);

    bool match(const NodeResult &value, PSC::Context &ctx) override;
};

class CaseNode : public UnaryNode {
private:
    std::vector<std::unique_ptr<CaseComponent>> cases;

public:
    using UnaryNode::UnaryNode;

    void addCase(CaseComponent *caseComponent);

    std::unique_ptr<NodeResult> evaluate(PSC::Context &ctx) override;
};
