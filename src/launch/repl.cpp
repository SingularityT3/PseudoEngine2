#include "pch.h"

#include <string>
#include "launch/run.h"

#ifdef READLINE
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#endif

extern std::string psfilename;
extern bool REPLMode;

static const std::string multilineKeywords[] = {
    "IF",
    "CASE",
    "WHILE",
    "REPEAT",
    "FOR",
    "PROCEDURE",
    "FUNCTION",
    "TYPE"
};

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

bool startREPL() {
    std::cout << "PseudoEngine2 v0.5.1 REPL\nEnter 'EXIT' to quit\n";

#ifdef READLINE
    rl_bind_key('\t', rl_tab_insert);
#endif

    Lexer lexer;
    Parser parser;
    auto globalCtx = PSC::Context::createGlobalContext();

    while (true) {
        std::string code;
        if (!getLine(code, "> ")) break;

        size_t size = code.size();
        if (size == 0) continue;
        if (code == "EXIT") break;
        if (code.starts_with("RUNFILE")) {
            if (size < 9) {
                std::cerr << "Expected filename" << std::endl;
                continue;
            }
            psfilename = code.substr(8, size - 8);
            for (size_t i = psfilename.size() - 1; i > 0; i--) {
                if (psfilename[i] != ' ' && psfilename[i] != '\t') break;
                psfilename.pop_back();
            }
            REPLMode = false;

            std::cout << "Running file " << psfilename << "\n";
            std::string exit = runFile() ? "successfully" : "with an error";
            std::cout << "\nProgram exited " << exit << "\n";
            
            psfilename = "<stdin>";
            REPLMode = true;
            continue;
        }

        for (const std::string &keyword : multilineKeywords) {
            if (code.starts_with(keyword)) {
                if (keyword == "TYPE" && code.find("=") != std::string::npos) break;
                std::string line = " ";
                while (line.size() > 0) {
                    code += "\n";
                    if (!getLine(line, ". ")) return true;
                    code += line;
                }
                break;
            }
        }

        lexer.setExpr(&code);
        try {
            const std::vector<Token*> &tokens = lexer.makeTokens();
            parser.setTokens(&tokens);
            PSC::Block *block = parser.parse();

            std::cout.precision(10);
            block->run(*globalCtx);
        } catch (const PSC::Error &e) {
            std::cout << "\n";
            e.print();
        }
    }
    return true;
}
