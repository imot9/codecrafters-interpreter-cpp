#include "Expr.hpp"

#include <vector>

class Parser {
public:
    Parser(std::vector<Token>);
    Expr* parse();

private:
    class ParseError : std::exception {};
    std::vector<Token> tokens;
    int current = 0;

    Expr* expression();
    Expr* equality();
    Expr* comparison();
    Expr* term();
    Expr* factor();
    Expr* unary();
    Expr* primary();
    
    Token* consume(TokenType type, const std::string& message);

    bool match(std::initializer_list<TokenType> types);
    bool check(TokenType type);
    bool is_at_end();
    Token* advance();
    Token* peek();
    Token* previous();
    ParseError* error(Token* token, const std::string& message);
    void synchronize();
};