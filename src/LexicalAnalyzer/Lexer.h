#ifndef LEXER
#define LEXER

#include <memory>
#include <string>
#include <fstream>
#include "BasicStructures.hpp"

namespace asterius
{

class Token
{
public:
    Token(TokenType id) noexcept;
private:
    TokenType id_;
};

class Number : public Token {
public:
    explicit Number(int val) noexcept;
private:
    int val_;
};

class Word : public Token {
public:
    explicit Word(const std::string& str, TokenType type = TokenType::Id);
private:
    std::string val_;
};

class Double : public Token {
public:
    explicit Double(double val) noexcept;
private:
    double val_;
};

class Lexer final
{
public:
    explicit Lexer(const std::string& filename);
    std::unique_ptr<Token> getNextToken();
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