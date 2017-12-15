#ifndef SYNTAX
#define SYNTAX

#include <map>
#include <stack>
#include <vector>
#include <string>
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
	RPN analyze();
	void generate(RPN& rpn, const Token&);
private:
	Lexer lexer_;
	std::stack<std::size_t> labelsStack_;
	std::stack<ActionType> actionsStack_;
	std::stack<ElementType> elementsStack_;
	std::stack<std::vector<std::string>> locals_;
	std::map<std::string, std::stack<std::size_t>> vars_;
	std::map<ElementType, std::vector<TransitionRule>> table_;

	void transit(const Token& token);
	bool isTerminal(ElementType elementType) const noexcept;
	template <typename E>
	constexpr auto toUnderlying(E e) const noexcept;
};

}
#endif // !SYNTAX