#include "Lexer.h"

using namespace std;
using namespace asterius;

map<string, TokenType> Lexer::keywords_ = 
{
    { "while", TokenType::WHILE },
    { "if",    TokenType::IF },
    { "int",   TokenType::INT },
    { "float", TokenType::FLOAT },
    { "read",  TokenType::READ },
    { "write", TokenType::WRITE }
};

Lexer::Lexer(const string& filename, Program& program)
    : reader_(filename),
    line_(1),
    character_(1),
    program_(program)
{
}

Token Lexer::getNextToken()
{
    for ( ; peek() != EOF ; getch()) {
        switch (peek())
        {// skip space, tab, new line symbols
        case ' ':
        case '\t': 
            continue;
        case '\n':
            character_ = 0;
            ++line_;
            continue;
        case '=':
            if (match('=')) {
                getch();
                return Token(TokenType::EQ);
            }
            return Token(TokenType::ASSIGN);
        case '<':
            if (match('=')) {
                getch();
                return Token(TokenType::LE);
            }
            return Token(TokenType::LT);
        case '>':
            if (match('=')) {
                getch();
                return Token(TokenType::GE);
            }
            return Token(TokenType::GT);
        case '+':
            getch();
            return Token(TokenType::PLUS);
        case '-':
            getch();
            return Token(TokenType::MINUS);
        case '*':
            getch();
            return Token(TokenType::PRODUCT);
        case '/':
            getch();
            return Token(TokenType::DIVISION);
        case '(':
            getch();
            return Token(TokenType::OPEN_BRACKET);
        case ')':
            getch();
            return Token(TokenType::CLOSE_BRACKET);
        case ';':
            getch();
            return Token(TokenType::STATEMENT_END);
        default:
            break;
        }
        if (isDigit(peek())) { // integer or float
            return readNumber();
        }
        else if (isLetter(peek())) { // id or keyword
            return readWord();
        }
        //unknown symbol raise exception
        unknown();
    }
    return Token(TokenType::NONE);
}

bool Lexer::eof()
{
    return reader_.peek() == EOF;
}

void Lexer::getch()
{
    ++character_;
    reader_.get();
}

char Lexer::peek()
{
    return (char)reader_.peek();
}

bool Lexer::match(char c)
{
    getch();
    return peek() == c;
}

Token Lexer::readNumber() 
{
    int num = charToInt(peek());
    getch();
    while (isDigit(peek())) {
        num = num * 10 + charToInt(peek());
        getch();
    }
    if (peek() != '.') {// integer
        Data data(DataType::INT, INT_SIZE, line_, character_, true);
        program_.addConstant(&num, data);
        return Token(TokenType::CONST, data);
    }
    getch(); // double
    double flt = num;
    double tail = 0.1;
    while (isDigit(peek())) {
        flt += tail * charToInt(peek());
        getch();
        tail /= 10;
    }
    Data data(DataType::FLOAT, FLOAT_SIZE, line_, character_, true);
    program_.addConstant(&flt, data);
    return Token(TokenType::CONST, data);
}

Token Lexer::readWord()
{
    string name;
    while (isDigit(peek()) || isLetter(peek())) {
        name.push_back(peek());
        getch();
    }
    auto type = findKeyword(name);
    if (type != TokenType::NONE) //keyword
        return Token(type);
    return Token(TokenType::ID, Data(), std::move(name));
}

TokenType Lexer::findKeyword(const string& name) const noexcept
{
    auto it = keywords_.find(name);
    if (it != keywords_.end())
        return it->second;
    return TokenType::NONE;
}

void Lexer::unknown()
{
    stringstream tmp;
    tmp << "Unknown symbol at line " << line_ << " character " << character_;
    throw logic_error(tmp.str());
}