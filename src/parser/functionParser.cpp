#include "parser/parser.h"

Node *Parser::parseFunction() {
    const Token &functionToken = *currentToken;
    advance();

    if (currentToken->type != TT_IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "identifier");
    const Token &identifier = *currentToken;
    advance();

    PSC::Function *function = new PSC::Function(identifier.value);
    function->defToken = &functionToken;
    function->byRef = false;

    FunctionNode *node = new FunctionNode(functionToken, function);
    nodes.push_back(node);

    if (currentToken->type == TT_LPAREN) {
        advance();

        if (currentToken->type == TT_BYREF) {
            function->byRef = true;
            advance();
        } else if (currentToken->type == TT_BYVAL) {
            advance();
        }

        while (currentToken->type != TT_RPAREN) {
            if (function->parameters.size() > 0) {
                if (currentToken->type != TT_COMMA)
                    throw PSC::ExpectedTokenError(*currentToken, "','");
                advance();
            }

            if (currentToken->type != TT_IDENTIFIER)
                throw PSC::ExpectedTokenError(*currentToken, "identifier or ')'");
            const std::string &paramName = currentToken->value;
            advance();

            if (currentToken->type != TT_COLON)
                throw PSC::ExpectedTokenError(*currentToken, "':'");
            advance();

            if (currentToken->type != TT_DATA_TYPE)
                throw PSC::ExpectedTokenError(*currentToken, "data type");

            PSC::DataType type = getPSCType();
            advance();

            function->parameters.emplace_back(paramName, type);
        }
        advance();
    }

    if (currentToken->type == TT_LINE_END) advance();
    if (currentToken->type != TT_RETURNS)
        throw PSC::ExpectedTokenError(*currentToken, "'RETURNS'");
    advance();

    if (currentToken->type != TT_DATA_TYPE)
        throw PSC::ExpectedTokenError(*currentToken, "data type");

    function->returnType = getPSCType();
    advance();

    PSC::Block *block = parseBlock();
    if (currentToken->type != TT_ENDFUNCTION)
        throw PSC::ExpectedTokenError(*currentToken, "'ENDFUNCTION'");
    advance();

    function->block = block;

    return node;
}

Node *Parser::parseFunctionCall() {
    FunctionCallNode *node = new FunctionCallNode(*currentToken);
    nodes.push_back(node);
    advance();

    if (currentToken->type != TT_LPAREN) std::abort();
    advance();

    if (currentToken->type == TT_RPAREN) {
        advance();
    } else {
        Node *evalExpr = parseEvaluationExpression();
        node->args.push_back(evalExpr);

        while (currentToken->type == TT_COMMA) {
            advance();

            evalExpr = parseEvaluationExpression();
            node->args.push_back(evalExpr);
        }

        if (currentToken->type != TT_RPAREN)
            throw PSC::ExpectedTokenError(*currentToken, "')'");
        advance();
    }

    return node;
}
