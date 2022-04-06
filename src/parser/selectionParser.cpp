#include "parser/parser.h"

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

Node *Parser::parseCaseStatement() {
    const Token &caseToken = *currentToken;
    advance();

    if (currentToken->type != TT_OF)
        throw PSC::ExpectedTokenError(*currentToken, "'OF'");
    advance();

    if (currentToken->type != TT_IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "IDENTIFIER");
    AccessNode *variable = create<AccessNode>(*currentToken);
    advance();

    if (currentToken->type == TT_LINE_END) advance();

    CaseNode *caseNode = create<CaseNode>(caseToken, *variable);
    while (currentToken->type != TT_ENDCASE) {
        if (currentToken->type == TT_OTHERWISE) {
            advance();

            if (currentToken->type != TT_COLON)
                throw PSC::ExpectedTokenError(*currentToken, "':'");
            advance();

            PSC::Block *block = parseBlock(BT_CASE);
            caseNode->addCase(new OtherwiseCaseComponent(*block));

            if (currentToken->type != TT_ENDCASE)
                throw PSC::ExpectedTokenError(*currentToken, "'ENDCASE'");
            break;
        }

        Node *factor = parseFactor();
        Node *otherFactor = nullptr;

        if (currentToken->type == TT_TO) {
            advance();
            otherFactor = parseFactor();
        }

        if (currentToken->type != TT_COLON)
            throw PSC::ExpectedTokenError(*currentToken, "':'");
        advance();

        PSC::Block *block = parseBlock(BT_CASE);

        CaseComponent *component;
        if (otherFactor == nullptr) {
            component = new EqualsCaseComponent(*block, *factor);
        } else {
            component = new RangeCaseComponent(*block, *factor, *otherFactor);
        }
        caseNode->addCase(component);
    }

    advance(); // ENDCASE
    return caseNode;
}
