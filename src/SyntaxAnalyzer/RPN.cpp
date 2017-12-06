#include "RPN.h"

namespace asterius
{

RPN::RPN(const std::map<std::string, Variable>& globalVars, const std::map<std::string, Variable>& localVars, const std::vector<Command>& commands) :
	globalVars_(globalVars), localVars_(localVars), commands_(commands)
{}

void RPN::addCommand(const Command& cmd)
{
	commands_.push_back(cmd);
}

Variable RPN::execute()
{
	return Variable( DataType::INT, new int(0) );
}

}