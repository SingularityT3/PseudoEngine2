#pragma once
#include <iostream>
#include <string>
#include "psc/scope/context.h"
#include "nodes/base.h"
#include "tokens.h"

namespace PSC {
    class Error {
    protected:
        const Token &token;
        const std::string type, info;

        Error(const Token &token, const std::string &type, const std::string &info);

    public:
        Error(const Token &token, const std::string &info = "");

        virtual std::string toStr() const;

        void print(std::ostream &os = std::cerr) const;
    };


    class SyntaxError : public Error {
    public:
        SyntaxError(const Token &token, const std::string &info = "");
    };


    class LexerError : public SyntaxError {
    private:
        Token t;
    public:
        LexerError(int line, int column, const std::string &info);
    };

    class InvalidCharError : public LexerError {
    public:
        InvalidCharError(int line, int column, char c);
    };

    class ExpectedQuotesError : public LexerError {
    public:
        ExpectedQuotesError(int line, int column, bool doubleQuotes);
    };

    class ExpectedTokenError : public SyntaxError {
    public:
        ExpectedTokenError(const Token &token, const std::string &expToken);
    };


    class RuntimeError : public Error {
    private:
        std::string rtInfo;

    public:
        const Context &context;

        RuntimeError(const Token &token, const Context &context, const std::string &info = "");

        std::string toStr() const override;
    };

    class InvalidUsageError : public RuntimeError {
    public:
        InvalidUsageError(const Token &token, const Context &context, const std::string &info);
    };

    class RedeclarationError : public RuntimeError {
    public:
        RedeclarationError(const Token &token, const Context &context, const std::string &identifier);
    };

    class ConstAssignError : public RuntimeError {
    public:
        ConstAssignError(const Token &token, const Context &context, const std::string &constant);
    };

    class NotDefinedError : public RuntimeError {
    public:
        NotDefinedError(const Token &token, const Context &context, const std::string &identifier);
    };

    class ConditionTypeError : public RuntimeError {
    public:
        ConditionTypeError(const Token &token, const Context &context);
    };

    class InvalidArgsError : public RuntimeError {
    public:
        InvalidArgsError(const Token &token, const Context &context);
    };
}
