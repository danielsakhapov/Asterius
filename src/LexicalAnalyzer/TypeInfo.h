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

#define INDEX_OF_FIRST_TERMINAL 34

enum class ElementType
{
    // NonTerminals
	START,	
	NEXT_PARAM,	
	FUNC,
	SUB_FUNC,
	ARGS,
	BLOCK,
	TYPE_DESC,
	ZARR,
	NEXT_ARG,
	TYPE,
	STATEMENT,
	EXPR,
	DESC,
	INDEX,
	PARAM,
	MULT_EXPR,
	ADD_EXPR,
	COMP_EXPR,
	AND_EXPR,
	OR_EXPR,
	NEG,	
	LEXPR,
	OR_TERM,
	AND_FACTOR,
	LCOMP_EXPR,
	TERM,
	FACTOR,
	Z,
	TYPEDEF,
	VALUE,
    ELSEST,
    EMPTY,
	FINISH,
    ASS,
    // Terminals
    MAIN,
    LET,
	BE,
	BY,
	FN,
    OF,
    ARRAY,
    INT,
	DOUBLE,
	BYTE,
    STRING,
    INT_CONST,
	DOUBLE_CONST,
	BYTE_CONST,
    STRING_CONST,
    NAME,
    WHILE,
    IF,
    ELSE,
    ASSIGN,
    PLUS,
    MINUS,
    PRODUCT,
    DIVISION,
    OR,
    AND,
    NOT,
    LESS,
    GREATER,
    EQ,
    LEQ,
    GEQ,
    NEQ,
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
    Token(ElementType id = ElementType::NONE, std::string&& name = std::string(), Data data = Data());
    Token(ElementType id, const std::string& name, Data data = Data());
    const std::string& getName() const noexcept;
    ElementType getType() const noexcept;

    static Token none;
private:
    ElementType id_;
    std::string name_;
    Data data_;
    friend bool operator<(const Token& lhs, const Token& rhs);
};

bool operator<(const Token& lhs, const Token& rhs);

}
#endif // !TYPE_INFO


