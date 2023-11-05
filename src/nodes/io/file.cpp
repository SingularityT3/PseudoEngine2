#include "pch.h"

#include "nodes/io/file.h"
#include "psc/error.h"
#include "psc/file.h"

OpenFileNode::OpenFileNode(const Token &token, Node &filename, PSC::FileMode mode)
    : Node(token), mode(mode), filename(filename) {}

std::unique_ptr<NodeResult> OpenFileNode::evaluate(PSC::Context &ctx) {
    auto filenameRes = filename.evaluate(ctx);
    if (filenameRes->type != PSC::DataType::STRING)
        throw PSC::RuntimeError(token, ctx, "Expected string for file name");

    const PSC::String &filename = filenameRes->get<PSC::String>();

    PSC::File *file = ctx.getFileManager().getFile(filename);
    if (file != nullptr)
        throw PSC::RuntimeError(token, ctx, "File '" + filename.value + "' is already open");

    bool success = ctx.getFileManager().createFile(filename, mode);
    if (!success)
        throw PSC::RuntimeError(token, ctx, "Failed to open file '" + filename.value + "'");

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


ReadFileNode::ReadFileNode(const Token &token, Node &filename, const Token &identifier)
    : Node(token), filename(filename), identifier(identifier) {}

std::unique_ptr<NodeResult> ReadFileNode::evaluate(PSC::Context &ctx) {
    auto filenameRes = filename.evaluate(ctx);
    if (filenameRes->type != PSC::DataType::STRING)
        throw PSC::RuntimeError(token, ctx, "Expected string for file name");
    
    PSC::Variable *var = ctx.getVariable(identifier.value);
    if (var == nullptr) {
        var = new PSC::Variable(identifier.value, PSC::DataType::STRING, false, &ctx);
        ctx.addVariable(var);
    }
    if (var->type != PSC::DataType::STRING)
        throw PSC::RuntimeError(token, ctx, "Variable of type STRING expected");

    auto &filename = filenameRes->get<PSC::String>();
    PSC::File *file = ctx.getFileManager().getFile(filename);
    if (file == nullptr)
        throw PSC::FileNotOpenError(token, ctx, filename.value);
    
    PSC::String *data = new PSC::String;
    *data = file->read();
    var->set(data);

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


WriteFileNode::WriteFileNode(const Token &token, Node &filename, Node &data)
    : Node(token), filename(filename), data(data) {}

std::unique_ptr<NodeResult> WriteFileNode::evaluate(PSC::Context &ctx) {
    auto filenameRes = filename.evaluate(ctx);
    if (filenameRes->type != PSC::DataType::STRING)
        throw PSC::RuntimeError(token, ctx, "Expected string for file name");
    
    auto &filename = filenameRes->get<PSC::String>();
    PSC::File *file = ctx.getFileManager().getFile(filename);
    if (file == nullptr)
        throw PSC::FileNotOpenError(token, ctx, filename.value);
    if (file->getMode() == PSC::FileMode::READ)
        throw PSC::RuntimeError(token, ctx, "File '" + filename.value + "' is opened as read-only");

    auto nodeRes = data.evaluate(ctx);
    std::unique_ptr<PSC::String> data;
    switch (nodeRes->type.type) {
        case PSC::DataType::INTEGER:
            data = nodeRes->get<PSC::Integer>().toString();
            break;
        case PSC::DataType::REAL:
            data = nodeRes->get<PSC::Real>().toString();
            break;
        case PSC::DataType::BOOLEAN:
            data = nodeRes->get<PSC::Boolean>().toString();
            break;
        case PSC::DataType::CHAR:
            data = nodeRes->get<PSC::Char>().toString();
            break;
        case PSC::DataType::STRING: {
            auto str = nodeRes->get<PSC::String>();
            data = std::make_unique<PSC::String>(str);
            break;
        } case PSC::DataType::DATE:
            data = nodeRes->get<PSC::Date>().toString();
            break;
        case PSC::DataType::NONE:
            throw PSC::RuntimeError(token, ctx, "Expected value for writing");
        case PSC::DataType::ENUM:
        case PSC::DataType::POINTER:
        case PSC::DataType::COMPOSITE:
            throw PSC::TypeOperationError(token, ctx, "Write");
    }

    file->write(*data);

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


CloseFileNode::CloseFileNode(const Token &token, Node &filename)
    : Node(token), filename(filename) {}

std::unique_ptr<NodeResult> CloseFileNode::evaluate(PSC::Context &ctx) {
    auto filenameRes = filename.evaluate(ctx);
    if (filenameRes->type != PSC::DataType::STRING)
        throw PSC::RuntimeError(token, ctx, "Expected string for file name");
    
    auto &filename = filenameRes->get<PSC::String>();
    PSC::File *file = ctx.getFileManager().getFile(filename);
    if (file == nullptr)
        throw PSC::FileNotOpenError(token, ctx, filename.value);

    ctx.getFileManager().closeFile(filename);

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


SeekFileNode::SeekFileNode(const Token &token, Node &filename, Node &address)
    : Node(token), filename(filename), address(address) {}

std::unique_ptr<NodeResult> SeekFileNode::evaluate(PSC::Context &ctx) {
    auto addressRes = address.evaluate(ctx);
    if (addressRes->type != PSC::DataType::INTEGER)
        throw PSC::RuntimeError(token, ctx, "Seek address must be of integer data type");
    const PSC::Integer &address = addressRes->get<PSC::Integer>();

    if (address.value < 1)
        throw PSC::RuntimeError(token, ctx, "Seek address must be greater than 0");

    auto filenameRes = filename.evaluate(ctx);
    if (filenameRes->type != PSC::DataType::STRING)
        throw PSC::RuntimeError(token, ctx, "Filename must be of string data type");
    const PSC::String &filenameStr = filenameRes->get<PSC::String>().value;

    PSC::File *file = ctx.getFileManager().getFile(filenameStr);
    if (file == nullptr)
        throw PSC::FileNotOpenError(token, ctx, filenameStr.value);
    if (file->getMode() != PSC::FileMode::RANDOM)
        throw PSC::RuntimeError(token, ctx, "'SEEK' can only be used for random files");
    
    if (!file->seek(address))
        throw PSC::RuntimeError(token, ctx, "Seek address exceeds file size");

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


GetRecordNode::GetRecordNode(const Token &token, Node &filename, const Token &identifier)
    : Node(token), filename(filename), identifier(identifier) {}

std::unique_ptr<NodeResult> GetRecordNode::evaluate(PSC::Context &ctx) {
    auto filenameRes = filename.evaluate(ctx);
    if (filenameRes->type != PSC::DataType::STRING)
        throw PSC::RuntimeError(token, ctx, "Filename must be of string data type");
    const PSC::String &filenameStr = filenameRes->get<PSC::String>().value;

    PSC::File *file = ctx.getFileManager().getFile(filenameStr);
    if (file == nullptr)
        throw PSC::FileNotOpenError(token, ctx, filenameStr.value);
    if (file->getMode() != PSC::FileMode::RANDOM)
        throw PSC::RuntimeError(token, ctx, "'GETRECORD' can only be used for random files");

    PSC::Variable *variable = ctx.getVariable(identifier.value);
    PSC::Array *array = ctx.getArray(identifier.value);
    if (variable == nullptr && array == nullptr)
        throw PSC::NotDefinedError(identifier, ctx, identifier.value);
    if ((variable != nullptr && variable->type == PSC::DataType::POINTER)
        || (array != nullptr && array->type == PSC::DataType::POINTER))
        throw PSC::RuntimeError(token, ctx, "Pointers cannot be stored in random files");

    if (variable != nullptr) {
        if (!file->getRecord(*variable, ctx))
            throw PSC::RuntimeError(token, ctx, "Failed to read data from random file");
    } else {
        if (!file->getRecord(*array, ctx))
            throw PSC::RuntimeError(token, ctx, "Failed to read data from random file");
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


PutRecordNode::PutRecordNode(const Token &token, Node &filename, const Token &identifier)
    : Node(token), filename(filename), identifier(identifier) {}

std::unique_ptr<NodeResult> PutRecordNode::evaluate(PSC::Context &ctx) {
    auto filenameRes = filename.evaluate(ctx);
    if (filenameRes->type != PSC::DataType::STRING)
        throw PSC::RuntimeError(token, ctx, "Filename must be of string data type");
    const PSC::String &filenameStr = filenameRes->get<PSC::String>().value;

    PSC::File *file = ctx.getFileManager().getFile(filenameStr);
    if (file == nullptr)
        throw PSC::FileNotOpenError(token, ctx, filenameStr.value);
    if (file->getMode() != PSC::FileMode::RANDOM)
        throw PSC::RuntimeError(token, ctx, "'PUTRECORD' can only be used for random files");

    PSC::Variable *variable = ctx.getVariable(identifier.value);
    PSC::Array *array = ctx.getArray(identifier.value);
    if (variable == nullptr && array == nullptr)
        throw PSC::NotDefinedError(identifier, ctx, identifier.value);
    if ((variable != nullptr && variable->type == PSC::DataType::POINTER)
        || (array != nullptr && array->type == PSC::DataType::POINTER))
        throw PSC::RuntimeError(token, ctx, "Pointers cannot be stored in random files");

    if (variable != nullptr) {
        file->putRecord(*variable);
    } else {
        file->putRecord(*array);
    }

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
