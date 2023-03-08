#include "pch.h"

#include "psc/file.h"
#include <filesystem>

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
            ofile->open(name.value, std::ios_base::app);
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
    if (mode == FileMode::READ) {
        ifile->close();
    } else {
        ofile->close();
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


bool FileManager::createFile(const String &name, FileMode mode) {
    namespace fs = std::filesystem;
    if (!fs::exists(fs::path(name.value)) && mode != FileMode::WRITE) return false;
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
