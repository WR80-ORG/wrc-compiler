#pragma once
#include <unordered_map>
#include <string_view>

namespace wrc::Tokens {

// ------------------------------
// Full list of C tokens
// Each token has: ID, string representation, isKeyword/isPrecompiled flag
// ------------------------------
#define TOKEN_LIST \
    \
    /* Keywords */ \
    \
    X(Auto, "auto", true, false) \
    X(Asm, "asm", true, false) \
    X(AsmLine, "__asm", true, false) \
    X(Break, "break", true, false) \
    X(Case, "case", true, false) \
    X(Char, "char", true, false) \
    X(Const, "const", true, false) \
    X(Continue, "continue", true, false) \
    X(Default, "default", true, false) \
    X(Do, "do", true, false) \
    X(Double, "double", true, false) \
    X(Else, "else", true, false) \
    X(Enum, "enum", true, false) \
    X(Extern, "extern", true, false) \
    X(Float, "float", true, false) \
    X(For, "for", true, false) \
    X(Goto, "goto", true, false) \
    X(If, "if", true, false) \
    X(Inline, "inline", true, false) \
    X(Int, "int", true, false) \
    X(Long, "long", true, false) \
    X(Mold, "mold", true, false) \
    X(Register, "register", true, false) \
    X(Restrict, "restrict", true, false) \
    X(Return, "return", true, false) \
    X(Short, "short", true, false) \
    X(Signed, "signed", true, false) \
    X(Sizeof, "sizeof", true, false) \
    X(Static, "static", true, false) \
    X(Struct, "struct", true, false) \
    X(Switch, "switch", true, false) \
    X(Typedef, "typedef", true, false) \
    X(Uint, "uint", true, false) \
    X(Union, "union", true, false) \
    X(Unsigned, "unsigned", true, false) \
    X(Void, "void", true, false) \
    X(Volatile, "volatile", true, false) \
    X(While, "while", true, false) \
    \
    /* Preprocessor directives */ \
    \
    X(HDefine, "#define", false, true) \
    X(HUndef, "#undef", false, true) \
    X(HInclude, "#include", false, true) \
    X(HIf, "#if", false, true) \
    X(HIfdef, "#ifdef", false, true) \
    X(HIfndef, "#ifndef", false, true) \
    X(HElse, "#else", false, true) \
    X(HElif, "#elif", false, true) \
    X(HEndif, "#endif", false, true) \
    X(HError, "#error", false, true) \
    X(HPragma, "#pragma", false, true) \
    X(HWarning, "#warning", false, true) \
    X(HLine, "#line", false, true) \
    \
    /* Operators */ \
    \
    X(Plus, "+", false, false) \
    X(Minus, "-", false, false) \
    X(Star, "*", false, false) \
    X(Slash, "/", false, false) \
    X(Percent, "%", false, false) \
    X(PlusPlus, "++", false, false) \
    X(MinusMinus, "--", false, false) \
    X(Eq, "==", false, false) \
    X(Neq, "!=", false, false) \
    X(Gt, ">", false, false) \
    X(Lt, "<", false, false) \
    X(Ge, ">=", false, false) \
    X(Le, "<=", false, false) \
    X(And, "&&", false, false) \
    X(Or, "||", false, false) \
    X(Not, "!", false, false) \
    X(BitAnd, "&", false, false) \
    X(BitOr, "|", false, false) \
    X(BitXor, "^", false, false) \
    X(BitNot, "~", false, false) \
    X(Shl, "<<", false, false) \
    X(Shr, ">>", false, false) \
    X(Assign, "=", false, false) \
    X(PlusAssign, "+=", false, false) \
    X(MinusAssign, "-=", false, false) \
    X(MulAssign, "*=", false, false) \
    X(DivAssign, "/=", false, false) \
    X(ModAssign, "%=", false, false) \
    X(AndAssign, "&=", false, false) \
    X(OrAssign, "|=", false, false) \
    X(XorAssign, "^=", false, false) \
    X(ShlAssign, "<<=", false, false) \
    X(ShrAssign, ">>=", false, false) \
    X(Question, "?", false, false) \
    X(Colon, ":", false, false) \
    X(Comma, ",", false, false) \
    X(Dot, ".", false, false) \
    X(Arrow, "->", false, false) \
    X(Ellipsis, "...", false, false) \
    \
    /* Delimiters */ \
    \
    X(LParen, "(", false, false) \
    X(RParen, ")", false, false) \
    X(LBrace, "{", false, false) \
    X(RBrace, "}", false, false) \
    X(LBracket, "[", false, false) \
    X(RBracket, "]", false, false) \
    X(Semicolon, ";", false, false) \
    \
    /* Literals & Identifiers */  \
    \
    X(Identifier, "NoStr", false, false) \
    X(Constant, "NoStr", false, false) \
    X(StringLiteral, "NoStr", false, false) \
    X(Error, "NoStr", false, false) \
    X(EndOfFile, "NoStr", false, false)

// ------------------------------
// Enum of token IDs
// ------------------------------
enum class TokenID {
#define X(id, str, keyword, precomp) id,
    TOKEN_LIST
#undef X
};

struct TokenType {
    TokenID id;
    std::string_view name;
    bool isKeyword = false;
    bool isPrecompiled = false;
};


// Token table for quick iteration or mapping
inline const TokenType TokenTable[] = {
#define X(id, str, keyword, precomp) { TokenID::id, str, keyword, precomp },
    TOKEN_LIST
#undef X
};

// Map for quick lookup by token name
inline const std::unordered_map<std::string_view, TokenType> TokenMap = [] {
    std::unordered_map<std::string_view, TokenType> m;
    for (auto& t : TokenTable) m[t.name] = t;
    return m;
}();

// ------------------------------
// Utility functions
// ------------------------------
inline bool IsKeyword(std::string_view name) {
    auto it = TokenMap.find(name);
    return it != TokenMap.end() && it->second.isKeyword;
}

inline bool IsPrecompiled(std::string_view name) {
    auto it = TokenMap.find(name);
    return it != TokenMap.end() && it->second.isPrecompiled;
}

inline TokenID GetTokenID(std::string_view name) {
    auto it = TokenMap.find(name);
    return it != TokenMap.end() ? it->second.id : TokenID::Error;
}

} // namespace wrc::Tokens