#include "pch.h"

#include "parser/parser.h"

Node *Parser::parseArrayDeclare(const Token &declareToken, std::vector<const Token*> &identifiers) {
    if (currentToken->type != TokenType::ARRAY) std::abort();
    advance();

    if (currentToken->type != TokenType::LSQRBRACKET)
        throw PSC::ExpectedTokenError(*currentToken, "'['");
    advance();

    std::vector<Node*> bounds;

    while (true) {
        Node *lowerBound = parseArithmeticExpression();

        if (currentToken->type != TokenType::COLON)
            throw PSC::ExpectedTokenError(*currentToken, "':'");
        advance();

        Node *upperBound = parseArithmeticExpression();

        bounds.push_back(lowerBound);
        bounds.push_back(upperBound);

        if (currentToken->type == TokenType::COMMA) advance();
        else break;
    }

    if (currentToken->type != TokenType::RSQRBRACKET)
        throw PSC::ExpectedTokenError(*currentToken, "']'");
    advance();

    if (currentToken->type != TokenType::OF)
        throw PSC::ExpectedTokenError(*currentToken, "'OF'");
    advance();

    if (currentToken->type != TokenType::DATA_TYPE && currentToken->type != TokenType::IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "data type");

    const Token &type = *currentToken;
    advance();

    return create<ArrayDeclareNode>(declareToken, std::move(identifiers), type, std::move(bounds));
}
