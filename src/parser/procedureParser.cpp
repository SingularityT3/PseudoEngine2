#include "parser/parser.h"

Node *Parser::parseCall() {
    const Token &callToken = *currentToken;
    advance();

    if (currentToken->type != TT_IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "identifier");

    CallNode *node = create<CallNode>(callToken, currentToken->value);
    advance();

    if (currentToken->type == TT_LPAREN) {
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
    }

    return node;
}

Node *Parser::parseProcedure() {
    const Token &procedureToken = *currentToken;
    advance();

    if (currentToken->type != TT_IDENTIFIER)
        throw PSC::ExpectedTokenError(*currentToken, "identifier");
    const Token &identifier = *currentToken;
    advance();

    PSC::Procedure *procedure = new PSC::Procedure(identifier.value);
    procedure->byRef = false;

    ProcedureNode *node = create<ProcedureNode>(procedureToken, procedure);

    if (currentToken->type == TT_LPAREN) {
        advance();

        if (currentToken->type == TT_BYREF) {
            procedure->byRef = true;
            advance();
        } else if (currentToken->type == TT_BYVAL) {
            advance();
        }

        while (currentToken->type != TT_RPAREN) {
            if (procedure->parameters.size() > 0) {
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

            procedure->parameters.emplace_back(paramName, type);
        }
        advance();
    }

    PSC::Block *block = parseBlock();
    if (currentToken->type != TT_ENDPROCEDURE)
        throw PSC::ExpectedTokenError(*currentToken, "'ENDPROCEDURE'");
    advance();

    procedure->block = block;

    return node;
}
