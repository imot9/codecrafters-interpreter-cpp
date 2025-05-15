#include "TokenType.hpp"

#include <string_view>
#include <ostream>
#include <variant>

class Token {
public:
    TokenType type;
    std::string_view lexeme;
    std::variant<std::string_view, float> literal;
    int line;

    Token(TokenType, std::string_view, std::variant<std::string_view, float>, int);
};

std::ostream& operator<<(std::ostream& os, const Token& token);