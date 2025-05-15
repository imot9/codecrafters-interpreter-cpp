#include "Token.hpp"

#include <string>
#include <list>

class Scanner {
public:
    std::string& source;
    std::list<Token> tokens;
    int start;
    int current;
    int line;


    Scanner(std::string&);
    std::list<Token> scan_tokens();
    void scan_token();
    void add_token(TokenType);
    void add_token(TokenType, const std::variant<std::string_view, float>);

    void string();
    void number();
    void identifier();

    char advance();
    char peek();
    char peek_next();
    bool match(char);
    bool is_at_end();
    bool is_digit(char);
    bool is_alpha(char);
    bool is_alphanumeric(char);
};