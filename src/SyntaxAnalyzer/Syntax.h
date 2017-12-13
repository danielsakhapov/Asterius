#ifndef SYNTAX
#define SYNTAX

#include <map>
#include <stack>
#include <vector>
#include <utility>
#include <iostream>

#include "RPN.h"
#include "LexicalAnalyzer/Lexer.h"

namespace asterius
{

class Parser final
{
public:
	Parser(Lexer&& lexer);
	RPN generate();
private:
	Lexer lexer_;
	std::stack<Action> actionsStack_;
	std::stack<std::string> nameStack_;
	std::stack<ElementType> elementsStack_;
	std::map<ElementType, std::vector<TransitionRule>> table_;

	void transit(const Token& token);
	bool isTerminal(const Token& token);
	template <typename E>
	constexpr auto toUnderlying(E e) noexcept;
};

}
#endif // !SYNTAX