#include "pch.h"
#include <ctype.h>

#include "lexer/lexer.h"

void Lexer::makeWord() {
    size_t startIdx = idx;
    int startColumn = column;

    for ( ; idx < expr->size(); advance()) {
        if (!isalnum(currentChar) && currentChar != '_') {
            break;
        }
    }

    std::string word = expr->substr(startIdx, idx - startIdx);
    if (word == "DIV") {
        tokens.emplace_back(new Token(TokenType::DIV, line, startColumn));
    } else if (word == "MOD") {
        tokens.emplace_back(new Token(TokenType::MOD, line, startColumn));
    }

    else if (word == "AND") {
        tokens.emplace_back(new Token(TokenType::AND, line, startColumn));
    } else if (word == "OR") {
        tokens.emplace_back(new Token(TokenType::OR, line, startColumn));
    } else if (word == "NOT") {
        tokens.emplace_back(new Token(TokenType::NOT, line, startColumn));
    } else if (word == "TRUE") {
        tokens.emplace_back(new Token(TokenType::TRUE, line, startColumn));
    } else if (word == "FALSE") {
        tokens.emplace_back(new Token(TokenType::FALSE, line, startColumn));
    }

    else if (word == "DECLARE") {
        tokens.emplace_back(new Token(TokenType::DECLARE, line, startColumn));
    } else if (word == "CONSTANT") {
        tokens.emplace_back(new Token(TokenType::CONSTANT, line, startColumn));
    } else if (word == "INTEGER" || word == "REAL" || word == "BOOLEAN" || word == "CHAR" || word == "STRING" || word == "DATE") {
        tokens.emplace_back(new Token(TokenType::DATA_TYPE, line, startColumn, word));
    } else if (word == "ARRAY") {
        tokens.emplace_back(new Token(TokenType::ARRAY, line, startColumn));
    }

    else if (word == "TYPE") {
        tokens.emplace_back(new Token(TokenType::TYPE, line, startColumn));
    } else if (word == "ENDTYPE") {
        tokens.emplace_back(new Token(TokenType::ENDTYPE, line, startColumn));
    }

    else if (word == "IF") {
        tokens.emplace_back(new Token(TokenType::IF, line, startColumn));
    } else if (word == "THEN") {
        tokens.emplace_back(new Token(TokenType::THEN, line, startColumn));
    } else if (word == "ELSE") {
        tokens.emplace_back(new Token(TokenType::ELSE, line, startColumn));
    } else if (word == "ENDIF") {
        tokens.emplace_back(new Token(TokenType::ENDIF, line, startColumn));
    }

    else if (word == "CASE") {
        tokens.emplace_back(new Token(TokenType::CASE, line, startColumn));
    } else if (word == "OF") {
        tokens.emplace_back(new Token(TokenType::OF, line, startColumn));
    } else if (word == "OTHERWISE") {
        tokens.emplace_back(new Token(TokenType::OTHERWISE, line, startColumn));
    } else if (word == "ENDCASE") {
        tokens.emplace_back(new Token(TokenType::ENDCASE, line, startColumn));
    }

    else if (word == "WHILE") {
        tokens.emplace_back(new Token(TokenType::WHILE, line, startColumn));
    } else if (word == "DO") {
        tokens.emplace_back(new Token(TokenType::DO, line, startColumn));
    } else if (word == "ENDWHILE") {
        tokens.emplace_back(new Token(TokenType::ENDWHILE, line, startColumn));
    }

    else if (word == "REPEAT") {
        tokens.emplace_back(new Token(TokenType::REPEAT, line, startColumn));
    } else if (word == "UNTIL") {
        tokens.emplace_back(new Token(TokenType::UNTIL, line, startColumn));
    }

    else if (word == "FOR") {
        tokens.emplace_back(new Token(TokenType::FOR, line, startColumn));
    } else if (word == "TO") {
        tokens.emplace_back(new Token(TokenType::TO, line, startColumn));
    } else if (word == "STEP") {
        tokens.emplace_back(new Token(TokenType::STEP, line, startColumn));
    } else if (word == "NEXT") {
        tokens.emplace_back(new Token(TokenType::NEXT, line, startColumn));
    }

    else if (word == "BREAK") {
        tokens.emplace_back(new Token(TokenType::BREAK, line, startColumn));
    } else if (word == "CONTINUE") {
        tokens.emplace_back(new Token(TokenType::CONTINUE, line, startColumn));
    }

    else if (word == "PROCEDURE") {
        tokens.emplace_back(new Token(TokenType::PROCEDURE, line, startColumn));
    } else if (word == "BYREF") {
        tokens.emplace_back(new Token(TokenType::BYREF, line, startColumn));
    } else if (word == "BYVAL") {
        tokens.emplace_back(new Token(TokenType::BYVAL, line, startColumn));
    } else if (word == "ENDPROCEDURE") {
        tokens.emplace_back(new Token(TokenType::ENDPROCEDURE, line, startColumn));
    } else if (word == "CALL") {
        tokens.emplace_back(new Token(TokenType::CALL, line, startColumn));
    }

    else if (word == "FUNCTION") {
        tokens.emplace_back(new Token(TokenType::FUNCTION, line, startColumn));
    } else if (word == "ENDFUNCTION") {
        tokens.emplace_back(new Token(TokenType::ENDFUNCTION, line, startColumn));
    } else if (word == "RETURNS") {
        tokens.emplace_back(new Token(TokenType::RETURNS, line, startColumn));
    } else if (word == "RETURN") {
        tokens.emplace_back(new Token(TokenType::RETURN, line, startColumn));
    }

    else if (word == "OUTPUT" || word == "PRINT") {
        tokens.emplace_back(new Token(TokenType::OUTPUT, line, startColumn));
    } else if (word == "INPUT") {
        tokens.emplace_back(new Token(TokenType::INPUT, line, startColumn));
    }

    else if (word == "OPENFILE") {
        tokens.emplace_back(new Token(TokenType::OPENFILE, line, startColumn));
    } else if (word == "READFILE") {
        tokens.emplace_back(new Token(TokenType::READFILE, line, startColumn));
    } else if (word == "WRITEFILE") {
        tokens.emplace_back(new Token(TokenType::WRITEFILE, line, startColumn));
    } else if (word == "CLOSEFILE") {
        tokens.emplace_back(new Token(TokenType::CLOSEFILE, line, startColumn));
    }

    else if (word == "READ") {
        tokens.emplace_back(new Token(TokenType::READ, line, startColumn));
    } else if (word == "WRITE") {
        tokens.emplace_back(new Token(TokenType::WRITE, line, startColumn));
    } else if (word == "APPEND") {
        tokens.emplace_back(new Token(TokenType::APPEND, line, startColumn));
    } else if (word == "RANDOM") {
        tokens.emplace_back(new Token(TokenType::RANDOM, line, startColumn));
    }

    else if (word == "SEEK") {
        tokens.emplace_back(new Token(TokenType::SEEK, line, startColumn));
    } else if (word == "GETRECORD") {
        tokens.emplace_back(new Token(TokenType::GETRECORD, line, startColumn));
    } else if (word == "PUTRECORD") {
        tokens.emplace_back(new Token(TokenType::PUTRECORD, line, startColumn));
    }

    else {
        tokens.emplace_back(new Token(TokenType::IDENTIFIER, line, startColumn, word));
    }
}

void Lexer::makeNumber() {
    int startIdx = idx;
    int startColumn = column;
    bool decimal = false;

    for ( ; idx < expr->size(); advance()) {
        if (currentChar == '.' && !decimal) {
            decimal = true;
            continue;
        }
        if (!isdigit(currentChar)) break;
    }

    TokenType type = decimal ? TokenType::REAL : TokenType::INTEGER;
    tokens.emplace_back(new Token(type, line, startColumn, expr->substr(startIdx, idx - startIdx)));
    // Check for date
    if (currentChar != '/' || decimal) return;

    int i = 0;
    char c = getNextChar(i);
    while (isdigit(c = getNextChar(++i))) {}
    if (c != '/' || !isdigit(c = getNextChar(++i))) return;

    for (int j = 0; j < i; j++) advance();
    while (isdigit(currentChar) && idx < expr->size()) advance();
    tokens.pop_back();
    tokens.emplace_back(new Token(TokenType::DATE, line, startColumn, expr->substr(startIdx, idx - startIdx)));
}

char escSeqFmt(char code) {
    switch (code) {
        case 'n':
            return '\n';
        case 't':
            return '\t';
        case '\'':
            return '\'';
        case '"':
            return '"';
        case '\\':
            return '\\';
        default:
            return -1;
    }
}

void Lexer::makeChar() {
    if (idx + 2 >= expr->size()) 
        throw PSC::LexerError(line, column, "Char must contain at least one character");

    int startColumn = column;
    advance();
    char c;

    if (currentChar == '\\') {
        advance();
        c = escSeqFmt(currentChar);
        if (c == -1) throw PSC::LexerError(line, column, "Invalid escape sequence");
    } else if (currentChar == '\'') {
        throw PSC::LexerError(line, column, "Char must contain at least one character");
    } else {
        c = currentChar;
    }

    if (idx + 1 >= expr->size() || (*expr)[idx + 1] != '\'')
        throw PSC::ExpectedQuotesError(line, column, false);
    advance();
    advance();

    std::string str;
    str += c;
    tokens.emplace_back(new Token(TokenType::CHAR, line, startColumn, str));
}

void Lexer::makeString() {
    int startColumn = column;
    advance();

    std::string str;
    while (currentChar != '"' && idx < expr->size()) {
        if (currentChar == '\\') {
            advance();
            char c = escSeqFmt(currentChar);
            if (c == -1) throw PSC::LexerError(line, column, "Invalid escape sequence");
            str += c;
        } else {
            str += currentChar;
        }
        advance();
    }

    if (idx >= expr->size() || currentChar != '"')
        throw PSC::ExpectedQuotesError(line, column, true);
    advance();

    tokens.emplace_back(new Token(TokenType::STRING, line, startColumn, str));
}
