#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include "psc/types/types.h"

namespace PSC {
    enum class FileMode {
        READ, WRITE, APPEND
    };

    class File {
    private:
        FileMode mode;
        std::unique_ptr<std::ofstream> ofile;
        std::unique_ptr<std::ifstream> ifile;
        bool open = true;

    public:
        const String name;
        
        File(const String &name, FileMode mode);

        ~File();

        FileMode getMode();

        bool eof();

        void close();

        String read();

        void write(const String &data);
    };

    class FileManager {
    private:
        static std::vector<std::unique_ptr<File>> files;
    
    public:
        static bool createFile(const String &name, FileMode mode);

        static File *getFile(const String &name);

        static void closeFile(const String &name);
    };
}
