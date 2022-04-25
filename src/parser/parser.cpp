#include "pch.h"

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

    blocks.emplace_back(block);

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

        if (blockType == BlockType::CASE && currentToken->type != TokenType::DECLARE) {
            bool endBlock = false;
            for (size_t i = 1; i + idx < tokens->size(); i++) {
                if (compareNextType(i, TokenType::COLON)) {
                    endBlock = true;
                    break;
                } else if (compareNextType(i, TokenType::LINE_END)) {
                    break;
                }
            }
            if (endBlock) break;
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
    switch (currentToken->type) {
        case TokenType::DECLARE:
            return parseDeclareExpression();
        case TokenType::CONSTANT:
            return parseConstDeclareExpression();
        case TokenType::IF:
            return parseIfStatement();
        case TokenType::CASE:
            return parseCaseStatement();
        case TokenType::WHILE:
            return parseWhileLoop();
        case TokenType::REPEAT:
            return parseRepeatLoop();
        case TokenType::FOR:
            return parseForLoop();
        case TokenType::CALL:
            return parseCall();
        case TokenType::OUTPUT:
            return parseOutput();
        case TokenType::INPUT:
            return parseInput();
        case TokenType::RETURN: {
            const Token &returnToken = *currentToken;
            advance();
            Node *evalExpr = parseEvaluationExpression();
            return create<ReturnNode>(returnToken, *evalExpr);
        }
        case TokenType::BREAK: {
            const Token &breakToken = *currentToken;
            advance();
            return create<BreakNode>(breakToken);
        }
        case TokenType::CONTINUE: {
            const Token &continueToken = *currentToken;
            advance();
            return create<ContinueNode>(continueToken);
        }
        default:
            return parseEvaluationExpression();
    }
}
