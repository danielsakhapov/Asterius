#ifndef LEXER
#define LEXER

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include "TypeInfo.h"
#include "SymbolTable.h"
#include "Program/Program.h"
#include "BasicStructures.hpp"

namespace asterius
{
 
class Lexer final
{
public:
    explicit Lexer(const std::string& filename, Program& program);
    Token getNextToken();
    bool eof();
private:
    void getch();
    char peek();
    bool match(char c);
    Token readNumber();
    Token readWord();
    TokenType findKeyword(const std::string& name) const noexcept;
    void unknown();

    std::ifstream reader_;
    size_t line_;
    size_t character_;
    Program& program_;
    static std::map<std::string, TokenType> keywords_;
};

}

#endif // !LEXER