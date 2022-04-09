#include "lexer/lexer.h"

void Lexer::advance() {
    if (currentChar == '\n') {
        line++;
        column = 0;
    }

    if (++idx >= (int) expr->size()) return;

    currentChar = (*expr)[idx];
    column++;
}

Lexer::Lexer(std::string *expr)
{
    setExpr(expr);
}

Lexer::~Lexer() {
    for (Token *t : tokens) {
        delete t;
    }
}

void Lexer::setExpr(std::string *expr) {
    expr->erase(std::remove(expr->begin(), expr->end(), '\r'), expr->end());
    this->expr = expr;
    idx = -1;
    currentChar = 0;
    line = 1;
    column = 0;
    advance();
}

const std::vector<Token*> Lexer::makeTokens() {
    int reserve = expr->size() / 10;
    if (reserve < 1) reserve = 1;
    tokens.reserve(reserve);

    int currentTokensIdx = tokens.size();
    std::vector<Token*> currentTokens;

    while (idx < (int) expr->size()) {
        if (currentChar == '+') {
            tokens.push_back(new Token(TT_PLUS, line, column));
        } else if (currentChar == '-') {
            tokens.push_back(new Token(TT_MINUS, line, column));
        } else if (currentChar == '*') {
            tokens.push_back(new Token(TT_STAR, line, column));
        } else if (currentChar == '/') {
            advance();
            if (idx >= (int) expr->size() || currentChar != '/') {
                tokens.push_back(new Token(TT_SLASH, line, column));
            } else {
                int commentLine = line;
                while (line == commentLine && idx < (int) expr->size()) advance();
            }
            continue;
        } else if (currentChar == '(') {
            tokens.push_back(new Token(TT_LPAREN, line, column));
        } else if (currentChar == ')') {
            tokens.push_back(new Token(TT_RPAREN, line, column));
        } else if (currentChar == '[') {
            tokens.push_back(new Token(TT_LSQRBRACKET, line, column));
        } else if (currentChar == ']') {
            tokens.push_back(new Token(TT_RSQRBRACKET, line, column));
        } else if (currentChar == '=') {
            tokens.push_back(new Token(TT_EQUALS, line, column));
        } else if (currentChar == ':') {
            tokens.push_back(new Token(TT_COLON, line, column));
        } else if (currentChar == ',') {
            tokens.push_back(new Token(TT_COMMA, line, column));
        } else if (currentChar == '&') {
            tokens.push_back(new Token(TT_AMPERSAND, line, column));
        } else if (currentChar == '\'') {
            makeChar();
            continue;
        } else if (currentChar == '"') {
            makeString();
            continue;
        } else if (currentChar == '>') {
            advance();

            if (idx >= (int) expr->size() || currentChar != '=') {
                tokens.push_back(new Token(TT_GREATER, line, column));
                continue;
            } else {
                tokens.push_back(new Token(TT_GREATER_EQUAL, line, column));
            }
        } else if (currentChar == '<') {
            advance();

            if (idx >= (int) expr->size() || (currentChar != '=' && currentChar != '>' && currentChar != '-')) {
                tokens.push_back(new Token(TT_LESSER, line, column));
                continue;
            } else if (currentChar == '=') {
                tokens.push_back(new Token(TT_LESSER_EQUAL, line, column));
            } else if (currentChar == '>') {
                tokens.push_back(new Token(TT_NOT_EQUALS, line, column));
            } else {
                tokens.push_back(new Token(TT_ASSIGNMENT, line, column));
            }
        } else if (isalpha(currentChar)) {
            makeWord();
            continue;
        } else if (isdigit(currentChar)) {
            makeNumber();
            continue;
        } else if (currentChar == '\n') {
            tokens.push_back(new Token(TT_LINE_END, line, column));
        } else if (currentChar != ' ' && currentChar != '\t') {
            throw PSC::InvalidCharError(line, column, currentChar);
        }
        advance();
    }
    tokens.push_back(new Token(TT_EXPRESSION_END, line, column));

    currentTokens.reserve(tokens.size() - currentTokensIdx + 1);
    for (int i = currentTokensIdx; i < (int) tokens.size(); i++) {
        currentTokens.push_back(tokens[i]);
    }

    return currentTokens;
}
