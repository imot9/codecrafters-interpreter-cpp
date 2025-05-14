#include "Token.hpp"

Token::Token(TokenType type, std::string_view lexeme, std::string_view literal, int line)
    : type(type), lexeme(lexeme), literal(literal), line(line) {}


std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token_type_to_string(token.type) << " " << token.lexeme << " ";
    if (token.literal.empty()) {
        os << "null";
    } else {
        os << token.literal;
    }
    return os;
}