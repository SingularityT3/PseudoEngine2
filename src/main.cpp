#include "pch.h"
#include <string>
#include <ctime>
#include <stdlib.h>

#include "run.h"

std::string psfilename = "<stdin>";
bool REPLMode = true;

int main(int argc, char **argv) {
    auto fn = startREPL;

    if (argc == 2) {
        psfilename = argv[1];
        REPLMode = false;
        fn = runFile;
    } else if (argc > 2) {
        std::cerr << "Too many arguements!\nUsage:\n" << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    }

    srand((unsigned int) time(NULL));

    return fn() ? EXIT_SUCCESS : EXIT_FAILURE;
}
