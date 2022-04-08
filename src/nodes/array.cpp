#include "psc/error.h"
#include "nodes/array.h"

ArrayDeclareNode::ArrayDeclareNode(const Token &token, std::unique_ptr<PSC::Array> &&array)
    : Node(token), array(std::move(array))
{}

std::unique_ptr<NodeResult> ArrayDeclareNode::evaluate(PSC::Context &ctx) {
    if (ctx.getArray(array->name) != nullptr)
        throw PSC::RedeclarationError(token, ctx, array->name);

    ctx.addArray(std::move(array));

    return std::make_unique<NodeResult>(nullptr, PSC::DT_NONE);
}


ArrayAccessNode::ArrayAccessNode(const Token &token, const std::string &arrayName)
    : Node(token), arrayName(arrayName)
{}

std::unique_ptr<NodeResult> ArrayAccessNode::evaluate(PSC::Context &ctx) {
    PSC::Array *array = ctx.getArray(arrayName);
    if (array == nullptr)
        throw PSC::NotDefinedError(token, ctx, "Array '" + arrayName + "'");

    if (indices.size() != array->dimensions.size())
        throw PSC::RuntimeError(token, ctx, "Invalid number of indices");

    std::vector<PSC::int_t> evaluatedIndices;
    evaluatedIndices.reserve(indices.size());

    for (int i = 0; i < (int) indices.size(); i++) {
        Node *index = indices[i];
        auto result = index->evaluate(ctx);

        if (result->type != PSC::DT_INTEGER)
            throw PSC::RuntimeError(index->getToken(), ctx, "Array indices must be of type INTEGER");

        PSC::int_t x = result->get<PSC::Integer>().value;
        if (!array->dimensions[i].isValidIndex(x))
            throw PSC::RuntimeError(index->getToken(), ctx, "Index out of bounds");

        evaluatedIndices.push_back(x);
    }

    PSC::Value &value = array->getElement(evaluatedIndices);

    std::unique_ptr<PSC::Value> ret;
    switch (array->type) {
        case PSC::DT_INTEGER:
            ret = std::make_unique<PSC::Integer>((PSC::Integer&) value);
            break;
        case PSC::DT_REAL:
            ret = std::make_unique<PSC::Real>((PSC::Real&) value);
            break;
        case PSC::DT_BOOLEAN:
            ret = std::make_unique<PSC::Boolean>((PSC::Boolean&) value);
            break;
        case PSC::DT_CHAR:
            ret = std::make_unique<PSC::Char>((PSC::Char&) value);
            break;
        case PSC::DT_STRING:
            ret = std::make_unique<PSC::String>((PSC::String&) value);
            break;
        default:
            std::abort();
    }

    return std::make_unique<NodeResult>(std::move(ret), array->type);
}


ArrayAssignNode::ArrayAssignNode(const Token &token, const std::string &arrayName)
    : Node(token), arrayName(arrayName)
{}

std::unique_ptr<NodeResult> ArrayAssignNode::evaluate(PSC::Context &ctx) {
    PSC::Array *array = ctx.getArray(arrayName);
    if (array == nullptr)
        throw PSC::NotDefinedError(token, ctx, "Array '" + arrayName + "'");

    if (indices.size() != array->dimensions.size())
        throw PSC::RuntimeError(token, ctx, "Invalid number of indices");

    std::vector<PSC::int_t> evaluatedIndices;
    evaluatedIndices.reserve(indices.size());

    for (int i = 0; i < (int) indices.size(); i++) {
        Node *index = indices[i];
        auto result = index->evaluate(ctx);

        if (result->type != PSC::DT_INTEGER)
            throw PSC::RuntimeError(index->getToken(), ctx, "Array indices must be of type INTEGER");

        PSC::int_t x = result->get<PSC::Integer>().value;
        if (!array->dimensions[i].isValidIndex(x))
            throw PSC::RuntimeError(index->getToken(), ctx, "Index out of bounds");

        evaluatedIndices.push_back(x);
    }

    PSC::Value &value = array->getElement(evaluatedIndices);

    auto assignValue = node->evaluate(ctx);
    if (assignValue->type != array->type)
        throw PSC::InvalidUsageError(token, ctx, "assignment operator: incompatible data types");

    switch (array->type) {
        case PSC::DT_INTEGER:   
            ((PSC::Integer&) value).value = assignValue->get<PSC::Integer>().value;
            break;
        case PSC::DT_REAL:   
            ((PSC::Real&) value).value = assignValue->get<PSC::Real>().value;
            break;
        case PSC::DT_BOOLEAN:   
            ((PSC::Boolean&) value).value = assignValue->get<PSC::Boolean>().value;
            break;
        case PSC::DT_CHAR:   
            ((PSC::Char&) value).value = assignValue->get<PSC::Char>().value;
            break;
        case PSC::DT_STRING:   
            ((PSC::String&) value).value = assignValue->get<PSC::String>().value;
            break;
        default:
            std::abort();
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DT_NONE);
}
