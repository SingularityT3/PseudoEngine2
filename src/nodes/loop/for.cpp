#include "psc/error.h"
#include "nodes/loop/control.h"
#include "nodes/loop/for.h"

ForLoopNode::ForLoopNode(const Token &token, const Token &identifier, Node &start, Node &stop, Node *step, PSC::Block *block)
    : Node(token),
    identifier(identifier),
    start(start),
    stop(stop),
    step(step),
    block(block)
{}

std::unique_ptr<NodeResult> ForLoopNode::evaluate(PSC::Context &ctx) {
    PSC::Variable *iterator = ctx.getVariable(identifier.value);
    if (iterator == nullptr) {
        iterator = new PSC::Variable(identifier.value, PSC::DataType::INTEGER, false);
        ctx.addVariable(iterator);
    }

    if (iterator->type != PSC::DataType::INTEGER)
        throw PSC::RuntimeError(token, ctx, "Iterator variable must be of type INTEGER");


    auto startRes = start.evaluate(ctx);

    if (startRes->type != PSC::DataType::INTEGER)
        throw PSC::RuntimeError(token, ctx, "Start value of FOR loop iterator must be of type INTEGER");


    auto stopRes = stop.evaluate(ctx);

    if (stopRes->type != PSC::DataType::INTEGER)
        throw PSC::RuntimeError(token, ctx, "Stop value of FOR loop iterator must be of type INTEGER");


    PSC::int_t stepValue;
    if (step != nullptr) {
        auto stepRes = step->evaluate(ctx);

        if (stepRes->type != PSC::DataType::INTEGER)
            throw PSC::RuntimeError(token, ctx, "Step value of FOR loop iterator must be of type INTEGER");

        stepValue = stepRes->get<PSC::Integer>();
    } else {
        stepValue = 1;
    }

    PSC::Integer &iteratorValue = iterator->get<PSC::Integer>();

    PSC::int_t startValue = startRes->get<PSC::Integer>();
    PSC::int_t stopValue = stopRes->get<PSC::Integer>();

    bool stepNegative = stepValue < 0;

    for (iteratorValue = startValue;
        (stepNegative && iteratorValue.value >= stopValue) || (!stepNegative && iteratorValue.value <= stopValue);
        iteratorValue.value += stepValue
    ) {
        try {
            block->run(ctx);
        } catch (BreakErrSignal&) {
            break;
        } catch (ContinueErrSignal&) {
            continue;
        }
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
