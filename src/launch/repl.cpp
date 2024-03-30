#include "pch.h"

#include <string>
#include "launch/run.h"
#include "PsConfig.h"
#include "line_util.h"

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

bool startREPL() {
    std::cout << "PseudoEngine2 v" << PseudoEngine2_VERSION_MAJOR << '.' << PseudoEngine2_VERSION_MINOR << '.' << PseudoEngine2_VERSION_PATCH << " REPL\nEnter '?' for help, 'EXIT' to quit\n";

    Lexer lexer;
    Parser parser;
    auto globalCtx = PSC::Context::createGlobalContext();

    while (true) {
        std::string code;
        if (!getLine(code, "> ")) break;

        size_t size = code.size();
        if (size == 0) continue;
        if (code == "?") {
            std::cout << "Visit https://github.com/SingularityT3/PseudoEngine2 for syntax, examples and more info\nUse `RUNFILE <filename>` to run programs stored in files" << std::endl;
            continue;
        } else if (code == "EXIT") {
            break;
        } else if (code.starts_with("RUNFILE")) {
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

            std::cout << "==> Running file '" << psfilename << "'\n";
            std::string exit = runFile() ? "successfully" : "with an error";
            std::cout << "\n==> Program exited " << exit << "\n";

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
