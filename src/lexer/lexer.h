#pragma once
#include <string>
#include <vector>
#include <memory>
#include "lexer/tokens.h"
#include "psc/error.h"

class Lexer {
private:
    const std::string *expr;
    std::vector<std::unique_ptr<Token>> tokens;

    char currentChar;
    int line;
    int column;
    size_t idx;

    void advance();

    void makeWord();

    void makeNumber();

    void makeChar();

    void makeString();

    char getNextChar(size_t n);

public:
    Lexer() = default;

    Lexer(std::string *expr);

    void setExpr(std::string *_expr);

    const std::vector<Token*> makeTokens();
};
