#include <string>

#include "Token.hpp"

class Lox {
public:
    static void error(int, const std::string&);
    static void error(Token*, const std::string&);
    static bool had_error;
    
private:
    static void report(int, const std::string&, const std::string&);
};