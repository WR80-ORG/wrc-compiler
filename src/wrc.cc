#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "tokens.h"

namespace wrc {
    
int Start(int argc, char** argv) {
    std::cout << "Debug!\n\n";

    if (argc < 2) {
        std::cout << "wrc <file_name>\n";
        return 0;
    }
    
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "The file you tried to open is a folder or does not exist.\n";
        return 1;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();
    file.close();

    Lexer lexer(code);
    
    while (true) {
        Token token = lexer.Tokenize();
        if (token.id == Tokens::TokenID::EndOfFile) 
            break;

        std::cout << "Token: " << static_cast<int>(token.id)
                  << " | Lexeme: \"" << token.lexeme << "\""
                  << " | Line: " << token.line
                  << " | Col: " << token.col << "\n";
    }

    return 0;
}

} // namespace wrc