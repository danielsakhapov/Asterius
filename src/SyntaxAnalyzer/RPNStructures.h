#ifndef RPN_STRUCTURES
#define RPN_STRUCTURES

#include "LexicalAnalyzer/TypeInfo.h"

namespace asterius
{

enum class CommandType
{
	INT,
	FN,
	VAR,
	ADD,
	SUB,
	MULT,
	DIV,
	ASSIGN,
	DOUBLE,
	BLOCK_END,
	INT_CONST,
	BLOCK_BEGIN
};

struct Command
{
	CommandType type_;
	void* data_;
};

struct Variable
{
	Variable(DataType, void*);
	DataType type_;
	void* data_;
};

}

#endif