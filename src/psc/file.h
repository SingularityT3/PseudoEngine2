#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include "psc/array.h"
#include "psc/types/types.h"

namespace PSC {
    enum class FileMode {
        READ, WRITE, APPEND, RANDOM
    };

    class File {
    private:
        FileMode mode;
        std::unique_ptr<std::ofstream> ofile;
        std::unique_ptr<std::ifstream> ifile;
        std::vector<std::string> records;
        size_t ptr = 0;
        bool open = true;
        bool modified = false;

    public:
        const String name;
        
        File(const String &name, FileMode mode);

        ~File();

        FileMode getMode();

        bool eof();

        void close();

        String read();

        void write(const String &data);

        bool seek(const Integer &address);

        bool getRecord(DataHolder &data, Context &ctx);

        void putRecord(DataHolder &data);
    };

    class FileManager {
    private:
        std::vector<std::unique_ptr<File>> files;
    
    public:
        bool createFile(const String &name, FileMode mode);

        File *getFile(const String &name);

        void closeFile(const String &name);
    };
}
