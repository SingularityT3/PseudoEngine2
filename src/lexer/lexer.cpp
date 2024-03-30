#include "pch.h"
#include <algorithm>

#include "lexer/lexer.h"

void Lexer::advance() {
    if (currentChar == '\n') {
        line++;
        column = 0;
    }

    if (++idx >= expr->size()) return;

    currentChar = (*expr)[idx];
    column++;
}

Lexer::Lexer(std::string *expr)
{
    setExpr(expr);
}

void Lexer::setExpr(std::string *_expr) {
    _expr->erase(std::remove(_expr->begin(), _expr->end(), '\r'), _expr->end());
    expr = _expr;
    idx = SIZE_MAX; // overflow to 0 on advance()
    currentChar = 0;
    line = 1;
    column = 0;
    advance();
}

char Lexer::getNextChar(size_t n) {
    if (idx + n >= expr->size()) return 0;
    return (*expr)[idx + n];
}

const std::vector<Token*> Lexer::makeTokens() {
    size_t reserve = expr->size() / 10;
    if (reserve < 1) reserve = 1;
    tokens.reserve(reserve);

    size_t currentTokensIdx = tokens.size();
    std::vector<Token*> currentTokens;

    while (idx < expr->size()) {
        if (currentChar == '+') {
            tokens.emplace_back(new Token(TokenType::PLUS, line, column));
        } else if (currentChar == '-') {
            tokens.emplace_back(new Token(TokenType::MINUS, line, column));
        } else if (currentChar == '*') {
            tokens.emplace_back(new Token(TokenType::STAR, line, column));
        } else if (currentChar == '/') {
            advance();
            if (idx >= expr->size() || currentChar != '/') {
                tokens.emplace_back(new Token(TokenType::SLASH, line, column));
            } else {
                int commentLine = line;
                while (line == commentLine && idx < expr->size()) advance();
            }
            continue;
        } else if (currentChar == '(') {
            if (idx > 0 && tokens.size() > 0 && (*expr)[idx-1] != ' ') {
                TokenType t = tokens[tokens.size()-1]->type;
                if (t == TokenType::INPUT) {
                    throw PSC::LexerError(line, column, "Unexpected '('\n'INTPUT' is statement(not a function) used as 'INPUT <var>' where <var> is a variable to store the input");
                } else if (t == TokenType::OUTPUT) {
                    throw PSC::LexerError(line, column, "Unexpected '('\n'OUTPUT' is statement(not a function) used as 'OUTPUT <value>' or 'OUTPUT <value1>, <value2>, ...'");
                } else if (t == TokenType::OPENFILE) {
                    throw PSC::LexerError(line, column, "Unexpected '('\n'OPENFILE' is statement(not a function) used as 'OPENFILE <filename> FOR <mode>'");
                } else if (t == TokenType::READFILE) {
                    throw PSC::LexerError(line, column, "Unexpected '('\n'READFILE' is statement(not a function) used as 'READFILE <filename>, <var>'");
                } else if (t == TokenType::WRITEFILE) {
                    throw PSC::LexerError(line, column, "Unexpected '('\n'WRITEFILE' is statement(not a function) used as 'WRITEFILE <filename>, <line>'");
                } else if (t == TokenType::CLOSEFILE) {
                    throw PSC::LexerError(line, column, "Unexpected '('\n'CLOSEFILE' is statement(not a function) used as 'CLOSEFILE <filename>'");
                }
            }
            tokens.emplace_back(new Token(TokenType::LPAREN, line, column));
        } else if (currentChar == ')') {
            tokens.emplace_back(new Token(TokenType::RPAREN, line, column));
        } else if (currentChar == '[') {
            tokens.emplace_back(new Token(TokenType::LSQRBRACKET, line, column));
        } else if (currentChar == ']') {
            tokens.emplace_back(new Token(TokenType::RSQRBRACKET, line, column));
        } else if (currentChar == '=') {
            advance();
            if (idx >= expr->size() || currentChar != '=') {
                tokens.emplace_back(new Token(TokenType::EQUALS, line, column - 1));
                continue;
            } else {
                throw PSC::LexerError(line, column - 1, "Invalid token '=='\nIf you want to check for equality, use single '=' instead");
            }
        } else if (currentChar == ':') {
            tokens.emplace_back(new Token(TokenType::COLON, line, column));
        } else if (currentChar == ',') {
            tokens.emplace_back(new Token(TokenType::COMMA, line, column));
        } else if (currentChar == '&') {
            tokens.emplace_back(new Token(TokenType::AMPERSAND, line, column));
        } else if (currentChar == '^') {
            tokens.emplace_back(new Token(TokenType::CARET, line, column));
        } else if (currentChar == '.') {
            tokens.emplace_back(new Token(TokenType::PERIOD, line, column));
        } else if (currentChar == '\'') {
            makeChar();
            continue;
        } else if (currentChar == '"') {
            makeString();
            continue;
        } else if (currentChar == '>') {
            advance();

            if (idx >= expr->size() || currentChar != '=') {
                tokens.emplace_back(new Token(TokenType::GREATER, line, column));
                continue;
            } else {
                tokens.emplace_back(new Token(TokenType::GREATER_EQUAL, line, column));
            }
        } else if (currentChar == '<') {
            advance();

            if (idx >= expr->size() || (currentChar != '=' && currentChar != '>' && currentChar != '-')) {
                tokens.emplace_back(new Token(TokenType::LESSER, line, column));
                continue;
            } else if (currentChar == '=') {
                tokens.emplace_back(new Token(TokenType::LESSER_EQUAL, line, column));
            } else if (currentChar == '>') {
                tokens.emplace_back(new Token(TokenType::NOT_EQUALS, line, column));
            } else {
                tokens.emplace_back(new Token(TokenType::ASSIGNMENT, line, column));
            }
        } else if (isalpha(currentChar)) {
            makeWord();
            continue;
        } else if (isdigit(currentChar)) {
            makeNumber();
            continue;
        } else if (currentChar == '\n') {
            tokens.emplace_back(new Token(TokenType::LINE_END, line, column));
        } else if (currentChar != ' ' && currentChar != '\t') {
            throw PSC::InvalidCharError(line, column, currentChar);
        }
        advance();
    }
    tokens.emplace_back(new Token(TokenType::EXPRESSION_END, line, column));

    currentTokens.reserve(tokens.size() - currentTokensIdx + 1);
    for (size_t i = currentTokensIdx; i < tokens.size(); i++) {
        currentTokens.push_back(tokens[i].get());
    }

    return currentTokens;
}
