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
    AND,
    NOT,    
    INT,
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
	ASS_INT_CONST,
    CONDITION_END,
	ASS_BYTE_CONST,
    CONDITION_BEGIN,
    ARRAY_DEMENSION,
	ASS_STRING_CONST,
	ASS_DOUBLE_CONST,
};

struct Action
{
    ActionType type_;
    void* data_;
};

}

#endif