#include "TypeInfo.h"

using namespace std;
using namespace asterius;

//Posistion
Position::Position() noexcept
    : line_(1),
    char_(1)
{
}

void Position::newLine() noexcept
{
    ++line_;
    char_ = 1;
}

void Position::operator++() noexcept
{
    ++char_;
}

void Position::operator--() noexcept
{
    --char_;
}

size_t Position::line() const noexcept
{
    return line_;
}

size_t Position::character() const noexcept
{
    return char_;
}

string asterius::to_string(const Position& position)
{
    return std::to_string(position.line()) + ':' + std::to_string(position.character());
}

//Data

Data::Data(DataType type, size_t size, const Position& position) noexcept
    : type_(type),
    size_(size),
    position_(position)
{
}

size_t Data::size() const noexcept
{
    return size_;
}

const Position& Data::position() const noexcept
{
    return position_;
}

void Data::setOffset(size_t offset) noexcept
{
    offset_ = offset;
}

//Token
Token::Token()
{
}

Token::Token(ElementType id, const Position& position, std::string&& name)
    : id_(id),
    position_(position),
    name_(std::move(name))
{
}

Token::Token(ElementType id, const Position& position, const std::string& name)
    : id_(id),
    position_(position),
    name_(name)
{
}

const string& Token::getName() const noexcept
{
    return name_;
}

ElementType Token::getType() const noexcept
{
    return id_;
}

const Position& Token::getPosition() const noexcept
{
    return position_;
}

bool asterius::operator<(const Token& lhs, const Token& rhs)
{
    return lhs.name_ < rhs.name_;
}