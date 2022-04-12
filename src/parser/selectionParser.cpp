#include "parser/parser.h"

Node *Parser::parseIfStatement() {
    const Token &ifToken = *currentToken;
    advance();

    Node *condition = parseEvaluationExpression();

    if (currentToken->type == TokenType::LINE_END) advance();
    if (currentToken->type != TokenType::THEN)
        throw PSC::ExpectedTokenError(*currentToken, "'THEN'");
    advance();

    PSC::Block *block = parseBlock();

    IfStatementNode *ifNode = create<IfStatementNode>(ifToken);
    ifNode->components.emplace_back(condition, *block);

    while (currentToken->type == TokenType::ELSE) {
        advance();
        if (currentToken->type == TokenType::IF) {
            advance();
            condition = parseEvaluationExpression();

            if (currentToken->type == TokenType::LINE_END) advance();
            if (currentToken->type != TokenType::THEN)
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

    if (currentToken->type != TokenType::ENDIF)
        throw PSC::ExpectedTokenError(*currentToken, "'ENDIF'");
    advance();

    return ifNode;
}

Node *Parser::parseCaseStatement() {
    const Token &caseToken = *currentToken;
    advance();

    if (currentToken->type != TokenType::OF)
        throw PSC::ExpectedTokenError(*currentToken, "'OF'");
    advance();

    if (currentToken->type != TokenType::IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "IDENTIFIER");
    AccessNode *variable = create<AccessNode>(*currentToken);
    advance();

    if (currentToken->type == TokenType::LINE_END) advance();

    CaseNode *caseNode = create<CaseNode>(caseToken, *variable);
    while (currentToken->type != TokenType::ENDCASE) {
        if (currentToken->type == TokenType::OTHERWISE) {
            advance();

            if (currentToken->type != TokenType::COLON)
                throw PSC::ExpectedTokenError(*currentToken, "':'");
            advance();

            PSC::Block *block = parseBlock(BlockType::CASE);
            caseNode->addCase(new OtherwiseCaseComponent(*block));

            if (currentToken->type != TokenType::ENDCASE)
                throw PSC::ExpectedTokenError(*currentToken, "'ENDCASE'");
            break;
        }

        Node *expr = parseEvaluationExpression();
        Node *otherExpr = nullptr;

        if (currentToken->type == TokenType::TO) {
            advance();
            otherExpr = parseEvaluationExpression();
        }

        if (currentToken->type != TokenType::COLON)
            throw PSC::ExpectedTokenError(*currentToken, "':'");
        advance();

        PSC::Block *block = parseBlock(BlockType::CASE);

        CaseComponent *component;
        if (otherExpr == nullptr) {
            component = new EqualsCaseComponent(*block, *expr);
        } else {
            component = new RangeCaseComponent(*block, *expr, *otherExpr);
        }
        caseNode->addCase(component);
    }

    advance(); // ENDCASE
    return caseNode;
}
