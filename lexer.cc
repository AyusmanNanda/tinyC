#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <sstream>
#include <vector>

enum class TokenType {
    Keyword,        // int, float, char, string, etc.
    Identifier,     // variable names
    Integer,        // integer
    Float,          // float
    Char,           // character 'a'
    String,         // string "hello"
    Operator,       // +, -, =, *, /, etc.
    Punctuation,    // ;, ,, (), {}, []
    Endline,        // newline
    EndOfFile,      // EOF
    Unknown         // Unknown
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
};

std::string readFile(const std::string& filename){
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    return source;
}

class Lexer{
private:
    std::string source;
    size_t position;
    int line;
    int column;
public:
    Lexer(const std::string& src): source(src), position(0), line(1), column(1) {};
    Token readToken() {
    while (position < source.size() && isspace(source[position])) {
        if (source[position] == '\n') { line++; column = 1; } 
        else { column++; }
        position++;
    }
    if (position >= source.size()) {
        Token t;
        t.type = TokenType::EndOfFile;
        t.lexeme = "";
        t.line = line;
        t.column = column;
        return t;
    }

    char current = source[position];
    int startColumn = column;

    if (isalpha(current) || current == '_') {
        std::string lexeme;
        while (position < source.size() && (isalnum(source[position]) || source[position] == '_')) {
            lexeme += source[position++];
            column++;
        }
        Token t;
        if (lexeme == "int" || lexeme == "float" || lexeme == "char" || lexeme == "string") {
            t.type = TokenType::Keyword;
        } else {
            t.type = TokenType::Identifier;
        }
        t.lexeme = lexeme;
        t.line = line;
        t.column = startColumn;
        return t;
    }

    if (isdigit(current)) {
        std::string lexeme;
        bool isFloat = false;
        while (position < source.size() && isdigit(source[position])) {
            lexeme += source[position++];
            column++;
        }
        if (position < source.size() && source[position] == '.') {
            isFloat = true;
            lexeme += source[position++];
            column++;
            while (position < source.size() && isdigit(source[position])) {
                lexeme += source[position++];
                column++;
            }
        }
        Token t;
        if (isFloat) {
            t.type = TokenType::Float;
        } else {
            t.type = TokenType::Integer;
        }
        t.lexeme = lexeme;
        t.line = line;
        t.column = startColumn;
        return t;
    }

    if (current == '+' || current == '-' || current == '*' || current == '/' || current == '=') {
        position++;
        column++;
        Token t;
        t.type = TokenType::Operator;
        t.lexeme = std::string(1, current);
        t.line = line;
        t.column = startColumn;
        return t;
    }

    if (current == ';' || current == ',' || current == '(' || current == ')' || current == '{' || current == '}') {
        position++;
        column++;
        Token t;
        t.type = TokenType::Punctuation;
        t.lexeme = std::string(1, current);
        t.line = line;
        t.column = startColumn;
        return t;
    }

    if (current == '\n') {
        position++;
        column = 1;
        line++;
        Token t;
        t.type = TokenType::Endline;
        t.lexeme = "\\n";
        t.line = line - 1;
        t.column = startColumn;
        return t;
    }

    position++;
    column++;
    Token t;
    t.type = TokenType::Unknown;
    t.lexeme = std::string(1, current);
    t.line = line;
    t.column = startColumn;
    return t;
}
    
};

int main(int argc, char *argv[]) {
    int word_count = 0;
    std::string word, filename;
    if (argc < 2) {
        std::cerr << "Usage: lexer <filename>\n" << std::endl;
        exit(1);
    }
    std::filesystem::path filepath(argv[1]);

    if(filepath.extension() != ".tinyc"){
        std::cout << "invalid source file \n" << std::endl;
        return 1;
    }
    std::string source = readFile(argv[1]);
    Lexer lex(source);

    std::vector<Token> tokens;

    while (true) {
    Token t = lex.readToken();
    tokens.push_back(t);
    if (t.type == TokenType::EndOfFile){
        break;
    }    
    }
    for(const auto& token : tokens){
        std::cout << "Token: " << token.lexeme << " Type: " << static_cast<int>(token.type) << " Line: " << token.line << " Column: " << token.column << "\n";
    }   
    
}
