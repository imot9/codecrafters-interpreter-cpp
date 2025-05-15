#include "Token.hpp"

#include <iomanip>

bool all_zero(std::string_view, size_t);

Token::Token(TokenType type, std::string_view lexeme, std::variant<std::string_view, float> literal, int line)
    : type(type), lexeme(lexeme), literal(literal), line(line) {}


std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token_type_to_string(token.type) << " " << token.lexeme << " ";
    std::visit([&os, &token](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::string_view>) {
            if (arg.empty()) os << "null";
            else os << arg;
        } else if constexpr (std::is_same_v<T, float>) {
            size_t dot_pos = token.lexeme.find('.');
            if (dot_pos == std::string_view::npos) {
                os << std::fixed << std::setprecision(1) << arg;        // No decimal part
            } else {
                if (all_zero(token.lexeme, dot_pos)) {
                    os << std::fixed << std::setprecision(1) << arg;    // Decimal part (only zeroes)
                } else {
                    os << token.lexeme;                                 // Decimal part
                }
            }
        }
    }, token.literal);

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