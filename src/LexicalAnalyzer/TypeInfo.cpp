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

Token Token::none{TokenType::NONE};

Token::Token(TokenType id, Data data, std::string&& name)
    : id_(id),
    data_(data),
    name_(std::move(name))
{
}

const string& Token::getName() const noexcept
{
    return name_;
}

bool asterius::operator<(const Token& lhs, const Token& rhs)
{
    return lhs.name_ < rhs.name_;
}