#pragma once
#include <iostream>
#include <string>

enum TokenType {
    TT_INTEGER,
    TT_REAL,
    TT_CHAR,
    TT_STRING,

    TT_RPAREN,
    TT_LPAREN,

    TT_PLUS,
    TT_MINUS,
    TT_STAR,
    TT_SLASH,
    TT_DIV,
    TT_MOD,

    TT_AMPERSAND,

    TT_ASSIGNMENT,
    TT_COLON,
    TT_COMMA,

    TT_EQUALS,
    TT_NOT_EQUALS,
    TT_GREATER,
    TT_LESSER,
    TT_GREATER_EQUAL,
    TT_LESSER_EQUAL,

    TT_AND,
    TT_OR,
    TT_NOT,

    TT_TRUE,
    TT_FALSE,

    TT_DECLARE,
    TT_CONSTANT,
    TT_IDENTIFIER,

    TT_DATA_TYPE,

    TT_IF,
    TT_THEN,
    TT_ELSE,
    TT_ENDIF,

    TT_CASE,
    TT_OF,
    TT_OTHERWISE,
    TT_ENDCASE,

    TT_WHILE,
    TT_DO,
    TT_ENDWHILE,

    TT_REPEAT,
    TT_UNTIL,

    TT_FOR,
    TT_TO,
    TT_STEP,
    TT_NEXT,

    TT_PROCEDURE,
    TT_BYREF,
    TT_BYVAL,
    TT_ENDPROCEDURE,
    TT_CALL,

    TT_FUNCTION,
    TT_ENDFUNCTION,
    TT_RETURNS,
    TT_RETURN,

    TT_OUTPUT,
    TT_INPUT,

    TT_LINE_END,
    TT_EXPRESSION_END
};

struct Token {
    TokenType type;
    int line, column;
    std::string value;

    Token(const TokenType &type, int line, int column, const std::string &value = "");
};

std::ostream &operator<<(std::ostream &os, const TokenType &t);

std::ostream &operator<<(std::ostream &os, const Token &t);
