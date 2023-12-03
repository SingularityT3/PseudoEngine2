#include "pch.h"
#include <sstream>

#include "psc/error.h"

extern std::string psfilename;

using namespace PSC;

Error::Error(const Token &token, const std::string &type, const std::string &info)
    : token(token), type(type), info(info)
{}

Error::Error(const Token &token, const std::string &info)
    : Error(token, "Error", info)
{}

std::string Error::toStr() const {
    std::ostringstream os;
    os << type << " on line " << token.line << ", column " << token.column << " of " << psfilename;
    if (info.size() > 0) {
        os << ":\n" << info;
    }
    return os.str();
}

void Error::print(std::ostream &os) const {
    os << toStr() << std::endl;
}


PedanticError::PedanticError(const Token &token, const std::string &info)
    : Error(token, "Error(pedantic)", info)
{}

SyntaxError::SyntaxError(const Token &token, const std::string &info)
    : Error(token, "Syntax Error", info)
{}

LexerError::LexerError(int line, int column, const std::string &info)
    : SyntaxError(t, info), t(TokenType::CHAR, line, column)
{}

inline std::string invalidCharMsg(char c) {
    std::string s = "Invalid characher '";
    s += c;
    s += '\'';
    return s;
}

InvalidCharError::InvalidCharError(int line, int column, char c)
    : LexerError(line, column, invalidCharMsg(c))
{}

ExpectedQuotesError::ExpectedQuotesError(int line, int column, bool doubleQuotes)
    : LexerError(line, column, (std::string("Expected '") + (doubleQuotes ? "\"" : "'")) + "'")
{}

ExpectedTokenError::ExpectedTokenError(const Token &token, const std::string &expToken)
    : SyntaxError(token, "Expected " + expToken)
{}


RuntimeError::RuntimeError(const Token &token, const Context &context, const std::string &info)
    : Error(token, "Runtime Error", info), context(context)
{
    std::ostringstream os;
    os << type << " in file " << psfilename;
    if (info.size() > 0) {
        os << ":\n" << info;
    }

    os << "\n\nTraceback:\n" << context.getName() << ", line " << token.line << ", column " << token.column;

    const Context *ctx = context.getParent();
    while (ctx != nullptr) {
        os << "\n" << ctx->getName() << ", line " << ctx->switchToken->line << ", column " << ctx->switchToken->column;
        Context *parent = ctx->getParent();
        ctx = parent;
    }

    rtInfo = os.str();
}

std::string RuntimeError::toStr() const {
    return rtInfo;
}


InvalidUsageError::InvalidUsageError(const Token &token, const Context &context, const std::string &info)
    : RuntimeError(token, context, "Invalid usage of " + info)
{}

TypeOperationError::TypeOperationError(const Token &token, const Context &context, const std::string &op)
    : RuntimeError(token, context, op + " operation cannot be used on non-primitive types")
{}

RedeclarationError::RedeclarationError(const Token &token, const Context &context, const std::string &identifier)
    : RuntimeError(token, context, "Redeclaration of '" + identifier + "'")
{}

RedefinitionError::RedefinitionError(const Token &token, const Context &context, const std::string &identifier)
    : RuntimeError(token, context, "Redefinition of '" + identifier + "'")
{}

ConstAssignError::ConstAssignError(const Token &token, const Context &context, const std::string &constant)
    : RuntimeError(token, context, "Assignment to constant '" + constant + "'")
{}

NotDefinedError::NotDefinedError(const Token &token, const Context &context, const std::string &identifier)
    : RuntimeError(token, context, identifier + " is not defined")
{}

ArrayDirectAccessError::ArrayDirectAccessError(const Token &token, const Context &context)
    : RuntimeError(token, context, "Attempting to access array without indices")
{}

inline std::string conditionTypeErrorMessage(TokenType type) {
    std::stringstream msg;
    msg << "Invalid condition for ";
    switch (type) {
        case TokenType::IF:
            msg << "IF statement";
            break;
        case TokenType::WHILE:
            msg << "WHILE loop";
            break;
        case TokenType::REPEAT:
            msg << "REPEAT loop";
            break;
        default:
            msg << type;
    }
    msg << ": Expected result of type BOOLEAN";
    return msg.str();
}

ConditionTypeError::ConditionTypeError(const Token &token, const Context &context)
    : RuntimeError(token, context, conditionTypeErrorMessage(token.type))
{}

inline std::string invalidArgsErrorMessage(std::vector<PSC::DataType> expected_types, std::vector<PSC::DataType> actual_types) {
    std::stringstream msg;
    msg << "Invalid args: expected (";
    
    size_t et_size = expected_types.size();
    size_t at_size = actual_types.size();

    for (size_t i = 0; i < et_size; i++) {
        if (i != 0) msg << ", ";
        msg << expected_types[i];
    }
    if (et_size == 0) msg << "None";

    msg << "), got (";
    
    for (size_t i = 0; i < at_size; i++) {
        if (i != 0) msg << ", ";
        msg << actual_types[i];
    }
    if (at_size == 0) msg << "None";
    msg << ")";

    return msg.str();
}

InvalidArgsError::InvalidArgsError(const Token &token, const Context &context, std::vector<PSC::DataType> &&expected_types, std::vector<PSC::DataType> &&actual_types)
    : RuntimeError(token, context, invalidArgsErrorMessage(std::move(expected_types), std::move(actual_types)))
{}

FileNotOpenError::FileNotOpenError(const Token &token, const Context &context, const std::string &filename)
    : RuntimeError(token, context, "File '" + filename + "' is not open") {}
