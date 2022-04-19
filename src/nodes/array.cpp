#include "psc/error.h"
#include "nodes/array.h"

ArrayDeclareNode::ArrayDeclareNode(const Token &token, std::vector<const Token*> &&identifiers, PSC::DataType type, std::vector<Node*> &&bounds)
    : Node(token),
    identifiers(identifiers),
    type(type),
    bounds(std::move(bounds))
{}

std::unique_ptr<NodeResult> ArrayDeclareNode::evaluate(PSC::Context &ctx) {
    if (bounds.size() % 2 != 0 || bounds.size() == 0) std::abort();

    for (auto identifier : identifiers) {
        if (ctx.getArray(identifier->value) != nullptr)
            throw PSC::RedeclarationError(token, ctx, identifier->value);
    }

    std::vector<PSC::ArrayDimension> dimensions;

    for (size_t i = 0; i < bounds.size(); i += 2) {
        auto lowerRes = bounds[i]->evaluate(ctx);
        if (lowerRes->type != PSC::DataType::INTEGER)
            throw PSC::RuntimeError(bounds[i]->getToken(), ctx, "Array indices must be of type INTEGER");

        auto upperRes = bounds[i + 1]->evaluate(ctx);
        if (upperRes->type != PSC::DataType::INTEGER)
            throw PSC::RuntimeError(bounds[i + 1]->getToken(), ctx, "Array indices must be of type INTEGER");

        const PSC::int_t &lower = lowerRes->get<PSC::Integer>().value;
        const PSC::int_t &upper = upperRes->get<PSC::Integer>().value;

        if (upper < lower)
            throw PSC::RuntimeError(bounds[i + 1]->getToken(), ctx, "Array upper bound must be greater than lower bound");

        dimensions.emplace_back(i / 2, lower, upper);
    }

    for (auto identifier : identifiers) {
        auto array = std::make_unique<PSC::Array>(identifier->value, type, dimensions);
        array->init();
        ctx.addArray(std::move(array));
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


ArrayAccessNode::ArrayAccessNode(const Token &token, std::vector<Node*> &&indices)
    : Node(token), indices(std::move(indices))
{}

std::pair<PSC::Value*, PSC::DataType> ArrayAccessNode::getValue(PSC::Context &ctx) {
    PSC::Array *array = ctx.getArray(token.value);
    if (array == nullptr)
        throw PSC::NotDefinedError(token, ctx, "Array '" + token.value + "'");

    if (indices.size() != array->dimensions.size())
        throw PSC::RuntimeError(token, ctx, "Invalid number of indices");

    std::vector<PSC::int_t> evaluatedIndices;
    evaluatedIndices.reserve(indices.size());

    for (size_t i = 0; i < indices.size(); i++) {
        Node *index = indices[i];
        auto result = index->evaluate(ctx);

        if (result->type != PSC::DataType::INTEGER)
            throw PSC::RuntimeError(index->getToken(), ctx, "Array indices must be of type INTEGER");

        PSC::int_t x = result->get<PSC::Integer>().value;
        if (!array->dimensions[i].isValidIndex(x))
            throw PSC::RuntimeError(index->getToken(), ctx, "Index out of bounds");

        evaluatedIndices.push_back(x);
    }

    PSC::Value *value = &(array->getElement(evaluatedIndices));
    std::pair<PSC::Value*, PSC::DataType> p(value, array->type);
    return p;
}

std::unique_ptr<NodeResult> ArrayAccessNode::evaluate(PSC::Context &ctx) {
    auto [value, type] = getValue(ctx);    

    std::unique_ptr<PSC::Value> ret;
    switch (type) {
        case PSC::DataType::INTEGER:
            ret = std::make_unique<PSC::Integer>(*((PSC::Integer*) value));
            break;
        case PSC::DataType::REAL:
            ret = std::make_unique<PSC::Real>(*((PSC::Real*) value));
            break;
        case PSC::DataType::BOOLEAN:
            ret = std::make_unique<PSC::Boolean>(*((PSC::Boolean*) value));
            break;
        case PSC::DataType::CHAR:
            ret = std::make_unique<PSC::Char>(*((PSC::Char*) value));
            break;
        case PSC::DataType::STRING:
            ret = std::make_unique<PSC::String>(*((PSC::String*) value));
            break;
        default:
            std::abort();
    }

    return std::make_unique<NodeResult>(std::move(ret), type);
}


ArrayAssignNode::ArrayAssignNode(const Token &token, Node &node, const std::string &arrayName, std::vector<Node*> &&indices)
    : UnaryNode(token, node),
    arrayName(arrayName),
    indices(std::move(indices))
{}

std::unique_ptr<NodeResult> ArrayAssignNode::evaluate(PSC::Context &ctx) {
    PSC::Array *array = ctx.getArray(arrayName);
    if (array == nullptr)
        throw PSC::NotDefinedError(token, ctx, "Array '" + arrayName + "'");

    if (indices.size() != array->dimensions.size())
        throw PSC::RuntimeError(token, ctx, "Invalid number of indices");

    std::vector<PSC::int_t> evaluatedIndices;
    evaluatedIndices.reserve(indices.size());

    for (size_t i = 0; i < indices.size(); i++) {
        Node *index = indices[i];
        auto result = index->evaluate(ctx);

        if (result->type != PSC::DataType::INTEGER)
            throw PSC::RuntimeError(index->getToken(), ctx, "Array indices must be of type INTEGER");

        PSC::int_t x = result->get<PSC::Integer>().value;
        if (!array->dimensions[i].isValidIndex(x))
            throw PSC::RuntimeError(index->getToken(), ctx, "Index out of bounds");

        evaluatedIndices.push_back(x);
    }

    PSC::Value &value = array->getElement(evaluatedIndices);

    auto assignValue = node.evaluate(ctx);
    assignValue->implicitCast(array->type);
    if (assignValue->type != array->type)
        throw PSC::InvalidUsageError(token, ctx, "assignment operator: incompatible data types");

    switch (array->type) {
        case PSC::DataType::INTEGER:   
            ((PSC::Integer&) value).value = assignValue->get<PSC::Integer>().value;
            break;
        case PSC::DataType::REAL:   
            ((PSC::Real&) value).value = assignValue->get<PSC::Real>().value;
            break;
        case PSC::DataType::BOOLEAN:   
            ((PSC::Boolean&) value).value = assignValue->get<PSC::Boolean>().value;
            break;
        case PSC::DataType::CHAR:   
            ((PSC::Char&) value).value = assignValue->get<PSC::Char>().value;
            break;
        case PSC::DataType::STRING:   
            ((PSC::String&) value).value = assignValue->get<PSC::String>().value;
            break;
        default:
            std::abort();
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
