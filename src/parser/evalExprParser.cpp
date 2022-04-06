#include "parser/parser.h"

Node *Parser::parseEvaluationExpression() {
    Node *logicalExpression = parseLogicalExpression();

    while (currentToken->type == TT_EQUALS || currentToken->type == TT_NOT_EQUALS) {
        const Token &op = *currentToken;
        advance();

        Node *otherlogicalExpression = parseLogicalExpression();
        logicalExpression = create<ComparisonNode>(op, *logicalExpression, *otherlogicalExpression);
    }

    return logicalExpression;
}

Node *Parser::parseLogicalExpression() {
    Node *comparisonExpr = parseComparisonExpression();

    while (currentToken->type == TT_AND || currentToken->type == TT_OR) {
        const Token &op = *currentToken;
        advance();

        Node *otherComparisonExpr = parseComparisonExpression();

        comparisonExpr = create<LogicNode>(op, *comparisonExpr, *otherComparisonExpr);
    }

    return comparisonExpr;
}

Node *Parser::parseComparisonExpression() {
    if (currentToken->type == TT_NOT) {
        const Token &op = *currentToken;
        advance();

        Node *node = parseComparisonExpression();

        return create<NotNode>(op, *node);
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

        strExpr = create<ComparisonNode>(op, *strExpr, *otherStrExpr);
    }

    return strExpr;
}

Node *Parser::parseStringExpression() {
    Node *arithmeticExpr = parseArithmeticExpression();

    while (currentToken->type == TT_AMPERSAND) {
        const Token &op = *currentToken;
        advance();

        Node *otherArithmeticExpr = parseArithmeticExpression();

        arithmeticExpr = create<StringConcatenationNode>(op, *arithmeticExpr, *otherArithmeticExpr);
    }

    return arithmeticExpr;
}

Node *Parser::parseArithmeticExpression() {
    Node *term = parseTerm();

    while (currentToken->type == TT_PLUS || currentToken->type == TT_MINUS) {
        const Token &op = *currentToken;
        advance();

        Node *otherTerm = parseTerm();

        term = create<ArithmeticOperationNode>(op, *term, *otherTerm);
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

        factor = create<ArithmeticOperationNode>(op, *factor, *otherFactor);
    }

    return factor;
}

Node *Parser::parseFactor() {
    if (currentToken->type == TT_MINUS) {
        const Token &op = *currentToken;
        advance();

        Node *atom = parseAtom();
        atom = create<NegateNode>(op, *atom);

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
            AccessNode *node = create<AccessNode>(*currentToken);
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

    return create<CastNode>(token, *expr, type);
}
