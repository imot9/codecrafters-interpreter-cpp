#include "Parser.hpp"

#include "Lox.hpp"

Parser::Parser(std::vector<Token> tokens) : tokens(tokens) {}

Expr *Parser::parse()
{
    try
    {
        return this->expression();
    }
    catch(ParseError e)
    {
        return nullptr;
    }
    
}

Expr* Parser::expression() {
    return this->equality();
}

Expr* Parser::equality() {
    Expr* expr = this->comparison();

    while (this->match({BANG_EQUAL, EQUAL_EQUAL})) {
        Token* op = this->previous();
        Expr* right = this->comparison();
        expr = new Binary(expr, op, right);
    }

    return expr;
}

Expr* Parser::comparison() {
    Expr* expr = this->term();

    while (this->match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        Token* op = this->previous();
        Expr* right = this->term();
        expr = new Binary(expr, op, right);
    }

    return expr;
}

Expr* Parser::term() {
    Expr* expr = this->factor();

    while (this->match({MINUS, PLUS})) {
        Token* op = this->previous();
        Expr* right = this->factor();
        expr = new Binary(expr, op, right);
    }

    return expr;
}

Expr* Parser::factor() {
    Expr* expr = this->unary();

    while (this->match({SLASH, STAR})) {
        Token* op = this->previous();
        Expr* right = this->unary();
        expr = new Binary(expr, op, right);
    }

    return expr;
}

Expr* Parser::unary() {
    while (this->match({BANG, MINUS})) {
        Token* op = this->previous();
        Expr* right = this->unary();
        return new Unary(op, right);
    }

    return this->primary();
}

Expr* Parser::primary() {
    if (this->match({FALSE})) return new Literal("false");
    if (this->match({TRUE})) return new Literal("true");
    if (this->match({NIL})) return new Literal("nil");

    if (this->match({NUMBER, STRING})) {
        return new Literal(this->previous()->literal);
    }

    if (this->match({LEFT_PAREN})) {
        Expr* expr = this->expression();
        this->consume(RIGHT_PAREN, "Expect ')' after expression.");
        return new Grouping(expr);
    }

    throw this->error(this->peek(), "Expect expression.");
}

Token* Parser::consume(TokenType type, const std::string& message)
{
    if (this->check(type)) return this->advance();
    throw this->error(this->peek(), message);
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (auto type : types) {
        if (this->check(type)) {
            this->advance();
            return true;
        }
    }

    return false;
}

bool Parser::check(TokenType type) {
    if (this->is_at_end()) return false;
    return this->peek()->type == type;
}

Token* Parser::advance() {
    if (!is_at_end()) current++;
    return this->previous();
}

bool Parser::is_at_end() {
    return this->peek()->type == END_OF_FILE;
}

Token* Parser::peek() {
    return &this->tokens[current];
}

Token* Parser::previous() {
    return &this->tokens[current - 1];
}

Parser::ParseError* Parser::error(Token* token, const std::string& message) {
    Lox::error(token, message);
    return new Parser::ParseError();
}

void Parser::synchronize() {
    this->advance();

    while (!this->is_at_end()) {
        if (this->previous()->type == SEMICOLON) return;

        switch (this->peek()->type)
        {
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
            return;

        default:
            break;
        }

        this->advance();
    }
}
