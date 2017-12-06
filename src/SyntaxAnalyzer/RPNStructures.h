#ifndef RPN_STRUCTURES
#define RPN_STRUCTURES

#include "TypeInfo.h"

namespace asterius
{

enum class CommandType
{
	INT,
	INT1,
	VAR,
	ADD,
	SUB,
	MULT,
	DIV,
	ASSIGN
};

struct Command
{
	CommandType type_;
	void* data;
};

struct Variable
{
	Variable(DataType, void*);
	DataType type_;
	void* data_;
};

}

#endif