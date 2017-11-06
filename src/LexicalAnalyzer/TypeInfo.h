#ifndef TYPE_INFO
#define TYPE_INFO

#include "BasicStructures.hpp"

namespace asterius
{
enum class TokenType
{
    CONST,
    VAR,
    WHILE,
    IF,
    TYPE,
    AND,
    OR,
    EQ,
    TRUE,
    FALSE,
    READ,
    WRITE
};

class Token
{
public:
    Token(TokenType id, Data data, size_t line, size_t character);
private:
    TokenType id_;
    size_t line_;
    size_t char_;
    Data data_;
};
}
#endif // !TYPE_INFO


