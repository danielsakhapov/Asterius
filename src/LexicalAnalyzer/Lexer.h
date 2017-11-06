#ifndef LEXER
#define LEXER

#include <string>
#include <fstream>
#include "BasicStructures.hpp"
#include "TypeInfo.h"

namespace asterius
{

class Lexer final
{
public:
    explicit Lexer(const std::string& filename);
    //Token getNextToken();
private:
    void getch();
    char peek();
    bool match(char c);

    std::ifstream reader_;
    size_t line_;
    size_t character_;
};

}

#endif // !LEXER