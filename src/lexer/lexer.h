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
    int idx;

    void advance();

    void makeWord();

    void makeNumber();

    void makeChar();

    void makeString();

public:
    Lexer() = default;

    Lexer(const std::string *expr);

    ~Lexer();

    void setExpr(const std::string *expr);

    const std::vector<Token*> makeTokens();
};
