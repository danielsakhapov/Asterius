#include "Syntax.h"

namespace asterius
{

template <typename E>
constexpr auto Parser::toUnderlying(E e) const noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}

Parser::Parser(Lexer&& lexer)
	: lexer_(std::move(lexer))
{
	elementsStack_.push(ElementType::FUNC);
	actionsStack_.push({ ActionType::EMPTY, nullptr });
	Action emptyAct = { ActionType::EMPTY, nullptr };

	table_.emplace(
		ElementType::FUNC,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::FN, ElementType::SUB_FUNC },
				{ emptyAct, emptyAct } 
			},
			{ 
				{ ElementType::LET, ElementType::NAME, ElementType::BE, ElementType::TYPEDEF, ElementType::VALUE, ElementType::STATEMENT_END, ElementType::FUNC },
				{ emptyAct, emptyAct, emptyAct, emptyAct, emptyAct, emptyAct, emptyAct } 
			} 
		})
	);
	table_.emplace(
		ElementType::SUB_FUNC,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::NAME, ElementType::OPEN_BRACKET, ElementType::ARGS, ElementType::CLOSE_BRACKET, ElementType::BLOCK, ElementType::FUNC }, 
				{} 
			},
			{ 
				{ ElementType::MAIN, ElementType::OPEN_BRACKET, ElementType::ARGS, ElementType::CLOSE_BRACKET, ElementType::BLOCK },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::ARGS,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::LET, ElementType::NAME, ElementType::BE, ElementType::TYPEDEF, ElementType::VALUE, ElementType::NEXT_ARG }, 
				{} 
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::NEXT_ARG,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::COMMA, ElementType::LET, ElementType::NAME, ElementType::BE, ElementType::TYPEDEF, ElementType::VALUE, ElementType::NEXT_ARG }, 
				{} 
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::TYPEDEF, 
		std::vector<TransitionRule>({
			{ 
				{ ElementType::INT,  }, 
				{ { ActionType::INT, new std::string } } 
			},
			{ 
				{ ElementType::DOUBLE,  }, 
				{} 
			},
			{ 
				{ ElementType::BYTE,  }, 
				{}
			},
			{ 
				{ ElementType::STRING,  },
				{}
			}		 
		})
	);
	table_.emplace(
		ElementType::VALUE, 
		std::vector<TransitionRule>({
			{ 
				{ ElementType::INT_CONST,  }, 
				{ { ActionType::INT_CONST, new int } }
			},
			{ 
				{ ElementType::DOUBLE_CONST,  },
				{} 
			},
			{ 
				{ ElementType::BYTE_CONST,  }, 
				{} 
			},
			{ 
				{ ElementType::STRING_CONST,  },
				{}
			},
			{ 
				{ ElementType::ARRAY, ElementType::OF, ElementType::INT_CONST, ElementType::ZARR },
				{}
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			}
		})
	);
	table_.emplace(
		ElementType::BLOCK,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::OPEN_FIGURE, ElementType::STATEMENT, ElementType::CLOSE_FIGURE },
				{}
			}
		})
	);
	table_.emplace(
		ElementType::STATEMENT,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::WHILE, ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::BLOCK, ElementType::STATEMENT },
				{}
			},
			{
				{ ElementType::IF, ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::BLOCK, ElementType::ELSEST },
				{}
			},
			{ 
				{ ElementType::READ, ElementType::OPEN_BRACKET, ElementType::NAME, ElementType::DESC, ElementType::CLOSE_BRACKET, ElementType::STATEMENT_END, ElementType::STATEMENT }, 
				{}
			},
			{ 
				{ ElementType::WRITE, ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::STATEMENT_END, ElementType::STATEMENT }, 
				{}
			},
			{ 
				{ ElementType::NAME, ElementType::DESC, ElementType::ASS, ElementType::STATEMENT_END, ElementType::STATEMENT },
				{}
			},
			{ 
				{ ElementType::LET, ElementType::NAME, ElementType::BE, ElementType::TYPEDEF, ElementType::VALUE, ElementType::STATEMENT_END, ElementType::STATEMENT },
				{}
			},
			{ 
				{ ElementType::EMPTY },
				{}
			} 
		})
	);
	table_.emplace(
		ElementType::ASS,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::ASSIGN, ElementType::EXPR, ElementType::STATEMENT_END, ElementType::STATEMENT },
				{} 
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			}
		})
	);
	table_.emplace(
		ElementType::ELSEST,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::ELSE, ElementType::BLOCK, ElementType::STATEMENT },
				{}
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::DESC,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::OPEN_SQUARE, ElementType::EXPR, ElementType::CLOSE_SQUARE, ElementType::INDEX },
				{}
			},
			{ 
				{ ElementType::OPEN_BRACKET, ElementType::PARAM, ElementType::CLOSE_BRACKET },
				{}
			},
			{ 
				{ ElementType::EMPTY },
				{}
			} 
		})
	);
	table_.emplace(
		ElementType::INDEX,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::OPEN_SQUARE, ElementType::EXPR, ElementType::CLOSE_SQUARE, ElementType::INDEX },
				{}
			},
			{
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::PARAM,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::NAME, ElementType::DESC, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM },
				{}
			},
			{ 
				{ ElementType::MINUS, ElementType::NEG, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM },
				{}
			},
			{ 
				{ ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM },
				{}
			},
			{ 
				{ ElementType::INT_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM }, 
				{}
			},
			{ 
				{ ElementType::DOUBLE_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM }, 
				{} 
			},
			{ 
				{ ElementType::BYTE_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM }, 
				{} 
			},
			{ 
				{ ElementType::NOT, ElementType::EXPR, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM }, 
				{} 
			},
			{
				{ ElementType::STRING_CONST, ElementType::NEXT_PARAM }, 
				{} 
			},
			{ 
				{ ElementType::EMPTY },
				{}
			}
		})
	);
	table_.emplace(
		ElementType::NEXT_PARAM,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::NAME, ElementType::COMMA, ElementType::EXPR, ElementType::NEXT_PARAM }, 
				{} 
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::EXPR,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::NAME, ElementType::DESC, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::MINUS, ElementType::NEG, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::INT_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::DOUBLE_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::BYTE_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::NOT, ElementType::EXPR, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::STRING_CONST },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::COMP_EXPR,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::LESS, ElementType::LEXPR }, 
				{}
			},
			{ 
				{ ElementType::GREATER, ElementType::LEXPR }, 
				{} 
			},
			{ 
				{ ElementType::EQ, ElementType::LEXPR },
				{} 
			},
			{ 
				{ ElementType::NEQ, ElementType::LEXPR }, 
				{} 
			},
			{ 
				{ ElementType::GEQ, ElementType::LEXPR }, 
				{} 
			},
			{ 
				{ ElementType::LEQ, ElementType::LEXPR }, 
				{} 
			},
			{
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::LEXPR,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::NAME, ElementType::DESC, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::MINUS, ElementType::NEG, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR },
				{} 
			},
			{ 
				{ ElementType::INT_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::DOUBLE_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{}
			},
			{ 
				{ ElementType::BYTE_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::NOT, ElementType::EXPR, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::STRING_CONST },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::OR_EXPR,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::OR, ElementType::OR_TERM, ElementType::OR_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::OR_TERM,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::AND_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::NAME, ElementType::DESC, ElementType::AND_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::NOT, ElementType::NEG, ElementType::AND_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::NOT, ElementType::NEG, ElementType::AND_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::INT_CONST, ElementType::AND_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::DOUBLE_CONST, ElementType::AND_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::BYTE_CONST, ElementType::AND_EXPR },
				{}
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::AND_EXPR,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::AND, ElementType::AND_FACTOR, ElementType::AND_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::AND_FACTOR,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::LCOMP_EXPR },
				{} 
			},
			{ 
				{ ElementType::NAME, ElementType::DESC, ElementType::LCOMP_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::NOT, ElementType::AND_FACTOR, ElementType::LCOMP_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::INT_CONST, ElementType::LCOMP_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::DOUBLE_CONST, ElementType::LCOMP_EXPR },
				{} 
			},
			{ 
				{ ElementType::BYTE_CONST, ElementType::LCOMP_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::LCOMP_EXPR,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::LESS, ElementType::LEXPR }, 
				{} 
			},
			{ 
				{ ElementType::GREATER, ElementType::LEXPR },
				{} 
			},
			{ 
				{ ElementType::EQ, ElementType::LEXPR }, 
				{} 
			},
			{ 
				{ ElementType::NEQ, ElementType::LEXPR }, 
				{}
			},
			{ 
				{ ElementType::GEQ, ElementType::LEXPR }, 
				{} 
			},
			{ 
				{ ElementType::LEQ, ElementType::LEXPR }, 
				{} 
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::ADD_EXPR,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::PLUS, ElementType::TERM, ElementType::ADD_EXPR },
				{} 
			},
			{ 
				{ ElementType::MINUS, ElementType::TERM, ElementType::ADD_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::EMPTY },
				{}
			} 
		})
	);
	table_.emplace(
		ElementType::TERM,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::MULT_EXPR }, 
				{}
			},
			{ 
				{ ElementType::NAME, ElementType::DESC, ElementType::MULT_EXPR }, 
				{}
			},
			{ 
				{ ElementType::MINUS, ElementType::NEG, ElementType::MULT_EXPR },
				{} 
			},
			{ 
				{ ElementType::INT_CONST, ElementType::MULT_EXPR }, 
				{}
			},
			{ 
				{ ElementType::DOUBLE_CONST, ElementType::MULT_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::BYTE_CONST, ElementType::MULT_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::MULT_EXPR,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::PRODUCT, ElementType::FACTOR, ElementType::MULT_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::DIVISION, ElementType::FACTOR, ElementType::MULT_EXPR }, 
				{} 
			},
			{ 
				{ ElementType::EMPTY },
				{}
			}
		})
	);
	table_.emplace(
		ElementType::FACTOR,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET }, 
				{} 
			},
			{ 
				{ ElementType::NAME, ElementType::DESC }, 
				{} 
			},
			{ 
				{ ElementType::MINUS, ElementType::NEG, ElementType::Z }, 
				{}
			},
			{ 
				{ ElementType::INT_CONST, ElementType::DESC }, 
				{} 
			},
			{ 
				{ ElementType::DOUBLE_CONST, ElementType::DESC },
				{} 
			},
			{ 
				{ ElementType::BYTE_CONST, ElementType::DESC }, 
				{}
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			}
		})
	);
	table_.emplace(
		ElementType::NEG,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET }, 
				{}
			},
			{ 
				{ ElementType::NAME, ElementType::DESC }, 
				{} 
			},
			{ 
				{ ElementType::INT_CONST, ElementType::DESC }, 
				{} 
			},
			{ 
				{ ElementType::DOUBLE_CONST, ElementType::DESC }, 
				{}
			},
			{ 
				{ ElementType::BYTE_CONST, ElementType::DESC },
				{} 
			}
		})
	);
	table_.emplace(
		ElementType::Z,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	table_.emplace(
		ElementType::ZARR,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::BY, ElementType::INT_CONST, ElementType::ZARR }, 
				{} 
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	
}



RPN Parser::generate()
{
	RPN rpn({}, {});	
	Token token = lexer_.getNextToken();	
	while (!lexer_.eof()) {
		try {			
			if (!isTerminal(elementsStack_.top())) {
				transit(token);
			}
			else {
				if (token.getType() == ElementType::NAME)
					nameStack_.push(token.getName());
				elementsStack_.pop();
				token = lexer_.getNextToken();
			}
			if (!actionsStack_.empty()) {
				rpn.addCommand(actionsStack_.top());
				actionsStack_.pop();
			}
			else
				throw std::logic_error("there is smth bad with your stack, sir!");
		}
		catch (const std::exception& ex) {
			std::cerr << ex.what() << " ";
			throw;
		}
	}
}



void Parser::transit(const Token& token)
{
	if (elementsStack_.top() == ElementType::EMPTY) {
		elementsStack_.pop();
		return;
	}
	const auto& rules = table_.at(elementsStack_.top());
	for (const auto& it: rules) {
		if (token.getType() == it.elements_[0] || it.elements_[0] == ElementType::EMPTY) {
			elementsStack_.pop();		
			for (auto it2 = it.elements_.rbegin(); it2 != it.elements_.rend(); ++it2) {
				elementsStack_.push(*it2);
			}
			for (auto it2 = it.acts_.rbegin(); it2 != it.acts_.rend(); ++it2) {
				if (
					it2->type_ == ActionType::INT || it2->type_ == ActionType::DOUBLE ||
					it2->type_ == ActionType::BYTE || it2->type_ == ActionType::STRING ||
					it2->type_ == ActionType::VAR
				) {
					*((std::string*)it2->data_) = nameStack_.top();
					nameStack_.pop();
				}
				actionsStack_.push(*it2);
			}
			return;
		}				
	}
	throw std::logic_error("wow! l00kz like 1'm c0nfuzzzed");
}



bool Parser::isTerminal(const Token& token)
{
	return toUnderlying(token.getType()) >= INDEX_OF_FIRST_TERMINAL; // l00kz l1ke maGGG1ck
}

}