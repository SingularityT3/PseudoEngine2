#pragma once
#include <iostream>
#include <string>

enum class TokenType {
    INTEGER,
    REAL,
    CHAR,
    STRING,
    DATE,

    RPAREN,
    LPAREN,

    PLUS,
    MINUS,
    STAR,
    SLASH,
    DIV,
    MOD,

    AMPERSAND,

    ASSIGNMENT,
    COLON,
    COMMA,

    EQUALS,
    NOT_EQUALS,
    GREATER,
    LESSER,
    GREATER_EQUAL,
    LESSER_EQUAL,

    AND,
    OR,
    NOT,

    TRUE,
    FALSE,

    DECLARE,
    CONSTANT,
    IDENTIFIER,

    DATA_TYPE,
    ARRAY,
    LSQRBRACKET,
    RSQRBRACKET,

    TYPE,
    ENDTYPE,
    CARET,
    PERIOD,

    IF,
    THEN,
    ELSE,
    ENDIF,

    CASE,
    OF,
    OTHERWISE,
    ENDCASE,

    WHILE,
    DO,
    ENDWHILE,

    REPEAT,
    UNTIL,

    FOR,
    TO,
    STEP,
    NEXT,

    BREAK,
    CONTINUE,

    PROCEDURE,
    BYREF,
    BYVAL,
    ENDPROCEDURE,
    CALL,

    FUNCTION,
    ENDFUNCTION,
    RETURNS,
    RETURN,

    OUTPUT,
    INPUT,

    OPENFILE,
    READFILE,
    WRITEFILE,
    CLOSEFILE,

    READ,
    WRITE,
    APPEND,

    LINE_END,
    EXPRESSION_END
};

struct Token {
    TokenType type;
    int line, column;
    std::string value;

    Token(const TokenType &type, int line, int column, const std::string &value = "");
};

std::ostream &operator<<(std::ostream &os, const TokenType &t);

std::ostream &operator<<(std::ostream &os, const Token &t);
