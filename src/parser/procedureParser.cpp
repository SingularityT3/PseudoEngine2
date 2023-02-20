#include "pch.h"

#include "parser/parser.h"

Node *Parser::parseCall() {
    const Token &callToken = *currentToken;
    advance();

    if (currentToken->type != TokenType::IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "identifier");

    const std::string &identifier = currentToken->value;
    advance();

    std::vector<Node*> args;
    if (currentToken->type == TokenType::LPAREN) {
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
    }

    return create<CallNode>(callToken, identifier, std::move(args));
}

Node *Parser::parseProcedure() {
    const Token &procedureToken = *currentToken;
    advance();

    if (currentToken->type != TokenType::IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "identifier");
    const std::string &procedureName = currentToken->value;
    advance();

    bool byRef = false;
    std::vector<std::string> parameterNames;
    std::vector<const Token*> parameterTypes;

    if (currentToken->type == TokenType::LPAREN) {
        advance();

        if (currentToken->type == TokenType::BYREF) {
            byRef = true;
            advance();
        } else if (currentToken->type == TokenType::BYVAL) {
            advance();
        }

        while (currentToken->type != TokenType::RPAREN) {
            if (parameterNames.size() > 0) {
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

            if (currentToken->type != TokenType::DATA_TYPE && currentToken->type != TokenType::IDENTIFIER)
                throw PSC::ExpectedTokenError(*currentToken, "data type");
            const Token *type = currentToken;
            advance();

            parameterNames.emplace_back(paramName);
            parameterTypes.emplace_back(type);
        }
        advance();
    }

    PSC::Block *block = parseBlock();
    if (currentToken->type != TokenType::ENDPROCEDURE)
        throw PSC::ExpectedTokenError(*currentToken, "'ENDPROCEDURE'");
    advance();

    return create<ProcedureNode>(
        procedureToken,
        procedureName,
        std::move(parameterNames),
        std::move(parameterTypes),
        byRef,
        *block
    );
}
