#include "pch.h"
#include <string>
#include <ctime>
#include <stdlib.h>

#include "launch/run.h"

#include "line_util.h"
#ifdef READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

std::string psfilename = "<stdin>";
bool REPLMode = true;

int main(int argc, char **argv) {
    auto fn = startREPL;

    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            std::cout << "Usage: " << argv[0] << " [options] [filename]\n\nOptions:\n -h\t--help\n -v\t--version\n" << std::endl;
            return EXIT_SUCCESS;
        } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
            std::cout << "PseudoEngine2 v0.5.1" << std::endl;
            return EXIT_SUCCESS;
        } else if (argv[1][0] == '-') {
            std::cerr << "Unknown option '" << argv[1] << "'\nUse `" << argv[0] << " --help` for more info" << std::endl;
            return EXIT_FAILURE;
        } else {
            psfilename = argv[1];
            REPLMode = false;
            fn = runFile;
        }
    }

#ifdef READLINE
    rl_bind_key('\t', rl_tab_insert);
#endif

    srand((unsigned int) time(NULL));

    return fn() ? EXIT_SUCCESS : EXIT_FAILURE;
}

bool getLine(std::string &line, const char *prompt) {
#ifdef READLINE
    char *buf = readline(prompt);
    if (buf != NULL) {
        line = buf;
        if (buf[0] != '\0') add_history(buf);
        free(buf);
    } else {
        line.clear();
        return false;
    }
#else
    std::cout << prompt << std::flush;
    std::getline(std::cin, line);
    if (std::cin.eof()) return false;
#endif

    return true;
}
