#include <iostream>
#include <fstream>
#include <string>

enum TokenType {
    INT,
    FLOAT,
    CHAR,
    STRING
};

struct Token {
    TokenType type;
    std::string value;
    int line;
};

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: lexer <filename>\n" << std::endl;
        exit(1);
    }
    std::ifstream ifs(argv[1]);
    if (!ifs.is_open()) {
        std::cerr << "Cannot open file " << argv[1] << std::endl;
    }
}
