#include "TokenType.hpp"

#include <string_view>
#include <ostream>

class Token {
public:
    TokenType type;
    std::string_view lexeme;
    std::string_view literal;
    int line;

    Token(TokenType, std::string_view, std::string_view, int);
};

std::ostream& operator<<(std::ostream& os, const Token& token);