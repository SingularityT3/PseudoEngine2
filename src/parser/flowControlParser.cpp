#include "parser/parser.h"

Node *Parser::parseForLoop() {
    const Token &forToken = *currentToken;
    advance();

    if (currentToken->type != TT_IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "identifier");
    const Token &iterator = *currentToken;
    advance();

    if (currentToken->type != TT_ASSIGNMENT)
        throw PSC::ExpectedTokenError(*currentToken, "'<-'");
    advance();

    Node *start = parseArithmeticExpression();

    if (currentToken->type != TT_TO)
        throw PSC::ExpectedTokenError(*currentToken, "'TO'");
    advance();

    Node *stop = parseArithmeticExpression();

    Node *step;
    if (currentToken->type == TT_STEP) {
        advance();
        step = parseArithmeticExpression();
    } else {
        step = nullptr;
    }

    PSC::Block *block = parseBlock();

    if (currentToken->type != TT_NEXT)
        throw PSC::ExpectedTokenError(*currentToken, "'NEXT'");
    advance();

    if (currentToken->type == TT_IDENTIFIER) {
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

    if (currentToken->type != TT_UNTIL)
        throw PSC::ExpectedTokenError(*currentToken, "'UNTIL'");
    advance();

    Node *condition = parseEvaluationExpression();

    return create<RepeatUntilNode>(repeatToken, *condition, *block);
}

Node *Parser::parseWhileLoop() {
    const Token &whileToken = *currentToken;
    advance();

    Node *condition = parseEvaluationExpression();

    if (currentToken->type == TT_LINE_END) advance();
    if (currentToken->type != TT_DO)
        throw PSC::ExpectedTokenError(*currentToken, "'DO'");
    advance();

    PSC::Block *block = parseBlock();

    if (currentToken->type != TT_ENDWHILE)
        throw PSC::ExpectedTokenError(*currentToken, "'ENDWHILE'");
    advance();

    return create<WhileLoopNode>(whileToken, *condition, *block);
}

Node *Parser::parseIfStatement() {
    const Token &ifToken = *currentToken;
    advance();

    Node *condition = parseEvaluationExpression();

    if (currentToken->type == TT_LINE_END) advance();
    if (currentToken->type != TT_THEN)
        throw PSC::ExpectedTokenError(*currentToken, "'THEN'");
    advance();

    PSC::Block *block = parseBlock();

    IfStatementNode *ifNode = create<IfStatementNode>(ifToken);
    ifNode->components.emplace_back(condition, *block);

    while (currentToken->type == TT_ELSE) {
        advance();
        if (currentToken->type == TT_IF) {
            advance();
            condition = parseEvaluationExpression();

            if (currentToken->type == TT_LINE_END) advance();
            if (currentToken->type != TT_THEN)
                throw PSC::ExpectedTokenError(*currentToken, "'THEN'");
            advance();

            block = parseBlock();
            ifNode->components.emplace_back(condition, *block);
        } else {
            block = parseBlock();
            ifNode->components.emplace_back(nullptr, *block);
            break;
        }
    }

    if (currentToken->type != TT_ENDIF)
        throw PSC::ExpectedTokenError(*currentToken, "'ENDIF'");
    advance();

    return ifNode;
}
