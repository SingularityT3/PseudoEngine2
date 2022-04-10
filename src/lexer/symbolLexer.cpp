#include <ctype.h>
#include "lexer/lexer.h"

void Lexer::makeWord() {
    int startIdx = idx;
    int startColumn = column;

    for ( ; idx < expr->size(); advance()) {
        if (!isalnum(currentChar) && currentChar != '_') {
            break;
        }
    }

    std::string word = expr->substr(startIdx, idx - startIdx);
    if (word == "DIV") {
        tokens.push_back(new Token(TokenType::DIV, line, startColumn));
    } else if (word == "MOD") {
        tokens.push_back(new Token(TokenType::MOD, line, startColumn));
    }

    else if (word == "AND") {
        tokens.push_back(new Token(TokenType::AND, line, startColumn));
    } else if (word == "OR") {
        tokens.push_back(new Token(TokenType::OR, line, startColumn));
    } else if (word == "NOT") {
        tokens.push_back(new Token(TokenType::NOT, line, startColumn));
    } else if (word == "TRUE") {
        tokens.push_back(new Token(TokenType::TRUE, line, startColumn));
    } else if (word == "FALSE") {
        tokens.push_back(new Token(TokenType::FALSE, line, startColumn));
    }

    else if (word == "DECLARE") {
        tokens.push_back(new Token(TokenType::DECLARE, line, startColumn));
    } else if (word == "CONSTANT") {
        tokens.push_back(new Token(TokenType::CONSTANT, line, startColumn));
    } else if (word == "INTEGER" || word == "REAL" || word == "BOOLEAN" || word == "CHAR" || word == "STRING") {
        tokens.push_back(new Token(TokenType::DATA_TYPE, line, startColumn, word));
    } else if (word == "ARRAY") {
        tokens.push_back(new Token(TokenType::ARRAY, line, startColumn));
    }

    else if (word == "IF") {
        tokens.push_back(new Token(TokenType::IF, line, startColumn));
    } else if (word == "THEN") {
        tokens.push_back(new Token(TokenType::THEN, line, startColumn));
    } else if (word == "ELSE") {
        tokens.push_back(new Token(TokenType::ELSE, line, startColumn));
    } else if (word == "ENDIF") {
        tokens.push_back(new Token(TokenType::ENDIF, line, startColumn));
    }

    else if (word == "CASE") {
        tokens.push_back(new Token(TokenType::CASE, line, startColumn));
    } else if (word == "OF") {
        tokens.push_back(new Token(TokenType::OF, line, startColumn));
    } else if (word == "OTHERWISE") {
        tokens.push_back(new Token(TokenType::OTHERWISE, line, startColumn));
    } else if (word == "ENDCASE") {
        tokens.push_back(new Token(TokenType::ENDCASE, line, startColumn));
    }

    else if (word == "WHILE") {
        tokens.push_back(new Token(TokenType::WHILE, line, startColumn));
    } else if (word == "DO") {
        tokens.push_back(new Token(TokenType::DO, line, startColumn));
    } else if (word == "ENDWHILE") {
        tokens.push_back(new Token(TokenType::ENDWHILE, line, startColumn));
    }

    else if (word == "REPEAT") {
        tokens.push_back(new Token(TokenType::REPEAT, line, startColumn));
    } else if (word == "UNTIL") {
        tokens.push_back(new Token(TokenType::UNTIL, line, startColumn));
    }

    else if (word == "FOR") {
        tokens.push_back(new Token(TokenType::FOR, line, startColumn));
    } else if (word == "TO") {
        tokens.push_back(new Token(TokenType::TO, line, startColumn));
    } else if (word == "STEP") {
        tokens.push_back(new Token(TokenType::STEP, line, startColumn));
    } else if (word == "NEXT") {
        tokens.push_back(new Token(TokenType::NEXT, line, startColumn));
    }

    else if (word == "PROCEDURE") {
        tokens.push_back(new Token(TokenType::PROCEDURE, line, startColumn));
    } else if (word == "BYREF") {
        tokens.push_back(new Token(TokenType::BYREF, line, startColumn));
    } else if (word == "BYVAL") {
        tokens.push_back(new Token(TokenType::BYVAL, line, startColumn));
    } else if (word == "ENDPROCEDURE") {
        tokens.push_back(new Token(TokenType::ENDPROCEDURE, line, startColumn));
    } else if (word == "CALL") {
        tokens.push_back(new Token(TokenType::CALL, line, startColumn));
    }

    else if (word == "FUNCTION") {
        tokens.push_back(new Token(TokenType::FUNCTION, line, startColumn));
    } else if (word == "ENDFUNCTION") {
        tokens.push_back(new Token(TokenType::ENDFUNCTION, line, startColumn));
    } else if (word == "RETURNS") {
        tokens.push_back(new Token(TokenType::RETURNS, line, startColumn));
    } else if (word == "RETURN") {
        tokens.push_back(new Token(TokenType::RETURN, line, startColumn));
    }

    else if (word == "OUTPUT" || word == "PRINT") {
        tokens.push_back(new Token(TokenType::OUTPUT, line, startColumn));
    } else if (word == "INPUT" || word == "READ") {
        tokens.push_back(new Token(TokenType::INPUT, line, startColumn));
    }

    else {
        tokens.push_back(new Token(TokenType::IDENTIFIER, line, startColumn, word));
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
    tokens.push_back(new Token(type, line, startColumn, expr->substr(startIdx, idx - startIdx).c_str()));
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
        throw PSC::LexerError(line, column, "Char must contain at least once character");

    int startColumn = column;
    advance();
    char c;

    if (currentChar == '\\') {
        advance();
        c = escSeqFmt(currentChar);
        if (c == -1) throw PSC::LexerError(line, column, "Invalid escape sequence");
    } else if (currentChar == '\'') {
        throw PSC::LexerError(line, column, "Char must contain at least once character");
    } else {
        c = currentChar;
    }

    if (idx + 1 >= expr->size() || (*expr)[idx + 1] != '\'')
        throw PSC::ExpectedQuotesError(line, column, false);
    advance();
    advance();

    std::string str;
    str += c;
    tokens.push_back(new Token(TokenType::CHAR, line, startColumn, str));
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

    tokens.push_back(new Token(TokenType::STRING, line, startColumn, str));
}
