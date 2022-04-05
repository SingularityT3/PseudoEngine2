#include "parser/parser.h"

Node *Parser::parseDeclareExpression() {
    const Token &op = *currentToken;
    advance();

    if (currentToken->type != TT_IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "identifier");

    const Token &identifier = *currentToken;
    advance();

    if (currentToken->type != TT_COLON)
        throw PSC::ExpectedTokenError(*currentToken, "':'");
    advance();

    if (currentToken->type != TT_DATA_TYPE)
        throw PSC::ExpectedTokenError(*currentToken, "data type");

    PSC::DataType type = getPSCType();

    advance();
    DeclareNode *node = new DeclareNode(op, identifier, type);
    nodes.push_back(node);
    return node;
}

Node *Parser::parseConstDeclareExpression() {
    const Token &op = *currentToken;
    advance();

    if (currentToken->type != TT_IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "identifier");

    const Token &identifier = *currentToken;
    advance();

    if (currentToken->type != TT_EQUALS)
        throw PSC::ExpectedTokenError(*currentToken, "'='");
    advance();

    Node *value;
    const Token &minusToken = *currentToken;
    bool negative = currentToken->type == TT_MINUS;
    if (negative) advance();

    if (currentToken->type == TT_INTEGER) {
        value = parseLiteral<IntegerNode>();
    } else if (currentToken->type == TT_REAL) {
        value = parseLiteral<RealNode>();
    } else if (currentToken->type == TT_TRUE || currentToken->type == TT_FALSE) {
        value = parseLiteral<BooleanNode>();
    } else if (currentToken->type == TT_CHAR) {
        value = parseLiteral<CharNode>();
    } else if (currentToken->type == TT_STRING) {
        value = parseLiteral<StringNode>();
    } else {
        throw PSC::ExpectedTokenError(*currentToken, "Integer, Real or Boolean literal"); 
    }

    if (negative) {
        value = new NegateNode(minusToken, *value);
        nodes.push_back(value);
    }

    Node *node = new ConstDeclareNode(op, *value, identifier);
    nodes.push_back(node);
    return node;
}

Node *Parser::parseAssignmentExpression() {
    const Token &identifier = *currentToken;
    advance();

    if (currentToken->type != TT_ASSIGNMENT)
        throw PSC::ExpectedTokenError(*currentToken, "'<-'");

    const Token &op = *currentToken;
    advance();

    Node *value = parseEvaluationExpression();

    AssignNode *node = new AssignNode(op, *value, identifier);
    nodes.push_back(node);
    return node;
}
