#include "pch.h"

#include "psc/file.h"
#include <filesystem>
#include <sstream>

using namespace PSC;

File::File(const String &name, FileMode mode) : mode(mode), name(name) {
    switch (mode) {
        case FileMode::READ:
            ifile = std::make_unique<std::ifstream>();
            ifile->open(name.value);
            break;
        case FileMode::WRITE:
            ofile = std::make_unique<std::ofstream>();
            ofile->open(name.value);
            break;
        case FileMode::APPEND:
            ofile = std::make_unique<std::ofstream>();
            ofile->open(name.value, std::ios::app);
            break;
        case FileMode::RANDOM:
            ifile = std::make_unique<std::ifstream>();
            ifile->open(name.value);
            std::string line;
            if (ifile->peek() != std::ifstream::traits_type::eof()) {
                while (!ifile->eof()) {
                    std::getline(*ifile, line);
                    if (line.starts_with('#') && records.size() > 1) {
                        records[records.size()-1] += '\n';
                        records[records.size()-1] += line;
                        continue;
                    }
                    records.push_back(std::move(line));
                }
            }
            ifile->close();
            while (records.size() > 0 && records[records.size() - 1].length() == 0) {
                records.pop_back();
            }
            break;
    }
}

File::~File() {
    if (open) close();
}

FileMode File::getMode() {
    return mode;
}

bool File::eof() {
    if (mode != FileMode::READ) std::abort();
    return ifile->eof();
}

void File::close() {
    switch (mode) {
        case FileMode::READ:
            ifile->close();
            break;
        case FileMode::WRITE:
        case FileMode::APPEND:
            ofile->close();
            break;
        case FileMode::RANDOM:
            if (modified) {
                ofile = std::make_unique<std::ofstream>();
                ofile->open(name.value);
                for (const auto &line : records) {
                    *ofile << line << "\n";
                }
                ofile->close();
            }
            break;
    }
    open = false;
}

String File::read() {
    String line;
    std::getline(*ifile, line.value);
    return line;
}

void File::write(const String &data) {
    *ofile << data.value << "\n";
}

bool File::seek(const Integer &address) {
    // Allow seeking to one index past size for appending
    if ((size_t) address.value > records.size() + 1) return false;
    ptr = address.value - 1;
    return true;
}

bool File::getRecord(DataHolder &data, Context &ctx) {
    if (ptr == records.size()) return false;
    const std::string &line = records[ptr];
    std::istringstream ss(line);
    return data.load(ss, ctx);
}

void File::putRecord(DataHolder &data) {
    std::ostringstream ss;
    data.dump(ss);
    if (ptr == records.size()) {
        records.push_back(ss.str());
    } else {
        records[ptr] = ss.str();
    }
    modified = true;
}

bool FileManager::createFile(const String &name, FileMode mode) {
    namespace fs = std::filesystem;
    if (!fs::exists(fs::path(name.value))) {
        if (mode == FileMode::RANDOM) {
            std::ofstream f(name.value, std::ios::out);
            f.close();
            if (f.fail()) return false;
        } else if (mode != FileMode::WRITE) {
            return false;
        }
    }
    files.emplace_back(std::make_unique<File>(name, mode));
    return true;
}

File *FileManager::getFile(const String &name) {
    for (auto &file : files) {
        if (file->name.value == name.value) return file.get();
    }
    return nullptr;
}

void FileManager::closeFile(const String &name) {
    for (std::size_t i = 0; i < files.size(); i++) {
        auto &file = files[i];
        if (file->name.value == name.value) {
            file->close();
            files.erase(files.begin() + i);
            return;
        }
    }
}
