#include "pch.h"

#include "parser/parser.h"

Node *Parser::parseEvaluationExpression() {
    Node *logicalExpression = parseLogicalExpression();

    while (currentToken->type == TokenType::EQUALS || currentToken->type == TokenType::NOT_EQUALS) {
        const Token &op = *currentToken;
        advance();

        Node *otherlogicalExpression = parseLogicalExpression();
        logicalExpression = create<ComparisonNode>(op, *logicalExpression, *otherlogicalExpression);
    }

    return logicalExpression;
}

Node *Parser::parseLogicalExpression() {
    Node *comparisonExpr = parseComparisonExpression();

    while (currentToken->type == TokenType::AND || currentToken->type == TokenType::OR) {
        const Token &op = *currentToken;
        advance();

        Node *otherComparisonExpr = parseComparisonExpression();

        comparisonExpr = create<LogicNode>(op, *comparisonExpr, *otherComparisonExpr);
    }

    return comparisonExpr;
}

Node *Parser::parseComparisonExpression() {
    if (currentToken->type == TokenType::NOT) {
        const Token &op = *currentToken;
        advance();

        Node *node = parseComparisonExpression();

        return create<NotNode>(op, *node);
    }

    Node *strExpr = parseStringExpression();

    while (
        currentToken->type == TokenType::EQUALS
        || currentToken->type == TokenType::NOT_EQUALS
        || currentToken->type == TokenType::GREATER
        || currentToken->type == TokenType::LESSER
        || currentToken->type == TokenType::GREATER_EQUAL
        || currentToken->type == TokenType::LESSER_EQUAL
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

    while (currentToken->type == TokenType::AMPERSAND) {
        const Token &op = *currentToken;
        advance();

        Node *otherArithmeticExpr = parseArithmeticExpression();

        arithmeticExpr = create<StringConcatenationNode>(op, *arithmeticExpr, *otherArithmeticExpr);
    }

    return arithmeticExpr;
}

Node *Parser::parseArithmeticExpression() {
    Node *term = parseTerm();

    while (currentToken->type == TokenType::PLUS || currentToken->type == TokenType::MINUS) {
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
        currentToken->type == TokenType::STAR
        || currentToken->type == TokenType::SLASH
        || currentToken->type == TokenType::DIV
        || currentToken->type == TokenType::MOD
    ) {
        const Token &op = *currentToken;
        advance();

        Node *otherFactor = parseFactor();

        factor = create<ArithmeticOperationNode>(op, *factor, *otherFactor);
    }

    return factor;
}

Node *Parser::parseFactor() {
    if (currentToken->type == TokenType::MINUS) {
        const Token &op = *currentToken;
        advance();

        Node *atom = parseAtom();
        atom = create<NegateNode>(op, *atom);

        return atom;
    }

    return parseAtom();
}

Node *Parser::parseAtom() {
    if (currentToken->type == TokenType::INTEGER) {
        return parseLiteral<IntegerNode>();
    } else if (currentToken->type == TokenType::REAL) {
        return parseLiteral<RealNode>();
    } else if (currentToken->type == TokenType::TRUE || currentToken->type == TokenType::FALSE) {
        return parseLiteral<BooleanNode>();
    } else if (currentToken->type == TokenType::CHAR) {
        return parseLiteral<CharNode>();
    } else if (currentToken->type == TokenType::STRING) {
        return parseLiteral<StringNode>();
    } else if (currentToken->type == TokenType::DATE) {
        return parseLiteral<DateNode>();
    } else if (currentToken->type == TokenType::IDENTIFIER) {
        if (compareNextType(1, TokenType::LPAREN)) return parseFunctionCall();
        
        const Token &identifier = *currentToken;
        auto resolver = parseIdentifierExpression();
        if (currentToken->type == TokenType::ASSIGNMENT) {
            const Token &token = *currentToken;
            advance();

            if (currentToken->type == TokenType::CARET) {
                const Token &refToken = *currentToken;
                advance();

                if (currentToken->type != TokenType::IDENTIFIER)
                    throw PSC::ExpectedTokenError(*currentToken, "identifier");
                auto valueResolver = parseIdentifierExpression();

                return create<PointerAssignNode>(refToken, std::move(resolver), std::move(valueResolver));
            } else {
                Node *expr = parseEvaluationExpression();
                return create<AssignNode>(token, *expr, std::move(resolver));
            }
        } else {
            return create<AccessNode>(identifier, std::move(resolver));
        }

        /*
        else if (compareNextType(1, TokenType::ASSIGNMENT)) {
            return parseAssignmentExpression();
        } else if (compareNextType(1, TokenType::LSQRBRACKET)) {
            return parseArrayOperation();
        } else {
            AccessNode *node = create<AccessNode>(*currentToken);
            advance();
            return node;
        }
        */
    } else if (currentToken->type == TokenType::LPAREN) {
        advance();
        Node *expr = parseEvaluationExpression();

        if (currentToken->type != TokenType::RPAREN)
            throw PSC::ExpectedTokenError(*currentToken, "')'");

        advance();
        return expr;
    } else if (currentToken->type == TokenType::DATA_TYPE) {
        return parseCast();
    } else if ((currentToken->type == TokenType::MOD || currentToken->type == TokenType::DIV) && compareNextType(1, TokenType::LPAREN)) {
        return parseModDivFn();
    }

    throw PSC::ExpectedTokenError(*currentToken, "value or expression");
}

Node *Parser::parseModDivFn() {
    const Token &token = *currentToken;
    advance();

    if (currentToken->type != TokenType::LPAREN) std::abort();
    advance();

    Node *expr = parseEvaluationExpression();

    if (currentToken->type != TokenType::COMMA)
        throw PSC::ExpectedTokenError(*currentToken, "','");
    advance();

    Node *otherExpr = parseEvaluationExpression();

    if (currentToken->type != TokenType::RPAREN)
        throw PSC::ExpectedTokenError(*currentToken, "')'");
    advance();

    return create<ArithmeticOperationNode>(token, *expr, *otherExpr);
}

Node *Parser::parseCast() {
    const Token &token = *currentToken;
    PSC::DataType type = getPSCType();
    advance();

    if (currentToken->type != TokenType::LPAREN)
        throw PSC::ExpectedTokenError(*currentToken, "'('");
    advance();

    Node *expr = parseEvaluationExpression();

    if (currentToken->type != TokenType::RPAREN)
        throw PSC::ExpectedTokenError(*currentToken, "')'");
    advance();

    return create<CastNode>(token, *expr, type);
}
