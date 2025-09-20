#pragma once

#include "tokens.h"
#include <string>
#include <vector>

namespace wrc {

struct Token {
    Tokens::TokenID id;
    std::string lexeme;
    int line;
    int col;
};

class Lexer {
public:
    explicit Lexer(const std::string& input);
    
    Token Tokenize();
    Token NextToken();

private:
    const std::string src_;
    size_t pos_ = 0;
    int line_ = 1, col_ = 1;
    
    // ------------------------------
    // Helpers
    // ------------------------------
    bool is_at_eof() const;    // true if at end of input
    char advance();             // advance by one char and return it
    char peek() const;          // look at current char without advancing
    void skip_trivia();         // skip whitespace, comments, line continuations
    Token lex_operator();
};

}  // namespace wrc