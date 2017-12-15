#ifndef RPN_
#define RPN_

#include <stack>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <algorithm>

#include "TypeInfo.h"
#include "RPNStructures.h"
#include "SyntaxStructures.h"

namespace asterius
{

class Command;

class RPN
{
public:
	void addCommand(Command* cmd);
	void addOperand(const Variable& variable);
	void createOperand(Variable& variable);
	void* getOperandData(const Variable& variable) const noexcept;
	Variable getNextOperand();
	std::size_t getCommandStackSize(); //will be deleted soon
	void execute();
private:
	std::vector<std::unique_ptr<Command> > commands_;
	std::vector<std::pair<Variable, bool> > stack_; //bool flag is true for temp variables
};

class Command
{
public:
	explicit Command(const std::string& name);
	virtual void execute(RPN& rpn) = 0;
	const std::string& name() const noexcept;
private:
	std::string name_;
};


//??????????????????????????
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