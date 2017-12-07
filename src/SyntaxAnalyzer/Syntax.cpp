#include "Syntax.h"

namespace asterius
{

Parser::Parser(Lexer&& lexer)
	: lexer_(std::move(lexer)),
    table_ {
        {ElementType::FUNC,{ //example
                TransitionRule(ElementType::FN,{ ElementType::SUB_FUNC }),
                TransitionRule(ElementType::LET,{ ElementType::NAME, ElementType::BE })
            } 
        }
    }
{
	stack_.push(ElementType::FUNC);
	table_.emplace(ElementType::FUNC,
			std::move(std::vector<TransitionRule>{
				TransitionRule( ElementType::FN, { ElementType::SUB_FUNC } ),
				TransitionRule( ElementType::LET, { ElementType::NAME, ElementType::BE } )
			})
	);
	table_.emplace( 
		ElementType::SUB_FUNC, 
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::NAME, {  } ),
		})
	);
}

void Parser::generate()
{
	while (!lexer_.eof()) {
		try {
			Token token = { TokenType::FN };
			//Token token = lexer_.getNextToken();
			#ifdef SYNTAX_DEBUG
			std::cout << token.getName() << std::endl;
			#endif
			transit(token);			
		}
		catch (const std::exception& ex) {
			std::cerr << ex.what() << " ";
		}
	}
}

void Parser::transit(const Token& token)
{
	if (!isTerminal(stack_.top())) {
		try {
			const auto& rules = table_.at(stack_.top());
			for (const auto& it: rules) {
				if (it.term_ == tokenToElement(token)) {
					stack_.pop();
					for (const auto& it2: it.vec_) {
						stack_.push(it2);
					}
					break;
				}
			}
		}
		catch (...) {
			#ifdef SYNTAX_DEBUG
			std::cerr << "wow";
			#endif
		}
	}
}


ElementType Parser::tokenToElement(const Token& token) const noexcept
{
	if (token.getType() == TokenType::FN)
		return ElementType::FN;

	if (token.getType() == TokenType::ID)
		return ElementType::NAME;
}


bool Parser::isTerminal(const ElementType& element) const noexcept
{
	return element == ElementType::NAME;
}

}