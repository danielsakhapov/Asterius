#ifndef RPN_
#define RPN_

#include <map>
#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "TypeInfo.h"
#include "RPNStructures.h"
#include "SyntaxStructures.h"

namespace asterius
{

class RPN
{
public:
	RPN(const std::vector<Command>& commands);
	void addCommand(Action&& act);
	Variable execute();
private:
	std::vector<Command> commands_;
	std::stack<std::vector<std::string>> locals_;
	std::map<std::string, std::stack<std::size_t>> vars_;
};

}

#endif