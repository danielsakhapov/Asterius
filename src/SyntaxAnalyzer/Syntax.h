#ifndef SYNTAX
#define SYNTAX

#include <map>
#include <stack>
#include <deque>
#include <vector>
#include <string>
#include <utility>
#include <iostream>

#include "RPN.h"
#include "LexicalAnalyzer/SymbolTable.h"
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
    std::deque<ActionType> actionsStack_;
    std::deque<ElementType> elementsStack_;
    SymbolTable symbol_table_;
    std::map<ElementType, std::vector<TransitionRule> > table_;

    //temp variables
    std::string name_;

    void transit(ElementType elementType, const Token& token);
	void match(ElementType, const Token& token) const;
    bool isTerminal(ElementType elementType) const noexcept;
    template <typename E>
    constexpr auto toUnderlying(E e) const noexcept;
};

}
#endif // !SYNTAX