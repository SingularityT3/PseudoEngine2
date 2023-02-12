#include "pch.h"

#include "nodes/file.h"
#include "psc/error.h"
#include "psc/file.h"

OpenFileNode::OpenFileNode(const Token &token, Node &filename, PSC::FileMode mode)
    : Node(token), mode(mode), filename(filename) {}

std::unique_ptr<NodeResult> OpenFileNode::evaluate(PSC::Context &ctx) {
    auto filenameRes = filename.evaluate(ctx);
    if (filenameRes->type != PSC::DataType::STRING)
        throw PSC::RuntimeError(token, ctx, "Expected string for file name");

    const PSC::String &filename = filenameRes->get<PSC::String>();

    PSC::File *file = PSC::FileManager::getFile(filename);
    if (file != nullptr)
        throw PSC::RuntimeError(token, ctx, "File '" + filename.value + "' is already open");

    bool success = PSC::FileManager::createFile(filename, mode);
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
        var = new PSC::Variable(identifier.value, PSC::DataType::STRING, false);
        ctx.addVariable(var);
    }
    if (var->type != PSC::DataType::STRING)
        throw PSC::RuntimeError(token, ctx, "Variable of type STRING expected");

    auto &filename = filenameRes->get<PSC::String>();
    PSC::File *file = PSC::FileManager::getFile(filename);
    if (file == nullptr)
        throw PSC::FileNotOpenError(token, ctx, filename.value);
    
    PSC::String *data = new PSC::String;
    *data = file->read();
    var->set(data);

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}


WriteFileNode::WriteFileNode(const Token &token, Node &filename, Node &node)
    : UnaryNode(token, node), filename(filename) {}

std::unique_ptr<NodeResult> WriteFileNode::evaluate(PSC::Context &ctx) {
    auto filenameRes = filename.evaluate(ctx);
    if (filenameRes->type != PSC::DataType::STRING)
        throw PSC::RuntimeError(token, ctx, "Expected string for file name");
    
    auto &filename = filenameRes->get<PSC::String>();
    PSC::File *file = PSC::FileManager::getFile(filename);
    if (file == nullptr)
        throw PSC::FileNotOpenError(token, ctx, filename.value);
    if (file->getMode() == PSC::FileMode::READ)
        throw PSC::RuntimeError(token, ctx, "File '" + filename.value + "' is opened as read-only");

    auto nodeRes = node.evaluate(ctx);
    PSC::String *data;
    switch (nodeRes->type) {
        case PSC::DataType::INTEGER:
            data = nodeRes->get<PSC::Integer>().toString().release();
            break;
        case PSC::DataType::REAL:
            data = nodeRes->get<PSC::Real>().toString().release();
            break;
        case PSC::DataType::BOOLEAN:
            data = nodeRes->get<PSC::Boolean>().toString().release();
            break;
        case PSC::DataType::CHAR:
            data = nodeRes->get<PSC::Char>().toString().release();
            break;
        case PSC::DataType::STRING:
            data = (PSC::String*) nodeRes->data.release();
            break;
        case PSC::DataType::NONE:
            throw PSC::RuntimeError(token, ctx, "Expected value for writing");
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
    PSC::File *file = PSC::FileManager::getFile(filename);
    if (file == nullptr)
        throw PSC::FileNotOpenError(token, ctx, filename.value);

    PSC::FileManager::closeFile(filename);

    return std::make_unique<NodeResult>(nullptr, PSC::DataType::NONE);
}
