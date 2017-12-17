#ifndef RPN_STRUCTURES
#define RPN_STRUCTURES

#include "LexicalAnalyzer/TypeInfo.h"

namespace asterius
{

enum class ActionType
{
    OR,
    EQ,
    LEQ,
    GEQ,
    NEQ,
    VAR,
    ADD,
    SUB,
    AND,
    NOT,
    DIV,
    INT,
    MULT,
    BYTE,
    NAME,
    MAIN,
    READ,
    LESS,
    PLUS,
    MINUS,
    ARRAY,
    INDEX,
    EMPTY,
    WRITE,
    UMINUS,
    ASSIGN,
    STRING,
    DOUBLE,
    IF_END,
    PRODUCT,
    GREATER,
    IF_BEGIN,
    ELSE_END,
    DIVISION,
    BLOCK_END,
    INT_CONST,
    FN_CREATE,
    WHILE_END,
    ELSE_START,
    VAR_CREATE,
    BYTE_CONST,
    ZERO_CONST,
    PARAMS_END,
    WHILE_BEGIN,
    BLOCK_BEGIN,    
    PARAMS_BEGIN,
    DOUBLE_CONST,
    STRING_CONST,
    FUNCTION_CALL,
    CONDITION_END,
    CONDITION_BEGIN,
    ARRAY_DEMENSION
};

struct Action
{
    ActionType type_;
    void* data_;
};

}

#endif