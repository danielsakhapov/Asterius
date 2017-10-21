#include "TypeInfo.h"

using namespace std;
using namespace asterius;

Token::Token(TokenType id) noexcept
    : id_(id)
{
}

Number::Number(int val) noexcept
    : Token(TokenType::INT),
    val_(val)
{
}

Word::Word(const string& str, TokenType type)
    : Token(type),
    val_(str)
{
}

Type::Type(const string& str, TokenType type, size_t width)
    : Word(str, type),
    width_(width)
{
}

size_t Type::width() const noexcept
{
    return width_;
}

Array::Array(const Type& type, size_t size)
    : Type("[]", TokenType::ARRAY, size * type.width()),
    type_(type),
    size_(size)
{
}

Double::Double(double val) noexcept
    : Token(TokenType::DOUBLE),
    val_(val)
{
}