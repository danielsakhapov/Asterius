#ifndef TYPE_INFO
#define TYPE_INFO

#include <string>

namespace asterius
{

#define CHAR_SIZE 1
#define INT_SIZE 4
#define FLOAT_SIZE 8

enum class DataType
{
    CHAR,
    INT,
    FLOAT,
    ARRAY,
    FUNCTION
};

enum class TokenType
{
    LEQ,
    GEQ,
    GREATER,
    LESS,
    ELSE,
    MAIN,
    OF,
    ARRAY,
    BY,
    NOT,
    CONST,
    ID,
    BYTE,
    LET,
    BE,
    FN,
    WHILE,
    IF,
    INT,
    DOUBLE,
    STRING,
    AND,
    OR,
    EQ,
    NEQ,
    LT,
    GT,
    LE,
    GE,
    ASSIGN,
    PLUS,
    MINUS,
    PRODUCT,
    DIVISION,
    STATEMENT_END, //;
    TRUE,
    FALSE,
    READ,
    WRITE,
    OPEN_FIGURE,
    CLOSE_FIGURE,
    OPEN_SQUARE,
    CLOSE_SQUARE,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    COMMA,
    NONE,
};

struct Data
{
    Data() noexcept;
    Data(DataType type, size_t size, size_t line, size_t chr, char isRef = false) noexcept;
    DataType type_;
    char isRef_;
    void* data_;
    size_t size_;
    size_t line_;
    size_t char_;
};


class Token
{
public:
    Token(TokenType id = TokenType::NONE, std::string&& name = std::string(), Data data = Data());
    Token(TokenType id, const std::string& name, Data data = Data());
    const std::string& getName() const noexcept;
    TokenType getType() const noexcept;

    static Token none;
private:
    TokenType id_;
    std::string name_;
    Data data_;
    friend bool operator<(const Token& lhs, const Token& rhs);
};

bool operator<(const Token& lhs, const Token& rhs);

}
#endif // !TYPE_INFO


