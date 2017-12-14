#ifndef RPN_STRUCTURES
#define RPN_STRUCTURES

#include "LexicalAnalyzer/TypeInfo.h"

namespace asterius
{

enum class ActionType
{
	INT,
	BYTE,
	STRING,
	FN,
	VAR,
	ADD,
	SUB,
	MULT,
	DIV,
	INDEX,
	EMPTY,
	ASSIGN,
	DOUBLE,
	BLOCK_END,
	INT_CONST,
	BLOCK_BEGIN
};

struct Action
{
	ActionType type_;
	void* data_;
};

struct Variable
{
	Variable(DataType, void*);
	DataType type_;
	void* data_;
};

class Command
{

};

}

#endif