#pragma once
#include "TokenType.hpp"

#include <string_view>
#include <ostream>
#include <variant>

class Token {
public:
    Token(TokenType, std::string_view, std::string_view, int);
    friend std::ostream& operator<<(std::ostream& os, const Token& token);

    TokenType type;
    std::string_view lexeme;
    std::string_view literal;
    int line;
};