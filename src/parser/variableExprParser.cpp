#include "pch.h"

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

    if (currentToken->type != TokenType::DATA_TYPE && currentToken->type != TokenType::IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "data type");

    const Token& type = *currentToken;
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

    if (currentToken->type != TokenType::EQUALS && currentToken->type != TokenType::ASSIGNMENT)
        throw PSC::ExpectedTokenError(*currentToken, "'=' or '<-'");
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
        throw PSC::ExpectedTokenError(*currentToken, " literal"); 
    }

    if (negative) value = create<NegateNode>(minusToken, *value);

    return create<ConstDeclareNode>(op, *value, identifier);
}

std::unique_ptr<AbstractVariableResolver> Parser::parseIdentifierExpression() {
    const Token &identifier = *currentToken;
    advance();
    std::unique_ptr<AbstractVariableResolver> resolver = std::make_unique<SimpleVariableSource>(identifier);

    bool resolve = true;
    while (resolve) {
        switch (currentToken->type) {
            case TokenType::PERIOD: {
                const Token &token = *currentToken;
                advance();

                resolver = std::make_unique<CompositeResolver>(token, std::move(resolver), *currentToken);
                advance();
                break;
            } case TokenType::CARET:
                resolver = std::make_unique<PointerDereferencer>(*currentToken, std::move(resolver));
                advance();
                break;
            case TokenType::LSQRBRACKET: {
                const Token &token = *currentToken;
                advance();
                std::vector<Node*> indices;
                while (true) {
                    Node *index = parseArithmeticExpression();
                    indices.push_back(index);

                    if (currentToken->type == TokenType::COMMA) advance();
                    else break;
                }

                if (currentToken->type != TokenType::RSQRBRACKET)
                    throw PSC::ExpectedTokenError(*currentToken, "']'");
                advance();

                resolver = std::make_unique<ArrayElementResolver>(token, std::move(resolver), std::move(indices));
                break;
            } default:
                resolve = false;
        }
    }

    return resolver;
}
