#pragma once
#include <string>
#include <vector>
#include "tokens.h"
#include "psc/error.h"

class Lexer {
private:
    const std::string *expr;
    std::vector<Token*> tokens;

    char currentChar;
    int line;
    int column;
    size_t idx;

    void advance();

    void makeWord();

    void makeNumber();

    void makeChar();

    void makeString();

public:
    Lexer() = default;

    Lexer(std::string *expr);

    ~Lexer();

    void setExpr(std::string *expr);

    const std::vector<Token*> makeTokens();
};
