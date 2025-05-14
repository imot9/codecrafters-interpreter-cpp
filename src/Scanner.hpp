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
    void add_token(TokenType, const std::string_view);
    char advance();
    char peek();
    bool match(char);
    bool is_at_end();
};