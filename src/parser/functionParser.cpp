#include "pch.h"

#include "parser/parser.h"

Node *Parser::parseFunction() {
    const Token &functionToken = *currentToken;
    advance();

    if (currentToken->type != TokenType::IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "identifier");
    const Token &identifier = *currentToken;
    advance();

    PSC::Function *function = new PSC::Function(identifier.value);
    function->defToken = &functionToken;
    function->byRef = false;

    FunctionNode *node = create<FunctionNode>(functionToken, function);

    if (currentToken->type == TokenType::LPAREN) {
        advance();

        if (currentToken->type == TokenType::BYREF) {
            function->byRef = true;
            advance();
        } else if (currentToken->type == TokenType::BYVAL) {
            advance();
        }

        while (currentToken->type != TokenType::RPAREN) {
            if (function->parameters.size() > 0) {
                if (currentToken->type != TokenType::COMMA)
                    throw PSC::ExpectedTokenError(*currentToken, "','");
                advance();
            }

            if (currentToken->type != TokenType::IDENTIFIER)
                throw PSC::ExpectedTokenError(*currentToken, "identifier or ')'");
            const std::string &paramName = currentToken->value;
            advance();

            if (currentToken->type != TokenType::COLON)
                throw PSC::ExpectedTokenError(*currentToken, "':'");
            advance();

            if (currentToken->type != TokenType::DATA_TYPE)
                throw PSC::ExpectedTokenError(*currentToken, "data type");

            PSC::DataType type = getPSCType();
            advance();

            function->parameters.emplace_back(paramName, type);
        }
        advance();
    }

    if (currentToken->type == TokenType::LINE_END) advance();
    if (currentToken->type != TokenType::RETURNS)
        throw PSC::ExpectedTokenError(*currentToken, "'RETURNS'");
    advance();

    if (currentToken->type != TokenType::DATA_TYPE)
        throw PSC::ExpectedTokenError(*currentToken, "data type");

    function->returnType = getPSCType();
    advance();

    PSC::Block *block = parseBlock();
    if (currentToken->type != TokenType::ENDFUNCTION)
        throw PSC::ExpectedTokenError(*currentToken, "'ENDFUNCTION'");
    advance();

    function->block = block;

    return node;
}

Node *Parser::parseFunctionCall() {
    const Token &functionToken = *currentToken;
    std::vector<Node*> args;
    advance();

    if (currentToken->type != TokenType::LPAREN) std::abort();
    advance();

    if (currentToken->type == TokenType::RPAREN) {
        advance();
    } else {
        args.push_back(parseEvaluationExpression());

        while (currentToken->type == TokenType::COMMA) {
            advance();
            args.push_back(parseEvaluationExpression());
        }

        if (currentToken->type != TokenType::RPAREN)
            throw PSC::ExpectedTokenError(*currentToken, "')'");
        advance();
    }

    return create<FunctionCallNode>(functionToken, std::move(args));
}
