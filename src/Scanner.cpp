#include "Scanner.hpp"
#include "Lox.hpp"

#include <list>
#include <string.h>
#include <iostream>

Scanner::Scanner(const std::string& source)
    : source(source), tokens(), start(0), current(0), line(1) { }

std::vector<Token> Scanner::scan_tokens() {
    while (!is_at_end()) {
        start = current;
        this->scan_token();
    }

    this->tokens.push_back(Token(TokenType::END_OF_FILE, std::string_view(), std::string_view(), this->line));
    return tokens;
}

void Scanner::scan_token() {
    char c = this->advance();
    switch (c) {
        case '(': this->add_token(TokenType::LEFT_PAREN); break;
        case ')': this->add_token(TokenType::RIGHT_PAREN); break;
        case '{': this->add_token(TokenType::LEFT_BRACE); break;
        case '}': this->add_token(TokenType::RIGHT_BRACE); break;
        case ',': this->add_token(TokenType::COMMA); break;
        case '.': this->add_token(TokenType::DOT); break;
        case '-': this->add_token(TokenType::MINUS); break;
        case '+': this->add_token(TokenType::PLUS); break;
        case ';': this->add_token(TokenType::SEMICOLON); break;
        case '*': this->add_token(TokenType::STAR); break;

        case '!': this->add_token(this->match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=': this->add_token(this->match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<': this->add_token(this->match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>': this->add_token(this->match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;

        case '/':
            if (this->match('/')) {
                while (this->peek() != '\n' && !this->is_at_end()) this->advance();
            } else {
                this->add_token(TokenType::SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            break;
        case '"':
            this->string();
            break;

        case '\n':
            line++;
            break;

        default:
            if (this->is_digit(c)) {
                this->number();
            } else if (this->is_alpha(c)) {
                this->identifier();
            } else {
                std::string err_msg = std::string("Unexpected character: ");
                err_msg.push_back(c);
                Lox::error(line, err_msg);
            }
            break;
    }
}

void Scanner::add_token(TokenType type) {
    this->add_token(type, std::string_view());
}

void Scanner::add_token(TokenType type, std::string_view literal) {
    std::string_view text = std::string_view(this->source).substr(start, current - start);
    this->tokens.push_back(Token(type, text, literal, line));
}

char Scanner::advance() {
    return this->source.at(current++);
}

bool Scanner::match(char expected) {
    if (this->is_at_end()) {
        return false;
    }
    if (this->source.at(current) != expected) {
        return false;
    }

    current++;
    return true;
}

void Scanner::string() {
    while (this->peek() != '"' && !this->is_at_end()) {
        if (this->peek() == '\n') {
            line++;
        }
        this->advance();
    }

    if (this->is_at_end()){
        Lox::error(line, "Unterminated string.");
        return;
    }

    // Closing "
    this->advance();

    std::string_view value = std::string_view(this->source).substr(this->start + 1, (this->current - 1) - (this->start + 1));
    add_token(TokenType::STRING, value);
}

void Scanner::number() {
    while(this->is_digit(this->peek())) this->advance();

    if (this->peek() == '.' && this->is_digit(this->peek_next())) {
        // Consume '.'
        this->advance();

        while(this->is_digit(this->peek())) this->advance();
    }

    this->add_token(TokenType::NUMBER, std::string_view(this->source).substr(this->start, this->current - this->start));
}

void Scanner::identifier() {
    while(this->is_alphanumeric(this->peek())) this->advance();

    std::string text = source.substr(this->start, this->current - this->start);
    TokenType type = TokenType::IDENTIFIER;

    try {
        type = keywords.at(text);
    } catch (std::out_of_range) {}

    this->add_token(type);
}

char Scanner::peek() const {
    if (this->is_at_end()) {
        return '\0';
    }
    return source.at(current);
}

char Scanner::peek_next() const {
    if (current + 1 >= this->source.length()) {
        return '\0';
    }

    return source.at(current + 1);
}

bool Scanner::is_at_end() const {
    return this->current >= this->source.length();
}

bool Scanner::is_digit(char c) const {
    return c >= '0' && c <= '9';
}

bool Scanner::is_alpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::is_alphanumeric(char c) const {
    return this->is_alpha(c) || this->is_digit(c);
}
