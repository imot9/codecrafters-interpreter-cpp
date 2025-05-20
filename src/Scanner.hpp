#include "Token.hpp"

#include <string>
#include <vector>

class Scanner {
public:
    Scanner(const std::string&);
    std::vector<Token> scan_tokens();

private:
    const std::string& source;
    std::vector<Token> tokens;
    int start;
    int current;
    int line;

    void scan_token();
    void add_token(TokenType);
    void add_token(TokenType, const std::string_view);

    void string();
    void number();
    void identifier();

    char advance();
    bool match(char);

    char peek() const;
    char peek_next() const;
    bool is_at_end() const;
    bool is_digit(char) const;
    bool is_alpha(char) const;
    bool is_alphanumeric(char) const;
};