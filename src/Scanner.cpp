#include "Scanner.hpp"

#include <list>
#include <string.h>
#include <iostream>

Scanner::Scanner(std::string& source)
    : source(source), tokens(), start(0), current(0), line(1) { }

std::list<Token> Scanner::scan_tokens() {
    while (!is_at_end()) {
        start = current;
        this->scan_token();
    }

    this->tokens.push_back(Token(TokenType::END_OF_FILE, std::string_view(), std::string_view(), this->line));
    return tokens;
}

bool Scanner::is_at_end() {
    return this->current >= this->source.length();
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
                while (this->peek() != '\n' && !this->is_at_end());
            } else {
                this->add_token(TokenType::SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            break;

        case '\n':
            line++;
            break;

        default:
            std::cout << "Unexpected character: " << c << std::endl;
            break;
    }
}

char Scanner::advance() {
    return this->source.at(current++);
}

char Scanner::peek() {
    if (this->is_at_end()) {
        return '\0';
    }
    return source.at(current);
}

void Scanner::add_token(TokenType type) {
    this->add_token(type, std::string_view());
}

void Scanner::add_token(TokenType type, std::string_view literal) {
    std::string_view text = std::string_view(this->source).substr(start, current - start);
    this->tokens.push_back(Token(type, text, literal, line));
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