#pragma once
#include <vector>
#include "tokens.h"
#include "nodes/node.h"
#include "psc/error.h"
#include "psc/block.h"

class Parser {
private:
    const std::vector<Token*> *tokens;
    std::vector<Node*> nodes;
    std::vector<PSC::Block*> blocks;

    const Token *currentToken;
    int idx;

    void advance();

    PSC::DataType getPSCType();

public:
    Parser() = default;

    Parser(const std::vector<Token*> *tokens);

    ~Parser();

    PSC::Block *parse();

    void setTokens(const std::vector<Token*> *tokens);

private:
    PSC::Block *parseBlock(bool main = false);

    Node *parseFunction();

    Node *parseCall();

    Node *parseProcedure();

    Node *parseExpression();

    Node *parseForLoop();

    Node *parseRepeatLoop();

    Node *parseWhileLoop();

    Node *parseIfStatement();

    Node *parseDeclareExpression();

    Node *parseConstDeclareExpression();

    Node *parseAssignmentExpression();

    Node *parseEvaluationExpression();

    Node *parseLogicalExpression();

    Node *parseComparisonExpression();

    Node *parseStringExpression();

    Node *parseArithmeticExpression();

    Node *parseTerm();

    Node *parseFactor();

    Node *parseAtom();

    Node *parseFunctionCall();

    template<typename T>
    Node *parseLiteral() {
        T *node = new T(*currentToken);
        nodes.push_back(node);
        advance();
        return node;
    }
};
