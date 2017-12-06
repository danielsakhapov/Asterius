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
	RPN(std::map<std::string, Variable>&, std::map<std::string, Variable>&, const std::vector<Command>&);
	void addCommand(const Command&);
	Variable execute();
private:
	std::vector<Command> commands_;
	std::map<std::string, Variable> localVars_;
	std::map<std::string, Variable> globalVars_;
};

}

#endif