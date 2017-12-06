#ifndef SYNTAX
#define SYNTAX

#include <map>
#include <stack>
#include <vector>
#include <utility>
#include <iostream>

#include "Lexer.h"
#include "SyntaxStructures.h"

namespace asterius
{

class Parser final
{
public:
	Parser(Lexer&& lexer);
	void generate();
	bool isTerminal(const ElementType&) const noexcept;
	void transit(const Token&);
	ElementType tokenToElement(const Token&) const noexcept;
private:
	std::map<ElementType, std::vector<TransitionRule>> table_;
	Lexer lexer_;
	std::stack<ElementType> stack_;
};

}
#endif // !SYNTAX