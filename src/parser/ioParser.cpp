#include "parser/parser.h"

Node *Parser::parseOutput() {
    OutputNode *outputNode = create<OutputNode>(*currentToken);
    advance();

    Node *expr = parseEvaluationExpression();
    outputNode->nodes.push_back(expr);

    while (currentToken->type == TT_COMMA) {
        advance();

        expr = parseEvaluationExpression();
        outputNode->nodes.push_back(expr);
    }

    return outputNode;
}

Node *Parser::parseInput() {
    const Token &inputToken = *currentToken;
    advance();

    if (currentToken->type != TT_IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "variable");

    InputNode *inputNode = create<InputNode>(inputToken, *currentToken);
    advance();

    return inputNode;
}
