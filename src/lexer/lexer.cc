#include "lexer.h"
#include <cctype>
#include <string>
#include "tokens.h"

namespace wrc {

// ------------------------------
// Constructor
// ------------------------------
Lexer::Lexer(const std::string& input) : src_(input) {}

// ------------------------------
// Helpers
// ------------------------------
bool Lexer::is_at_eof() const {
    return pos_ >= src_.size();
}

// Advance by one character and return it
char Lexer::advance() {
    if (is_at_eof()) return '\0';
    char c = src_[pos_++];
    if (c == '\n') {
        line_++;
        col_ = 1;
    } else {
        col_++;
    }
    return c;
}

// Peek at current character without advancing
char Lexer::peek() const {
    return is_at_eof() ? '\0' : src_[pos_];
}

// Skip whitespace, comments, line continuations
void Lexer::skip_trivia() {
    while (!is_at_eof()) {
        char c = peek();

        // Whitespace
        if (std::isspace(c)) {
            advance();
            continue;
        }

        // Line continuation: '\'
        if (c == '\\' && pos_ + 1 < src_.size() && src_[pos_ + 1] == '\n') {
            advance(); // skip '\'
            advance(); // skip '\n'
            line_++;
            col_ = 1;
            continue;
        }

        // Comments
        if (c == '/' && pos_ + 1 < src_.size()) {
            char next = src_[pos_ + 1];
            if (next == '/') { // single-line comment
                advance(); advance();
                while (!is_at_eof() && peek() != '\n') advance();
                continue;
            } else if (next == '*') { // multi-line comment
                advance(); advance();
                while (!is_at_eof()) {
                    if (peek() == '*' && pos_ + 1 < src_.size() && src_[pos_ + 1] == '/') {
                        advance(); advance();
                        break;
                    }
                    advance();
                }
                continue;
            }
        }

        break;
    }
}

// ------------------------------
// Lex operators and delimiters (1-3 chars)
// ------------------------------
Token Lexer::lex_operator() {
    size_t startCol = col_;
    std::string lexeme;

    lexeme += advance(); // first char

    // try add 2 characters more to the check
    for (int i = 0; i < 2; ++i) {
        char next = peek();
        if (next == '\0') break;

        std::string trial = lexeme + next;
        if (Tokens::TokenMap.count(trial)) {
            lexeme += advance();
        } else {
            break;
        }
    }

    Tokens::TokenID id = Tokens::GetTokenID(lexeme);
    return {id, lexeme, line_, static_cast<int>(startCol)};
}

// ------------------------------
// Tokenize next token
// ------------------------------
Token Lexer::Tokenize() {
    skip_trivia();
    if (is_at_eof()) return {Tokens::TokenID::EndOfFile, "", line_, col_};

    size_t startCol = col_;
    char c = peek();

    // Identifiers / Keywords
    if (std::isalpha(c) || c == '_') {
        std::string lexeme;
        while (std::isalnum(peek()) || peek() == '_') lexeme += advance();

        Tokens::TokenID id = Tokens::GetTokenID(lexeme);
        
        // If the token is not recognized, treat it as an identifier
        if (id == Tokens::TokenID::Error)
            id = Tokens::TokenID::Identifier;
        return {id, lexeme, line_, static_cast<int>(startCol)};
    }

    // Numbers / Constants
    if (std::isdigit(c)) {
        std::string lexeme;
        while (std::isdigit(peek())) lexeme += advance();
        return {Tokens::TokenID::Constant, lexeme, line_, static_cast<int>(startCol)};
    }

    // String literals
    if (c == '"') {
        std::string lexeme;
        advance(); // skip opening "
        while (!is_at_eof() && peek() != '"') {
            if (peek() == '\\') { // escape sequence
                lexeme += advance(); // '\'
                if (!is_at_eof()) lexeme += advance(); // escaped char
            } else {
                lexeme += advance();
            }
        }
        if (peek() == '"')
            advance(); // skip closing "
        return {Tokens::TokenID::StringLiteral, lexeme, line_, static_cast<int>(startCol)};
    }

    // Char literals
    if (c == '\'') {
        std::string lexeme;
        advance(); // skip opening '
        if (!is_at_eof()) {
            if (peek() == '\\') { // escape
                lexeme += advance(); // '\'
                if (!is_at_eof()) lexeme += advance(); // escaped char
            } else {
                lexeme += advance();
            }
        }
        if (peek() == '\'') 
            advance(); // skip closing '
        return {Tokens::TokenID::Constant, lexeme, line_, static_cast<int>(startCol)};
    }

    // Operators / Delimiters
    return lex_operator();
}

} // namespace wrc