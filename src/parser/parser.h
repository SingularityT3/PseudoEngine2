#pragma once
#include <vector>
#include <concepts>
#include "tokens.h"
#include "nodes/node.h"
#include "psc/error.h"
#include "psc/scope/block.h"

class Parser {
private:
    const std::vector<Token*> *tokens;
    std::vector<Node*> nodes;
    std::vector<PSC::Block*> blocks;

    const Token *currentToken;
    int idx;

    void advance();

    PSC::DataType getPSCType();

    template<std::derived_from<Node> T, typename... Args>
    inline T *create(Args&&... args) {
        T *node = new T(std::forward<Args>(args)...);
        nodes.push_back(node);
        return node;
    }

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

    Node *parseCast();

    Node *parseFunctionCall();

    template<std::derived_from<Node> T>
    inline Node *parseLiteral() {
        T *node = create<T>(*currentToken);
        advance();
        return node;
    }
};
