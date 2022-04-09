#include "parser/parser.h"

Node *Parser::parseArrayDeclare(const Token &declareToken, const Token &identifier) {
    if (currentToken->type != TT_ARRAY) std::abort();
    advance();

    if (currentToken->type != TT_LSQRBRACKET)
        throw PSC::ExpectedTokenError(*currentToken, "'['");
    advance();

    std::vector<Node*> bounds;

    while (true) {
        Node *lowerBound = parseArithmeticExpression();

        if (currentToken->type != TT_COLON)
            throw PSC::ExpectedTokenError(*currentToken, "':'");
        advance();

        Node *upperBound = parseArithmeticExpression();

        bounds.push_back(lowerBound);
        bounds.push_back(upperBound);

        if (currentToken->type == TT_COMMA) advance();
        else break;
    }

    if (currentToken->type != TT_RSQRBRACKET)
        throw PSC::ExpectedTokenError(*currentToken, "']'");
    advance();

    if (currentToken->type != TT_OF)
        throw PSC::ExpectedTokenError(*currentToken, "'OF'");
    advance();

    if (currentToken->type != TT_DATA_TYPE)
        throw PSC::ExpectedTokenError(*currentToken, "data type");

    PSC::DataType type = getPSCType();
    advance();

    return create<ArrayDeclareNode>(declareToken, identifier.value, type, std::move(bounds));
}

Node *Parser::parseArrayOperation() {
    const Token &identifier = *currentToken;
    advance();

    if (currentToken->type != TT_LSQRBRACKET) std::abort();
    advance();

    std::vector<Node*> indices;

    while (true) {
        Node *index = parseArithmeticExpression();
        indices.push_back(index);

        if (currentToken->type == TT_COMMA) advance();
        else break;
    }

    if (currentToken->type != TT_RSQRBRACKET)
        throw PSC::ExpectedTokenError(*currentToken, "']'");
    advance();

    if (currentToken->type == TT_ASSIGNMENT) {
        const Token &op = *currentToken;
        advance();
        Node *node = parseEvaluationExpression();
        return create<ArrayAssignNode>(op, *node, identifier.value, std::move(indices));
    }

    return create<ArrayAccessNode>(identifier, std::move(indices));
}
