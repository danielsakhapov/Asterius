#ifndef SYNTAX
#define SYNTAX

#include <map>
#include <stack>
#include <vector>
#include <utility>
#include <iostream>

#include "RPN.h"
#include "RPNStructures.h"
#include "SyntaxStructures.h"
#include "LexicalAnalyzer/Lexer.h"

namespace asterius
{

class Parser final
{
public:
	Parser(Lexer&& lexer);
	RPN generate();
	bool isTerminal(const ElementType&) const noexcept;
	void transit(const Token&);
	ElementType tokenToElement(const Token&) const noexcept;
private:
	Lexer lexer_;
	std::stack<ElementType> stack_;
	std::map<ElementType, std::vector<TransitionRule> > table_;
};

}
#endif // !SYNTAX