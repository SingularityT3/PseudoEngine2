#include "pch.h"

#include "parser/parser.h"

Node *Parser::parseForLoop() {
    const Token &forToken = *currentToken;
    advance();

    if (currentToken->type != TokenType::IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "identifier");
    const Token &iterator = *currentToken;
    advance();

    if (currentToken->type != TokenType::ASSIGNMENT)
        throw PSC::ExpectedTokenError(*currentToken, "'<-'");
    advance();

    Node *start = parseArithmeticExpression();

    if (currentToken->type != TokenType::TO)
        throw PSC::ExpectedTokenError(*currentToken, "'TO'");
    advance();

    Node *stop = parseArithmeticExpression();

    Node *step;
    if (currentToken->type == TokenType::STEP) {
        advance();
        step = parseArithmeticExpression();
    } else {
        step = nullptr;
    }

    PSC::Block *block = parseBlock();

    if (currentToken->type != TokenType::NEXT)
        throw PSC::ExpectedTokenError(*currentToken, "'NEXT'");
    advance();

    if (currentToken->type == TokenType::IDENTIFIER) {
        if (currentToken->value != iterator.value)
            throw PSC::ExpectedTokenError(*currentToken, "'" + iterator.value + "'");
        advance();
    }

    return create<ForLoopNode>(forToken, iterator, *start, *stop, step, block);
}

Node *Parser::parseRepeatLoop() {
    const Token &repeatToken = *currentToken;
    advance();

    PSC::Block *block = parseBlock();

    if (currentToken->type != TokenType::UNTIL)
        throw PSC::ExpectedTokenError(*currentToken, "'UNTIL'");
    advance();

    Node *condition = parseEvaluationExpression();

    return create<RepeatUntilNode>(repeatToken, *condition, *block);
}

Node *Parser::parseWhileLoop() {
    const Token &whileToken = *currentToken;
    advance();

    Node *condition = parseEvaluationExpression();

    if (currentToken->type == TokenType::LINE_END) advance();
    if (currentToken->type == TokenType::DO) advance();

    PSC::Block *block = parseBlock();

    if (currentToken->type != TokenType::ENDWHILE)
        throw PSC::ExpectedTokenError(*currentToken, "'ENDWHILE'");
    advance();

    return create<WhileLoopNode>(whileToken, *condition, *block);
}
