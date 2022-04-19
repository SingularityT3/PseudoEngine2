#include "parser/parser.h"

Node *Parser::parseDeclareExpression() {
    const Token &op = *currentToken;
    std::vector<const Token*> identifiers;
    advance();

    while (true) {
        if (currentToken->type != TokenType::IDENTIFIER)
            throw PSC::ExpectedTokenError(*currentToken, "identifier");

        identifiers.push_back(currentToken);
        advance();

        if (currentToken->type == TokenType::COMMA) advance();
        else break;
    }

    if (currentToken->type != TokenType::COLON)
        throw PSC::ExpectedTokenError(*currentToken, "':'");
    advance();

    if (currentToken->type == TokenType::ARRAY)
        return parseArrayDeclare(op, identifiers);

    if (currentToken->type != TokenType::DATA_TYPE)
        throw PSC::ExpectedTokenError(*currentToken, "data type");

    PSC::DataType type = getPSCType();

    advance();
    return create<DeclareNode>(op, std::move(identifiers), type);
}

Node *Parser::parseConstDeclareExpression() {
    const Token &op = *currentToken;
    advance();

    if (currentToken->type != TokenType::IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "identifier");

    const Token &identifier = *currentToken;
    advance();

    if (currentToken->type != TokenType::EQUALS)
        throw PSC::ExpectedTokenError(*currentToken, "'='");
    advance();

    Node *value;
    const Token &minusToken = *currentToken;
    bool negative = currentToken->type == TokenType::MINUS;
    if (negative) advance();

    if (currentToken->type == TokenType::INTEGER) {
        value = parseLiteral<IntegerNode>();
    } else if (currentToken->type == TokenType::REAL) {
        value = parseLiteral<RealNode>();
    } else if (currentToken->type == TokenType::TRUE || currentToken->type == TokenType::FALSE) {
        value = parseLiteral<BooleanNode>();
    } else if (currentToken->type == TokenType::CHAR) {
        value = parseLiteral<CharNode>();
    } else if (currentToken->type == TokenType::STRING) {
        value = parseLiteral<StringNode>();
    } else {
        throw PSC::ExpectedTokenError(*currentToken, "Integer, Real or Boolean literal"); 
    }

    if (negative) value = create<NegateNode>(minusToken, *value);

    return create<ConstDeclareNode>(op, *value, identifier);
}

Node *Parser::parseAssignmentExpression() {
    const Token &identifier = *currentToken;
    advance();

    if (currentToken->type != TokenType::ASSIGNMENT)
        throw PSC::ExpectedTokenError(*currentToken, "'<-'");

    const Token &op = *currentToken;
    advance();

    Node *value = parseEvaluationExpression();

    return create<AssignNode>(op, *value, identifier);
}
