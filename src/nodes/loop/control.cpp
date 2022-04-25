#include "pch.h"

#include "nodes/loop/control.h"

BreakErrSignal::BreakErrSignal(const Token &token) : token(token) {}

ContinueErrSignal::ContinueErrSignal(const Token &token) : token(token) {}

std::unique_ptr<NodeResult> BreakNode::evaluate(PSC::Context&) {
    throw BreakErrSignal(token);
}

std::unique_ptr<NodeResult> ContinueNode::evaluate(PSC::Context&) {
    throw ContinueErrSignal(token);
}
