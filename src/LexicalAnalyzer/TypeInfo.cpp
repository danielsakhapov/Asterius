#include "TypeInfo.h"

using namespace std;
using namespace asterius;

Data::Data() noexcept
    : isRef_(false)
{
}

Data::Data(DataType type, size_t size, size_t line, size_t chr, char isRef) noexcept
    : type_(type),
    size_(size),
    line_(line),
    char_(chr),
    isRef_(isRef)
{
}

Token Token::none;

Token::Token(TokenType id, std::string&& name, Data data)
    : id_(id),
    name_(std::move(name)),
    data_(data)
{
}

Token::Token(TokenType id, const std::string& name, Data data)
    : id_(id),
    name_(name),
    data_(data)
{
}

const string& Token::getName() const noexcept
{
    return name_;
}

TokenType Token::getType() const noexcept
{
    return id_;
}

bool asterius::operator<(const Token& lhs, const Token& rhs)
{
    return lhs.name_ < rhs.name_;
}