#ifndef LEXER
#define LEXER

#include <memory>
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
    std::unique_ptr<Token> getNextToken();
    size_t line() const noexcept;
    size_t character() const noexcept;
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