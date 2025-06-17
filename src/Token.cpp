#include "Token.hpp"

#include <iomanip>

bool all_zero(std::string_view, size_t);

Token::Token(TokenType type, std::string_view lexeme, std::string_view literal, int line)
    : type(type), lexeme(lexeme), literal(literal), line(line) {}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token_type_to_string(token.type) << " " << token.lexeme << " ";
    if (token.literal.empty()) {
        os << "null";
    } else if (token.type == NUMBER) {
        size_t dot_pos = token.lexeme.find('.');
        if (dot_pos == std::string_view::npos || all_zero(token.lexeme, dot_pos)) {
            float value = std::stof(std::string(token.lexeme));
            os << std::fixed << std::setprecision(1) << value;
        } else {
            os << token.lexeme;
        }
    } else {
        os << token.literal;
    }

    return os;
}

bool all_zero(std::string_view lexeme, size_t decimal) {
    bool all_zero = true;
    for (size_t i = decimal + 1; i < lexeme.size(); ++i) {
        if (lexeme[i] != '0') {
            all_zero = false;
            break;
        }
    }
    return all_zero;
}