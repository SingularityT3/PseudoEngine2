#include "parser/parser.h"

void Parser::advance() {
    if (++idx < tokens->size()) currentToken = (*tokens)[idx];
}

PSC::DataType Parser::getPSCType() {
    if (currentToken->value == "INTEGER") {
        return PSC::DataType::INTEGER;
    } else if (currentToken->value == "REAL") {
        return PSC::DataType::REAL;
    } else if (currentToken->value == "BOOLEAN") {
        return PSC::DataType::BOOLEAN;
    } else if (currentToken->value == "CHAR") {
        return PSC::DataType::CHAR;
    } else if (currentToken->value == "STRING") {
        return PSC::DataType::STRING;
    } else {
        std::abort();
    }
}

bool Parser::compareNextType(unsigned int n, TokenType type) {
    if (idx + n >= tokens->size()) return false;
    return (*tokens)[idx + n]->type == type;
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

void Parser::setTokens(const std::vector<Token*> *_tokens) {
    tokens = _tokens;
    idx = SIZE_MAX; // overflow to 0 on advance()
    advance();
    nodes.reserve(tokens->size() / 2);
}

PSC::Block *Parser::parse() {
    PSC::Block *block = parseBlock(BlockType::MAIN);

    if (currentToken->type != TokenType::EXPRESSION_END)
        throw PSC::SyntaxError(*currentToken);

    return block;
}

PSC::Block *Parser::parseBlock(BlockType blockType) {
    PSC::Block *block;
    if (blockType == BlockType::MAIN) block = new PSC::MainBlock();
    else block = new PSC::Block();

    blocks.push_back(block);

    while (true) {
        while (currentToken->type == TokenType::LINE_END) advance();
        if (currentToken->type == TokenType::EXPRESSION_END
            || currentToken->type == TokenType::ENDIF
            || currentToken->type == TokenType::OTHERWISE
            || currentToken->type == TokenType::ENDCASE
            || currentToken->type == TokenType::ELSE
            || currentToken->type == TokenType::ENDWHILE
            || currentToken->type == TokenType::UNTIL
            || currentToken->type == TokenType::NEXT
            || currentToken->type == TokenType::ENDPROCEDURE
            || currentToken->type == TokenType::ENDFUNCTION
        ) break;

        if (blockType == BlockType::CASE) {
            if (compareNextType(1, TokenType::COLON) // <value> :
                || (currentToken->type == TokenType::MINUS && compareNextType(2, TokenType::COLON)) // - <value> :
                || compareNextType(1, TokenType::TO) // <value> TO
                || (currentToken->type == TokenType::MINUS && compareNextType(2, TokenType::TO)) // - <value> TO
            ) break;
        }

        Node *node;
        if (currentToken->type == TokenType::PROCEDURE) {
            if (blockType != BlockType::MAIN) throw PSC::SyntaxError(*currentToken, "Procedures can only be defined in the global scope");
            node = parseProcedure();
        } else if (currentToken->type == TokenType::FUNCTION) {
            if (blockType != BlockType::MAIN) throw PSC::SyntaxError(*currentToken, "Functions can only be defined in the global scope");
            node = parseFunction();
        } else {
            node = parseExpression();
        }

        block->addNode(node);

        if (currentToken->type != TokenType::LINE_END && currentToken->type != TokenType::EXPRESSION_END) {
            throw PSC::SyntaxError(*currentToken);
        }
    }

    return block;
}

Node *Parser::parseExpression() {
    if (currentToken->type == TokenType::DECLARE) {
        return parseDeclareExpression();
    } else if (currentToken->type == TokenType::CONSTANT) {
        return parseConstDeclareExpression();
    } else if (currentToken->type == TokenType::IF) {
        return parseIfStatement();
    } else if (currentToken->type == TokenType::CASE) {
        return parseCaseStatement();
    } else if (currentToken->type == TokenType::WHILE) {
        return parseWhileLoop();
    } else if (currentToken->type == TokenType::REPEAT) {
        return parseRepeatLoop();
    } else if (currentToken->type == TokenType::FOR) {
        return parseForLoop();
    } else if (currentToken->type == TokenType::CALL) {
        return parseCall();
    } else if (currentToken->type == TokenType::RETURN) {
        const Token &returnToken = *currentToken;
        advance();
        Node *evalExpr = parseEvaluationExpression();
        return create<ReturnNode>(returnToken, *evalExpr);
    } else if (currentToken->type == TokenType::BREAK) {
        const Token &breakToken = *currentToken;
        advance();
        return create<BreakNode>(breakToken);
    } else if (currentToken->type == TokenType::CONTINUE) {
        const Token &continueToken = *currentToken;
        advance();
        return create<ContinueNode>(continueToken);
    } else if (currentToken->type == TokenType::OUTPUT) {
        return parseOutput();
    } else if (currentToken->type == TokenType::INPUT) {
        return parseInput();
    }

    return parseEvaluationExpression();
}
