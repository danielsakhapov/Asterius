#include "RPN.h"

namespace asterius
{

RPN::RPN(const std::vector<Command*>& commands) :
	commands_(commands)
{}

RPN::RPN(const std::vector<Variable>& stack) :
	stack_(stack)
{}

RPN::RPN(const std::vector<Command*>& commands, const std::vector<Variable>& stack) :
	commands_(commands), stack_(stack)
{}

void RPN::addCommand(Command* cmd)
{
	commands_.push_back(cmd);
}

std::size_t RPN::getCommandStackSize()
{
	return commands_.size();
}

Variable RPN::execute()
{
	return Variable( DataType::INT, new int(0) );
}

}