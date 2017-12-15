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

struct Variable
{
	Variable(DataType dataType);
	Variable(const DataType&, void*);
	DataType type_;
	void* data_;
};

class Command
{
public:
	Command(void*);
	Variable execute();
private:
	void* data_;
};

class IntCommand : public Command
{
public:
	IntCommand(void*);
};

class DoubleCommand : public Command
{
public:
	DoubleCommand(void*);
};

class ByteCommand : public Command
{
public:
	ByteCommand(void*);
};

class StringCommand : public Command
{
public:
	StringCommand(void*);
};

class IntConstCommand : public Command
{
public:
	IntConstCommand(void*);
};

class DoubleConstCommand : public Command
{
public:
	DoubleConstCommand(void*);
};

class ByteConstCommand : public Command
{
public:
	ByteConstCommand(void*);
};

class StringConstCommand : public Command
{
public:
	StringConstCommand(void*);
};

class ZeroConstCommand : public Command
{
public:
	ZeroConstCommand(void*);
};

class NameCommand : public Command
{
public:
	NameCommand(void*);
};

}

#endif