#include "pch.h"

#include "lexer/tokens.h"

Token::Token(const TokenType &type, int line, int column, const std::string &value)
    : type(type), line(line), column(column), value(value)
{}

static const char *TT_STR[] = {
    "TT_INTEGER",
    "TT_REAL",
    "TT_CHAR",
    "TT_STRING",

    "TT_LPAREN",
    "TT_RPAREN",

    "TT_PLUS",
    "TT_MINUS",
    "TT_STAR",
    "TT_SLASH",
    "TT_DIV",
    "TT_MOD",

    "TT_AMPERSAND",

    "TT_ASSIGNMENT",
    "TT_COLON",
    "TT_COMMA",

    "TT_EQUALS",
    "TT_NOT_EQUALS",
    "TT_GREATER",
    "TT_LESSER",
    "TT_GREATER_EQUAL",
    "TT_LESSER_EQUAL",

    "TT_AND",
    "TT_OR",
    "TT_NOT",

    "TT_TRUE",
    "TT_FALSE",

    "TT_DECLARE",
    "TT_CONSTANT",
    "TT_IDENTIFIER",

    "TT_DATA_TYPE",
    "TT_ARRAY",
    "TT_LSQRBRACKET",
    "TT_RSQRBRACKET",

    "TT_TYPE",
    "TT_ENDTYPE",
    "TT_CARET",
    "TT_PERIOD",

    "TT_IF",
    "TT_THEN",
    "TT_ELSE",
    "TT_ENDIF",

    "TT_CASE",
    "TT_OF",
    "TT_OTHERWISE",
    "TT_ENDCASE",

    "TT_WHILE",
    "TT_DO",
    "TT_ENDWHILE",

    "TT_REPEAT",
    "TT_UNTIL",

    "TT_FOR",
    "TT_TO",
    "TT_STEP",
    "TT_NEXT",
    "TT_BREAK",
    "TT_CONTINUE",

    "TT_PROCEDURE",
    "TT_BYREF",
    "TT_BYVAL",
    "TT_ENDPROCEDURE",
    "TT_CALL",

    "TT_FUNCTION",
    "TT_ENDFUNCTION",
    "TT_RETURNS",
    "TT_RETURN",

    "TT_OUTPUT",
    "TT_INPUT",

    "TT_OPENFILE",
    "TT_READFILE",
    "TT_WRITEFILE",
    "TT_CLOSEFILE",

    "TT_READ",
    "TT_WRITE",
    "TT_APPEND",

    "TT_LINE_END",
    "TT_EXPRESSION_END"
};

std::ostream &operator<<(std::ostream &os, const TokenType &t) {
    os << TT_STR[(int) t];
    return os;
}

std::ostream &operator<<(std::ostream &os, const Token &t) {
    os << "Token{type: " << t.type << ", line: " << t.line << ", column: " << t.column << ", value: \"" << t.value << "\"}";
    return os;
}
