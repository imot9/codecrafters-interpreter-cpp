#include "Lox.hpp"

#include <iostream>

bool Lox::had_error = false;

void Lox::error(int line, const std::string& message) {
    report(line, std::string(), message);
}

void Lox::report(int line, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    Lox::had_error = true;
}