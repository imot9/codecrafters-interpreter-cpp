#include "Scanner.hpp"
#include "Lox.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

void run(const std::string&);
void run_file(const std::string&, const std::string&);
void run_prompt();
std::string read_file_contents(const std::string& filename);

int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // Interactive mode
    if (argc == 1) {
        run_prompt();
        return 0;
    // File handling
    } else {
        run_file(argv[1], argv[2]);
    }

    if (Lox::had_error) {
        exit(65);
    }

    return 0;
}

void run(std::string& source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan_tokens();

    for (auto token : tokens) {
        std::cout << token << std::endl;
    }
}

void run_file(const std::string& command, const std::string& path) {
    if (command == "tokenize") {
        std::string file_contents = read_file_contents(path);
        run(file_contents);
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
    }
}

void run_prompt() {
    std::string line;

    for (;;) {
        std::cout << "> ";
        std::cin >> line;
        if (line == "") { break; }
        run(line);
    }
}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
