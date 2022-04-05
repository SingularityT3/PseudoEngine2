#include "parser/parser.h"

void Parser::advance() {
    if (++idx < (int) tokens->size()) currentToken = (*tokens)[idx];
}

PSC::DataType Parser::getPSCType() {
    if (currentToken->value == "INTEGER") {
        return PSC::DT_INTEGER;
    } else if (currentToken->value == "REAL") {
        return PSC::DT_REAL;
    } else if (currentToken->value == "BOOLEAN") {
        return PSC::DT_BOOLEAN;
    } else if (currentToken->value == "CHAR") {
        return PSC::DT_CHAR;
    } else if (currentToken->value == "STRING") {
        return PSC::DT_STRING;
    } else {
        std::abort();
    }
}

Parser::Parser(const std::vector<Token*> *tokens)
{
    setTokens(tokens);
}

Parser::~Parser() {
    for (Node *node : nodes) {
        delete node;
    }
    for (PSC::Block *block : blocks) {
        delete block;
    }
}

void Parser::setTokens(const std::vector<Token*> *tokens) {
    this->tokens = tokens;
    idx = -1;
    advance();
    nodes.reserve(tokens->size() * 0.6);
}

PSC::Block *Parser::parse() {
    PSC::Block *block = parseBlock(true);

    if (currentToken->type != TT_EXPRESSION_END)
        throw PSC::SyntaxError(*currentToken);

    return block;
}

PSC::Block *Parser::parseBlock(bool main) {
    PSC::Block *block = new PSC::Block();
    blocks.push_back(block);

    while (true) {
        while (currentToken->type == TT_LINE_END) advance();
        if (currentToken->type == TT_EXPRESSION_END
            || currentToken->type == TT_ENDIF
            || currentToken->type == TT_ELSE
            || currentToken->type == TT_ENDWHILE
            || currentToken->type == TT_UNTIL
            || currentToken->type == TT_NEXT
            || currentToken->type == TT_ENDPROCEDURE
            || currentToken->type == TT_ENDFUNCTION
        ) break;

        Node *node;
        if (currentToken->type == TT_PROCEDURE) {
            if (!main) throw PSC::SyntaxError(*currentToken, "Procedures can only be defined in the global scope");
            node = parseProcedure();
        } else if (currentToken->type == TT_FUNCTION) {
            if (!main) throw PSC::SyntaxError(*currentToken, "Functions can only be defined in the global scope");
            node = parseFunction();
        } else {
            node = parseExpression();
        }

        block->addNode(node);

        if (currentToken->type != TT_LINE_END && currentToken->type != TT_EXPRESSION_END) {
            throw PSC::SyntaxError(*currentToken);
        }
    }

    return block;
}

Node *Parser::parseExpression() {
    if (currentToken->type == TT_DECLARE) {
        return parseDeclareExpression();
    } else if (currentToken->type == TT_CONSTANT) {
        return parseConstDeclareExpression();
    } else if (currentToken->type == TT_IDENTIFIER) {
        if (idx + 1 >= (int) tokens->size() || (*tokens)[idx + 1]->type != TT_ASSIGNMENT)
            return parseEvaluationExpression();
        else
            return parseAssignmentExpression();
    } else if (currentToken->type == TT_IF) {
        return parseIfStatement();
    } else if (currentToken->type == TT_WHILE) {
        return parseWhileLoop();
    } else if (currentToken->type == TT_REPEAT) {
        return parseRepeatLoop();
    } else if (currentToken->type == TT_FOR) {
        return parseForLoop();
    } else if (currentToken->type == TT_CALL) {
        return parseCall();
    } else if (currentToken->type == TT_RETURN) {
        const Token &returnToken = *currentToken;
        advance();
        Node *evalExpr = parseEvaluationExpression();
        ReturnNode *node = new ReturnNode(returnToken, *evalExpr);
        nodes.push_back(node);
        return node;
    }

    return parseEvaluationExpression();
}
