#ifndef TYPE_INFO
#define TYPE_INFO

#include <string>

namespace asterius
{

#define BYTE_SIZE 1
#define INT_SIZE 4
#define FLOAT_SIZE 8
#define INDEX_OF_FIRST_TERMINAL 35

template<class type>
struct type_to_variable_size;

template<>
struct type_to_variable_size<char>
{
	static constexpr int value = BYTE_SIZE;
};

template<>
struct type_to_variable_size<int>
{
	static constexpr int value = INT_SIZE;
};

template<>
struct type_to_variable_size<double>
{
	static constexpr int value = FLOAT_SIZE;
};

enum class DataType
{
    BYTE,
    INT,
    FLOAT,
    ARRAY,
    FUNCTION
};

template<class type>
struct type_to_type;

template<>
struct type_to_type<char>
{
	static constexpr DataType value = DataType::BYTE;
};

template<>
struct type_to_type<int>
{
	static constexpr DataType value = DataType::INT;
};

template<>
struct type_to_type<double>
{
	static constexpr DataType value = DataType::FLOAT;
};

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
	LTYPEDEF,
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
    NONE
};

class Position
{
public:
    Position() noexcept;
    void newLine() noexcept;
    void operator++() noexcept;
    void operator--() noexcept;
    size_t line() const noexcept;
    size_t character() const noexcept;
private:
    size_t line_;
    size_t char_;
};

std::string to_string(const Position& position);

class Variable
{
public:
    Variable(DataType type, size_t size, const Position& position = Position()) noexcept;
    size_t size() const noexcept;
    const Position& position() const noexcept;
    void setOffset(int offset) noexcept;
    int offset() const noexcept;
    size_t calc_offset(size_t block_begin) const noexcept;
    void setRelative(bool isRelative) noexcept;
    bool isRelative() const noexcept;
    DataType type() const noexcept;
private:
    DataType type_; 
    int offset_; //offset in stack
    bool isRelative_;
    size_t size_; //can be carried through global table
    Position position_; //position in source file
};

template<class type>
Variable make_variable(const Position& position = Position())
{
	return Variable(type_to_type<type>::value, type_to_variable_size<type>::value, position);
}

class Token
{
public:
    Token();
    Token(ElementType id, const Position& position, std::string&& name);
    Token(ElementType id, const Position& position, const std::string& name = std::string());
    const std::string& getName() const noexcept;
    ElementType getType() const noexcept;
    const Position& getPosition() const noexcept;
private:
    ElementType id_;
    std::string name_;
    Position position_;
};

}
#endif // !TYPE_INFO


