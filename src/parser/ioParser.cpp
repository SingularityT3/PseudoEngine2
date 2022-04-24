#include "parser/parser.h"

Node *Parser::parseOutput() {
    const Token &outputToken = *currentToken;
    advance();

    std::vector<Node*> nodes;
    nodes.push_back(parseEvaluationExpression());

    while (currentToken->type == TokenType::COMMA) {
        advance();
        nodes.push_back(parseEvaluationExpression());
    }

    return create<OutputNode>(outputToken, std::move(nodes));
}

Node *Parser::parseInput() {
    const Token &inputToken = *currentToken;
    advance();

    if (currentToken->type != TokenType::IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "variable");

    InputNode *inputNode = create<InputNode>(inputToken, *currentToken);
    advance();

    return inputNode;
}
