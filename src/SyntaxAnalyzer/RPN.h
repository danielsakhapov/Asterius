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
	void addCommand(std::unique_ptr<Command> cmd, size_t position);
	void addOperand(const Variable& variable); //add existing variable to operands
	void createOperand(Variable& variable, void* src); // create temp variable and add to operands
	void createOperand(Variable&& variable, void* src);
	void createVariable(const Variable& variable); //allocates variable on stack
	void createVariable(const Variable& variable, void* src); //allocates variable on stack and set memory
	std::pair<Variable, void*> getNextOperand();
	void setCommand(size_t position) noexcept;
    size_t getCommand() const noexcept;
	void print() const;
	size_t getSize();
	void execute();
	void beginBlock();
	void endBlock();
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

template<class type>
class CreateVariableCommand : public Command
{
public:
	CreateVariableCommand(const Variable& variable, const std::string& name)
		: Command("create variable " + name),
		variable_(variable)
	{
	}

	void execute(RPN& rpn) override
	{
	    rpn.createVariable(variable_);
	}
private:
	Variable variable_;
};

template<>
class CreateVariableCommand<array_passport> : public Command
{
public:
	CreateVariableCommand(ElementType type, const std::vector<size_t>& dims, const std::string& name)
		: Command("create variable " + name),
		type_(type),
		dims_(dims)
	{
	}

	void execute(RPN& rpn) override
	{
		precalc();
		create_variable(rpn, ARRAY_SIZE, dims_[0], dims_.size() == 1);
		_execute(rpn, 0);
	}
private:
	void _execute(RPN& rpn, size_t level)
	{
		if (level == dims_.size() - 1) { // final dimension
			for (size_t i = 0; i < dims_[level]; ++i) {
				Variable variable(get_data_type(type_), get_element_size(type_));
				rpn.createVariable(variable);
			}
		}
		else {
			for (int i = 0, offset = ARRAY_SIZE * dims_[level]; i < (int)dims_[level]; offset += sizes_[level] - ARRAY_SIZE, ++i) {
				create_variable(rpn, offset, dims_[level + 1], level == dims_.size() - 2);
			}
			for (size_t i = 0; i < dims_[level]; ++i) {
				_execute(rpn, level + 1);
			}
		}
	}

	void precalc()
	{
		if (dims_.size() == 1) 
			return;

		sizes_.resize(dims_.size() - 1);
		sizes_.back() = get_element_size(type_) * dims_.back();
		for (int i = sizes_.size() - 2; i >= 0; --i) {
			sizes_[i] = (sizes_[i + 1] + ARRAY_SIZE) * dims_[i + 1];
		}
	}

	void create_variable(RPN& rpn, size_t offset, size_t size, bool lastDim)
	{
		array_passport passport;
		passport.element_type_ = lastDim ? get_data_type(type_) : DataType::ARRAY;
		passport.block_offset_ = offset;
		passport.element_size_ = lastDim ? get_element_size(type_) : ARRAY_SIZE;
		passport.size_ = size;
		rpn.createVariable(make_variable<array_passport>(), &passport);
	}

	std::vector<int> sizes_;
	size_t offset_;
	ElementType type_;
	std::vector<size_t> dims_;
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

class IndexCommand : public Command
{
public:
	IndexCommand();
	void execute(RPN& rpn) override;
};

template<class value_type>
class DataCommand : public Command
{
public:
	DataCommand(const Variable& variable, value_type data)
		: Command("create constant " + std::to_string(data)),
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

template<>
class DataCommand<std::string> : public Command
{
public:
    DataCommand(const std::string& data)
        : Command("create constant " + data),
        data_(data)
    {
    }

    void execute(RPN& rpn) override
    {
        size_t size = ARRAY_SIZE + data_.size();
        auto buf = std::make_unique<char[]>(size);
        array_passport passport;
        passport.block_offset_ = ARRAY_SIZE;
        passport.element_size_ = 1;
        passport.element_type_ = DataType::BYTE;
        passport.size_ = data_.size();
        memcpy(buf.get(), &passport, ARRAY_SIZE); //101 don't look here
        memcpy(buf.get() + ARRAY_SIZE, data_.data(), size - ARRAY_SIZE); //Lasciate ogni speranza, voi ch�entrate
        Variable var(DataType::ARRAY, size);
        rpn.createOperand(var, buf.get());
    };
private:
    std::string data_;
};

class BeginBlockCommand : public Command
{
public:
	BeginBlockCommand();
	void execute(RPN& rpn) override;
};

class EndBlockCommand : public Command
{
public:
	EndBlockCommand();
	void execute(RPN& rpn) override;
};

class SaveIPCommand : public Command {
public:
    SaveIPCommand();
    void execute(RPN& rpn);
};

class ArgumentCommand : public Command {

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