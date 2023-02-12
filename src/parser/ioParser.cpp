#include "pch.h"

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

Node *Parser::parseOpenFile() {
    const Token &token = *currentToken;
    advance();

    Node *filename = parseStringExpression();

    if (currentToken->type != TokenType::FOR)
        throw PSC::ExpectedTokenError(*currentToken, "FOR");
    advance();

    PSC::FileMode mode;
    switch (currentToken->type) {
        case TokenType::READ:
            mode = PSC::FileMode::READ;
            break;
        case TokenType::WRITE:
            mode = PSC::FileMode::WRITE;
            break;
        case TokenType::APPEND:
            mode = PSC::FileMode::APPEND;
            break;
        default:
            throw PSC::ExpectedTokenError(*currentToken, "READ, WRITE or APPEND");
    }
    advance();

    Node *openFileNode = create<OpenFileNode>(token, *filename, mode);
    return openFileNode;
}

Node *Parser::parseReadFile() {
    const Token &token = *currentToken;
    advance();

    Node *filename = parseStringExpression();

    if (currentToken->type != TokenType::COMMA)
        throw PSC::ExpectedTokenError(*currentToken, "','");
    advance();

    if (currentToken->type != TokenType::IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "variable");
    
    Node *readFileNode = create<ReadFileNode>(token, *filename, *currentToken);
    advance();

    return readFileNode;
}

Node *Parser::parseWriteFile() {
    const Token &token = *currentToken;
    advance();

    Node *filename = parseStringExpression();

    if (currentToken->type != TokenType::COMMA)
        throw PSC::ExpectedTokenError(*currentToken, "','");
    advance();

    Node *data = parseEvaluationExpression();
    Node *writeFileNode = create<WriteFileNode>(token, *filename, *data);
    return writeFileNode;
}

Node *Parser::parseCloseFile() {
    const Token &token = *currentToken;
    advance();

    Node *filename = parseStringExpression();
    Node *closeFileNode = create<CloseFileNode>(token, *filename);
    return closeFileNode;
}
