#ifndef RPN_
#define RPN_

#include <map>
#include <vector>
#include <string>

#include "RPNStructures.h"

namespace asterius
{

class RPN
{
public:
	RPN(const std::map<std::string, Variable>& globalVars, const std::map<std::string, Variable>& localVars, const std::vector<Command>& commands);
	void addCommand(const Command& command);
	Variable execute();
private:
	std::vector<Command> commands_;
	std::map<std::string, Variable> localVars_;
	std::map<std::string, Variable> globalVars_;
};

}

#endif