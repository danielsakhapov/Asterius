#include "Syntax.h"

namespace asterius
{

Parser::Parser(Lexer&& lexer)
	: lexer_(std::move(lexer))
{
	stack_.push(ElementType::FUNC);
	table_.insert( 
		std::make_pair( 
			ElementType::FUNC,
			std::vector<TransitionRule>{
				TransitionRule( ElementType::FN, { ElementType::SUB_FUNC } ),
				TransitionRule( ElementType::LET, { ElementType::NAME, ElementType::BE } )
			}
		)		
	);
	table_.insert( 
		std::make_pair( 
			ElementType::SUB_FUNC, 
			std::vector<TransitionRule>{
				TransitionRule( ElementType::NAME, {  } ),
			}
		)		
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
	if ( !isTerminal(stack_.top()) ) {
		try {
			auto rules = table_.at( stack_.top() );
			for (auto it: rules) {
				if (it.term_ == tokenToElement(token)) {
					stack_.pop();
					for (auto it2: it.vec_) {
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
	if (element == ElementType::NAME)
		return true;

	return false;
}

}