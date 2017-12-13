#ifndef SYNTAX_STRUCTURES
#define SYNTAX_STRUCTURES

#include <vector>

#include "RPNStructures.h"

namespace asterius
{

struct TransitionRule
{
	TransitionRule(std::vector<ElementType>&&, std::vector<Action>&&);
	std::vector<ElementType> elements_;
	std::vector<Action> acts_;
};

}

#endif