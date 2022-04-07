#include <string>
#include <ctime>
#include <stdlib.h>
#include "run.h"

std::string psfilename = "<stdin>";
bool REPLMode = true;

int main(int argc, char **argv) {
    auto fn = startREPL;

    if (argc > 1) {
        psfilename = argv[1];
        REPLMode = false;
        fn = runFile;
    }

    srand((unsigned int) time(NULL));

    return fn() ? EXIT_SUCCESS : EXIT_FAILURE;
}
