#ifndef RPN_
#define RPN_

#include <map>
#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "RPNStructures.h"

namespace asterius
{

class RPN
{
public:
	RPN(const std::vector<Command>& commands);
	void addCommand(Command&& command);
	Command& getCommandRevRef(size_t);
	Variable execute();
private:
	std::vector<Command> commands_;
	std::map<std::string, std::stack<std::size_t>> vars_;
	std::stack<std::vector<std::string>> locals_;
};

}

#endif