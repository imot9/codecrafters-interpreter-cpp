#include "Lox.hpp"

#include <iostream>

bool Lox::had_error = false;

void Lox::error(int line, const std::string& message) {
    report(line, std::string(), message);
}

void Lox::error(Token* token, const std::string& message) {
    if (token->type == TokenType::END_OF_FILE){
        report(token->line, " at end", message);
    } else {
        std::string where = " at '" + std::string(token->lexeme) + "'";
        report(token->line, where, message);
    }
}

void Lox::report(int line, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    Lox::had_error = true;
}