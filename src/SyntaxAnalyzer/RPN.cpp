#include "RPN.h"

namespace asterius
{

void RPN::addCommand(Command* cmd)
{
	commands_.emplace_back(cmd);
}

std::size_t RPN::getCommandStackSize()
{
	return commands_.size();
}

void RPN::execute()
{
	auto sz = commands_.size();
	for (size_t next = 0; next < sz; ++next) {
		commands_[next]->execute(*this);
	}
}

void RPN::addOperand(const Variable& variable)
{
	stack_.emplace_back(variable, false);
}

void RPN::createOperand(Variable& variable)
{
	//allocate variable on stack;
	stack_.emplace_back(variable, true);
}

void* RPN::getOperandData(const Variable& variable) const noexcept
{
	//call stack method
	return nullptr;
}

Variable RPN::getNextOperand()
{
	//remove from stack if stack_.back().second is true
	Variable tmp = stack_.back().first;
	stack_.pop_back();
	return tmp;
}


//Commands
Command::Command(const std::string& name)
	: name_(name)
{
}

const std::string& Command::name() const noexcept
{
	return name_;
}


//??????????????????????????
IntCommand::IntCommand(void* data) :
	Command("a")
{}

DoubleCommand::DoubleCommand(void* data) :
	Command("a")
{}

ByteCommand::ByteCommand(void* data) :
	Command("a")
{}

StringCommand::StringCommand(void* data) :
	Command("a")
{}

IntConstCommand::IntConstCommand(void* data) :
	Command("a")
{}

DoubleConstCommand::DoubleConstCommand(void* data) :
	Command("a")
{}

ByteConstCommand::ByteConstCommand(void* data) :
	Command("a")
{}

StringConstCommand::StringConstCommand(void* data) :
	Command("a")
{}

ZeroConstCommand::ZeroConstCommand(void* data) :
	Command("a")
{}

NameCommand::NameCommand(void* data) :
	Command("a")
{}

}