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
#include "Common/Stack.h"
#include "Common/BasicStructures.h"

namespace asterius
{

class Command;

class RPN
{
public:
	void addCommand(std::unique_ptr<Command> cmd);
	void addOperand(const Variable& variable); //add existing variable to operands
	void createOperand(Variable& variable, void* src); // create temp variable and add to operands
	void createVariable(const Variable& variable); //allocates variable on stack
	void* getOperandData(const Variable& variable) const noexcept;
	Variable getNextOperand();
	void setCommand(size_t position) noexcept;
	void print() const;
	void execute();
private:
	Stack<12800> stack_; //stack to hold data
	std::vector<std::unique_ptr<Command> > commands_; //RPN
	size_t next_; //instruction pointer
	std::vector<std::pair<Variable, bool> > operands_; //bool flag is true for temp variables
};

class Command
{
public:
	explicit Command(const std::string& name);
	const std::string& name() const noexcept;
	virtual void execute(RPN& rpn) = 0;
private:
	std::string name_;
};

class WriteCommand : public Command
{
public:
	WriteCommand();
	void execute(RPN& rpn) override;
};

class ReadCommand : public Command
{
public:
	ReadCommand();
	void execute(RPN& rpn) override;
};

class OperandCommand : public Command
{
public:
	OperandCommand(const Variable& variable, const std::string& name);
	void execute(RPN& rpn) override;
private:
	Variable variable_;
};

class CreateVariableCommand : public Command
{
public:
	CreateVariableCommand(const Variable& variable, const std::string& name);
	void execute(RPN& rpn) override;
private:
	Variable variable_;
};

class AddCommand : public Command
{
public:
	AddCommand();
	void execute(RPN& rpn) override;
};

class SubtractCommand : public Command
{
public:
	SubtractCommand();
	void execute(RPN& rpn) override;
};

class MultiplyCommand : public Command
{
public:
	MultiplyCommand();
	void execute(RPN& rpn) override;
};

class DivideCommand : public Command
{
public:
	DivideCommand();
	void execute(RPN& rpn) override;
};

class NegateCommand : public Command
{
public:
	NegateCommand();
	void execute(RPN& rpn) override;
};

class JumpCommand : public Command
{
public:
	JumpCommand();
	void execute(RPN& rpn) override;
};

class JumpIfNotCommand : public Command
{
public:
	JumpIfNotCommand();
	void execute(RPN& rpn) override;
};

class EqualCommand : public Command
{
public:
	EqualCommand();
	void execute(RPN& rpn) override;
};

class NotEqualCommand : public Command
{
public:
	NotEqualCommand();
	void execute(RPN& rpn) override;
};

class LessCommand : public Command
{
public:
	LessCommand();
	void execute(RPN& rpn) override;
};

class GreaterCommand : public Command
{
public:
	GreaterCommand();
	void execute(RPN& rpn) override;
};

class LessOrEqualCommand : public Command
{
public:
	LessOrEqualCommand();
	void execute(RPN& rpn) override;
};

class GreaterOrEqualCommand : public Command
{
public:
	GreaterOrEqualCommand();
	void execute(RPN& rpn) override;
};

class AndCommand : public Command
{
public:
	AndCommand();
	void execute(RPN& rpn) override;
};

class OrCommand : public Command
{
public:
	OrCommand();
	void execute(RPN& rpn) override;
};

class NotCommand : public Command
{
public:
	NotCommand();
	void execute(RPN& rpn) override;
};

class AssignCommand : public Command
{
public:
	AssignCommand();
	void execute(RPN& rpn) override;
};

template<class value_type>
class DataCommand : public Command
{
public:
	DataCommand(const Variable& variable, value_type data)
		: Command("create constant"),
		variable_(variable),
		data_(data)
	{
	}

	void execute(RPN& rpn) override 
	{
		rpn.createOperand(variable_, static_cast<void*>(&data_));
	};
private:
	value_type data_;
	Variable variable_;
};

//HELPER FUNCTIONS
template<class value_type>
value_type get_val(void* ptr);

template<class operation>
void perform(RPN& rpn, operation op);

template<class operation>
void performCompare(RPN& rpn, operation op);

}

#endif