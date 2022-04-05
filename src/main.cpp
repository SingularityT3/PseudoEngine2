#include <string>
#include "run.h"

std::string psfilename = "<stdin>";

int main(int argc, char **argv) {
    auto fn = startREPL;

    if (argc > 1) {
        psfilename = argv[1];
        fn = runFile;
    }

    return fn() ? EXIT_SUCCESS : EXIT_FAILURE;
}
