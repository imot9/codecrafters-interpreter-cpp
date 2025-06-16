#include "Scanner.hpp"
#include "Parser.hpp"
#include "AstPrinter.hpp"
#include "Lox.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

void run_tokenize(const std::string&);
void run_parse(const std::string&);
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

void run_tokenize(std::string& source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan_tokens();

    for (auto token : tokens) {
        std::cout << token << std::endl;
    }
}

void run_parse(std::string& source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan_tokens();
    Parser parser(tokens);
    AstPrinter* printer = new AstPrinter();

    Expr* expression = parser.parse();
    std::cout << printer->print(*expression);
}

void run_file(const std::string& command, const std::string& path) {
    if (command == "tokenize") {
        std::string file_contents = read_file_contents(path);
        run_tokenize(file_contents);
    } else if (command == "parse") {
        std::string file_contents = read_file_contents(path);
        run_parse(file_contents);
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
        run_parse(line);
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
