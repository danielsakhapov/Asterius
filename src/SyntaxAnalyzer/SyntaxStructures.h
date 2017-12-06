#ifndef SYNTAX_STRUCTURES
#define SYNTAX_STRUCTURES

#include "Lexer.h"

namespace asterius
{

enum class ElementType
{
	START,
	NAME,
	LET,
	BE,
	FN,
	INT,
	DOUBLE,
	CHAR,
	STRING,
	ARRAY,
	INT_CONST,
	DOUBLE_CONST,
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
	EMPTY,
	FINISH
};

struct TransitionRule
{
	TransitionRule(ElementType&&, std::vector<ElementType>&&);
	ElementType term_;
	std::vector<ElementType> vec_;
};

}

#endif // SYNTAX_STRUCTURES