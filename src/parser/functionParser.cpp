#include "pch.h"

#include "parser/parser.h"

Node *Parser::parseFunction() {
    const Token &functionToken = *currentToken;
    advance();

    if (currentToken->type != TokenType::IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "identifier");
    const std::string &functionName = currentToken->value;
    advance();

    bool byRef = false;
    std::vector<std::string> parameterNames;
    std::vector<const Token*> parameterTypes;
    std::vector<bool> parameterPassTypes;
    const Token *returnType;

    if (currentToken->type == TokenType::LPAREN) {
        advance();

        int typeCount = 1;
        int passTypeCount = 0;
        while (currentToken->type != TokenType::RPAREN) {
            if (parameterNames.size() > 0) {
                if (currentToken->type != TokenType::COMMA)
                    throw PSC::ExpectedTokenError(*currentToken, "','");
                advance();
            }

            if (currentToken->type == TokenType::BYREF
                || currentToken->type == TokenType::BYVAL
            ) {
                TokenType currentType = byRef ? TokenType::BYREF : TokenType::BYVAL;
                if (currentType != currentToken->type) {
                    parameterPassTypes.reserve(passTypeCount);
                    for (int i = 0; i < passTypeCount; i++)
                        parameterPassTypes.push_back(byRef);
                    byRef = !byRef;
                    passTypeCount = 1;
                } else {
                    passTypeCount++;
                }
                advance();
            } else {
                passTypeCount++;
            }

            if (currentToken->type != TokenType::IDENTIFIER)
                throw PSC::ExpectedTokenError(*currentToken, "identifier or ')'");
            const std::string &paramName = currentToken->value;
            advance();

            if (currentToken->type == TokenType::COLON) advance();
            else if (currentToken->type == TokenType::COMMA) {
                parameterNames.emplace_back(paramName);
                typeCount++;
                continue;
            }
            else throw PSC::ExpectedTokenError(*currentToken, "':'");

            if (currentToken->type != TokenType::DATA_TYPE && currentToken->type != TokenType::IDENTIFIER)
                throw PSC::ExpectedTokenError(*currentToken, "data type");
            const Token *type = currentToken;
            advance();

            parameterNames.emplace_back(paramName);
            parameterTypes.reserve(typeCount);
            for (int i = 0; i < typeCount; i++) {
                parameterTypes.emplace_back(type);
            }
            typeCount = 1;
        }
        if (typeCount != 1) throw PSC::ExpectedTokenError(*currentToken, "data type"); 
        if (passTypeCount > 0) {
            parameterPassTypes.reserve(passTypeCount);
            for (int i = 0; i < passTypeCount; i++)
                parameterPassTypes.push_back(byRef);
        }
        advance(); // ')'
    }

    while (currentToken->type == TokenType::LINE_END) advance();
    if (currentToken->type != TokenType::RETURNS)
        throw PSC::ExpectedTokenError(*currentToken, "'RETURNS'");
    advance();

    if (currentToken->type != TokenType::DATA_TYPE && currentToken->type != TokenType::IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "data type");
    returnType = currentToken;
    advance();

    PSC::Block *block = parseBlock();
    if (currentToken->type != TokenType::ENDFUNCTION)
        throw PSC::ExpectedTokenError(*currentToken, "'ENDFUNCTION'");
    advance();

    return create<FunctionNode>(
        functionToken,
        functionName,
        std::move(parameterNames),
        std::move(parameterTypes),
        std::move(parameterPassTypes),
        *block,
        *returnType
    );
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
