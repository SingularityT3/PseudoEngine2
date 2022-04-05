#include "parser/parser.h"

Node *Parser::parseEvaluationExpression() {
    Node *logicalExpression = parseLogicalExpression();

    while (currentToken->type == TT_EQUALS || currentToken->type == TT_NOT_EQUALS) {
        const Token &op = *currentToken;
        advance();

        Node *otherlogicalExpression = parseLogicalExpression();
        logicalExpression = new ComparisonNode(op, *logicalExpression, *otherlogicalExpression);
        nodes.push_back(logicalExpression);
    }

    return logicalExpression;
}

Node *Parser::parseLogicalExpression() {
    Node *comparisonExpr = parseComparisonExpression();

    while (currentToken->type == TT_AND || currentToken->type == TT_OR) {
        const Token &op = *currentToken;
        advance();

        Node *otherComparisonExpr = parseComparisonExpression();

        comparisonExpr = new LogicNode(op, *comparisonExpr, *otherComparisonExpr);
        nodes.push_back(comparisonExpr);
    }

    return comparisonExpr;
}

Node *Parser::parseComparisonExpression() {
    if (currentToken->type == TT_NOT) {
        const Token &op = *currentToken;
        advance();

        Node *node = parseComparisonExpression();
        node = new NotNode(op, *node);
        nodes.push_back(node);

        return node;
    } else if (currentToken->type == TT_TRUE || currentToken->type == TT_FALSE) {
        return parseLiteral<BooleanNode>();
    }

    Node *strExpr = parseStringExpression();

    while (
        currentToken->type == TT_EQUALS
        || currentToken->type == TT_NOT_EQUALS
        || currentToken->type == TT_GREATER
        || currentToken->type == TT_LESSER
        || currentToken->type == TT_GREATER_EQUAL
        || currentToken->type == TT_LESSER_EQUAL
    ) {
        const Token &op = *currentToken;
        advance();

        Node *otherStrExpr = parseStringExpression();

        strExpr = new ComparisonNode(op, *strExpr, *otherStrExpr);
        nodes.push_back(strExpr);
    }

    return strExpr;
}

Node *Parser::parseStringExpression() {
    Node *arithmeticExpr = parseArithmeticExpression();

    while (currentToken->type == TT_AMPERSAND) {
        const Token &op = *currentToken;
        advance();

        Node *otherArithmeticExpr = parseArithmeticExpression();

        arithmeticExpr = new StringConcatenationNode(op, *arithmeticExpr, *otherArithmeticExpr);
        nodes.push_back(arithmeticExpr);
    }

    return arithmeticExpr;
}

Node *Parser::parseArithmeticExpression() {
    Node *term = parseTerm();

    while (currentToken->type == TT_PLUS || currentToken->type == TT_MINUS) {
        const Token &op = *currentToken;
        advance();

        Node *otherTerm = parseTerm();

        term = new ArithmeticOperationNode(op, *term, *otherTerm);
        nodes.push_back(term);
    }

    return term;
}

Node *Parser::parseTerm() {
    Node *factor = parseFactor();

    while (
        currentToken->type == TT_STAR
        || currentToken->type == TT_SLASH
        || currentToken->type == TT_DIV
        || currentToken->type == TT_MOD
    ) {
        const Token &op = *currentToken;
        advance();

        Node *otherFactor = parseFactor();

        factor = new ArithmeticOperationNode(op, *factor, *otherFactor);
        nodes.push_back(factor);
    }

    return factor;
}

Node *Parser::parseFactor() {
    if (currentToken->type == TT_MINUS) {
        const Token &op = *currentToken;
        advance();

        Node *atom = parseAtom();
        atom = new NegateNode(op, *atom);
        nodes.push_back(atom);

        return atom;
    }

    return parseAtom();
}

Node *Parser::parseAtom() {
    if (currentToken->type == TT_INTEGER) {
        return parseLiteral<IntegerNode>();
    } else if (currentToken->type == TT_REAL) {
        return parseLiteral<RealNode>();
    } else if (currentToken->type == TT_CHAR) {
        return parseLiteral<CharNode>();
    } else if (currentToken->type == TT_STRING) {
        return parseLiteral<StringNode>();
    } else if (currentToken->type == TT_IDENTIFIER) {
        if (idx + 1 >= (int) tokens->size() || (*tokens)[idx + 1]->type != TT_LPAREN) {
            AccessNode *node = new AccessNode(*currentToken);
            nodes.push_back(node);
            advance();
            return node;
        } else {
            return parseFunctionCall();
        }
    } else if (currentToken->type == TT_LPAREN) {
        advance();
        Node *expr = parseEvaluationExpression();

        if (currentToken->type != TT_RPAREN)
            throw PSC::ExpectedTokenError(*currentToken, "')'");

        advance();
        return expr;
    } else if (currentToken->type == TT_DATA_TYPE) {
        return parseCast();
    }

    throw PSC::ExpectedTokenError(*currentToken, "value or expression");
}

Node *Parser::parseCast() {
    const Token &token = *currentToken;
    PSC::DataType type = getPSCType();
    advance();

    if (currentToken->type != TT_LPAREN)
        throw PSC::ExpectedTokenError(*currentToken, "'('");
    advance();

    Node *expr = parseEvaluationExpression();

    if (currentToken->type != TT_RPAREN)
        throw PSC::ExpectedTokenError(*currentToken, "')'");
    advance();

    CastNode *node = new CastNode(token, *expr, type);
    nodes.push_back(node);
    return node;
}
