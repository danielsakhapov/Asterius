#ifndef TYPE_INFO
#define TYPE_INFO

#include "BasicStructures.hpp"
#include <string>

namespace asterius
{
    enum class TokenType
    {
        TYPE,
        INT,    //const
        DOUBLE, //const
        STRING, //literal
        ARRAY,
        WHILE,
        IF,
        AND,
        OR,
        ID,
        EQ,
        TRUE,
        FALSE,
        READ,
        WRITE,
    };

    class Token
    {
    public:
        Token(TokenType id) noexcept;
    private:
        TokenType id_;
    };

    class Number : public Token
    {
    public:
        explicit Number(int val) noexcept;
    private:
        int val_;
    };

    class Word : public Token
    {
    public:
        explicit Word(const std::string& str, TokenType type = TokenType::ID);
    private:
        std::string val_;
    };

    class Type : public Word
    {
    public:
        Type(const std::string& str, TokenType type, size_t width);
        size_t width() const noexcept;
    private:
        size_t width_; // type size in bytes
    };

    class Array : public Type
    {
    public:
        Array(const Type& type, size_t size);
    private:
        Type type_;
        size_t size_;
    };

    class Double : public Token
    {
    public:
        explicit Double(double val) noexcept;
    private:
        double val_;
    };
}

#endif // !TYPE_INFO


