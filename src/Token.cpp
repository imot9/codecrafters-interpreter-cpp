#include "Token.hpp"

#include <iomanip>

bool all_zero(std::string_view, size_t);

Token::Token(TokenType type, std::string_view lexeme, std::variant<std::string_view, float> literal, int line)
    : type(type), lexeme(lexeme), literal(literal), line(line) {}


std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token_type_to_string(token.type) << " " << token.lexeme << " ";

    std::visit([&os, &token](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, float>) {
            std::ostringstream ss;
            os << arg;
            os << ss.str();
        } else if constexpr (std::is_same_v<T, std::string_view>) {
            if (arg.empty()) {
                os << "null";
            } else {
                os << std::string(arg);
            }
        }
    }, token.literal);

    return os;
}