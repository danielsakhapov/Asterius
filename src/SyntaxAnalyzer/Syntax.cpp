#include "Syntax.h"

namespace asterius
{

template <typename E>
constexpr auto Parser::toUnderlying(E e) const noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}

RPN Parser::analyze()
{
	RPN rpn;
	Token token = lexer_.getNextToken();
	while (!lexer_.eof()) {
		try {			
			if (!isTerminal(elementsStack_.top())) {				
				transit(token);
			}
			else {
				actionsStack_.pop();
				elementsStack_.pop();
				token = lexer_.getNextToken();
			}
			if (elementsStack_.top() == ElementType::EMPTY)
				elementsStack_.pop();
			if (!actionsStack_.empty()) {
				generate(rpn, token);
			}
			else {
				throw std::logic_error("there is smth bad with your stack, sir!");
			}
		}
		catch (const std::exception& ex) {
			std::cerr << ex.what() << " ";
			throw;
		}
	}
	return rpn;
}



void Parser::generate(RPN& rpn, const Token& token)
{
	ActionType act = actionsStack_.top();
	
	if (act == ActionType::EMPTY) {
		return;
	}
	
	Command* cmd = nullptr;
	
	if (act == ActionType::INT) {
		//cmd = new IntCommand(new std::size_t);
	}

	if (act == ActionType::DOUBLE) {
		//cmd = new DoubleCommand(new std::size_t);
	}

	if (act == ActionType::BYTE) {
		//cmd = new ByteCommand(new std::size_t);
	}

	if (act == ActionType::STRING) {
		//cmd = new StringCommand(new std::size_t);
	}

	if (act == ActionType::INT_CONST) {
		//cmd = new IntConstCommand(new int(std::stoi(token.getName())));
	}

	if (act == ActionType::DOUBLE_CONST) {
		//cmd = new DoubleConstCommand(new double(std::stod(token.getName())));
	}

	if (act == ActionType::BYTE_CONST) {
		//cmd = new ByteConstCommand(new char(token.getName()[0]));
	}

	if (act == ActionType::STRING_CONST) {
		//cmd = new StringConstCommand(new const char*(token.getName().c_str()));
	}

	if (act == ActionType::ZERO_CONST) {
		//cmd = new ZeroConstCommand(new int(0));
	}

	//if (cmd)
		//rpn.addCommand(std::make_unique<Command>(cmd));
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
			actionsStack_.pop();
			for (auto it2 = it.acts_.rbegin(); it2 != it.acts_.rend(); ++it2) {				
				actionsStack_.push(*it2);
			}
			return;
		}
	}
	throw std::logic_error("wow! l00kz like 1'm c0nfuzzzed");
}



bool Parser::isTerminal(ElementType elementType) const noexcept
{
	return toUnderlying(elementType) >= INDEX_OF_FIRST_TERMINAL; // l00kz l1ke maGGG1ck
}



Parser::Parser(Lexer&& lexer)
	: lexer_(std::move(lexer))
{
	//locals_.push({});
	elementsStack_.push(ElementType::FUNC);
	actionsStack_.push(ActionType::EMPTY);

	table_.emplace(
		ElementType::FUNC,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::FN, ElementType::SUB_FUNC },
				{ ActionType::FN_CREATE, ActionType::EMPTY }
			},
			{ 
				{ ElementType::LET, ElementType::NAME, ElementType::BE, ElementType::TYPEDEF, ElementType::VALUE, ElementType::STATEMENT_END, ElementType::FUNC },
				{ ActionType::VAR_CREATE, ActionType::NAME, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
			} 
		})
	);
	table_.emplace(
		ElementType::SUB_FUNC,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::NAME, ElementType::OPEN_BRACKET, ElementType::ARGS, ElementType::CLOSE_BRACKET, ElementType::BLOCK, ElementType::FUNC }, 
				{ ActionType::NAME, ActionType::PARAMS_BEGIN, ActionType::EMPTY, ActionType::PARAMS_END, ActionType::EMPTY, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::MAIN, ElementType::OPEN_BRACKET, ElementType::ARGS, ElementType::CLOSE_BRACKET, ElementType::BLOCK },
				{ ActionType::MAIN, ActionType::PARAMS_BEGIN, ActionType::EMPTY, ActionType::PARAMS_END, ActionType::EMPTY, ActionType::EMPTY }
			} 
		})
	);
	table_.emplace(
		ElementType::ARGS,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::LET, ElementType::NAME, ElementType::BE, ElementType::TYPEDEF, ElementType::VALUE, ElementType::NEXT_ARG }, 
				{ ActionType::VAR_CREATE, ActionType::NAME, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
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
				{ ActionType::EMPTY, ActionType::VAR_CREATE, ActionType::NAME, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
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
				{ ElementType::INT }, 
				{ ActionType::INT } 
			},
			{ 
				{ ElementType::DOUBLE }, 
				{ ActionType::DOUBLE } 
			},
			{ 
				{ ElementType::BYTE }, 
				{ ActionType::BYTE }
			},
			{ 
				{ ElementType::STRING },
				{ ActionType::STRING }
			}		 
		})
	);
	table_.emplace(
		ElementType::VALUE, 
		std::vector<TransitionRule>({
			{ 
				{ ElementType::INT_CONST }, 
				{ ActionType::INT_CONST }
			},
			{ 
				{ ElementType::DOUBLE_CONST },
				{ ActionType::DOUBLE_CONST } 
			},
			{ 
				{ ElementType::BYTE_CONST }, 
				{ ActionType::BYTE_CONST } 
			},
			{ 
				{ ElementType::STRING_CONST },
				{ ActionType::STRING_CONST }
			},
			{ 
				{ ElementType::ARRAY, ElementType::OF, ElementType::INT_CONST, ElementType::ZARR },
				{ ActionType::ARRAY, ActionType::ARRAY_DEMENSION, ActionType::INT_CONST, ActionType::EMPTY }
			},
			{ 
				{ ElementType::EMPTY },
				{ ActionType::ZERO_CONST } 
			}
		})
	);
	table_.emplace(
		ElementType::BLOCK,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::OPEN_FIGURE, ElementType::STATEMENT, ElementType::CLOSE_FIGURE },
				{ ActionType::BLOCK_BEGIN, ActionType::EMPTY, ActionType::BLOCK_END }
			}
		})
	);
	table_.emplace(
		ElementType::STATEMENT,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::WHILE, ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::BLOCK, ElementType::STATEMENT },
				{ ActionType::WHILE_BEGIN, ActionType::CONDITION_BEGIN, ActionType::EMPTY, ActionType::CONDITION_END, ActionType::EMPTY, ActionType::WHILE_END }
			},
			{
				{ ElementType::IF, ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::BLOCK, ElementType::ELSEST },
				{ ActionType::IF_BEGIN, ActionType::CONDITION_BEGIN, ActionType::EMPTY, ActionType::CONDITION_END, ActionType::EMPTY, ActionType::IF_END }
			},
			{ 
				{ ElementType::READ, ElementType::OPEN_BRACKET, ElementType::NAME, ElementType::DESC, ElementType::CLOSE_BRACKET, ElementType::STATEMENT_END, ElementType::STATEMENT }, 
				{ ActionType::EMPTY, ActionType::EMPTY, ActionType::VAR, ActionType::EMPTY, ActionType::EMPTY, ActionType::READ, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::WRITE, ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::STATEMENT_END, ElementType::STATEMENT }, 
				{ ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::WRITE, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::NAME, ElementType::DESC, ElementType::ASS, ElementType::STATEMENT_END, ElementType::STATEMENT },
				{ ActionType::VAR, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::LET, ElementType::NAME, ElementType::BE, ElementType::TYPEDEF, ElementType::VALUE, ElementType::STATEMENT_END, ElementType::STATEMENT },
				{ ActionType::VAR_CREATE, ActionType::NAME, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
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
				{ ElementType::ASSIGN, ElementType::EXPR },
				{ ActionType::EMPTY, ActionType::ASSIGN } 
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
				{ ActionType::ELSE_START, ActionType::EMPTY, ActionType::ELSE_END }
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
				{ ActionType::EMPTY, ActionType::EMPTY, ActionType::INDEX, ActionType::EMPTY }
			},
			{ 
				{ ElementType::OPEN_BRACKET, ElementType::PARAM, ElementType::CLOSE_BRACKET },
				{ ActionType::EMPTY, ActionType::EMPTY, ActionType::FUNCTION_CALL }
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
				{ ActionType::EMPTY, ActionType::EMPTY, ActionType::INDEX, ActionType::EMPTY }
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
				{ ActionType::VAR, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::MINUS, ElementType::NEG, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM },
				{ ActionType::EMPTY, ActionType::UMINUS, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM },
				{ ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::INT_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM }, 
				{ ActionType::INT_CONST, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::DOUBLE_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM }, 
				{ ActionType::DOUBLE_CONST, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::BYTE_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM }, 
				{ ActionType::BYTE_CONST, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::NOT, ElementType::EXPR, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM }, 
				{ ActionType::EMPTY, ActionType::NOT, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
			},
			{
				{ ElementType::STRING_CONST, ElementType::NEXT_PARAM }, 
				{ ActionType::STRING_CONST, ActionType::EMPTY } 
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
				{ ElementType::COMMA, ElementType::EXPR, ElementType::NEXT_PARAM }, 
				{ ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
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
				{ ActionType::VAR, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::MINUS, ElementType::NEG, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{ ActionType::EMPTY, ActionType::UMINUS, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{ ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::INT_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{ ActionType::INT_CONST, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::DOUBLE_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{ ActionType::DOUBLE_CONST, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::BYTE_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{ ActionType::BYTE_CONST, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::NOT, ElementType::EXPR, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{ ActionType::EMPTY, ActionType::NOT, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::STRING_CONST },
				{ ActionType::STRING_CONST }
			} 
		})
	);
	table_.emplace(
		ElementType::COMP_EXPR,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::LESS, ElementType::LEXPR }, 
				{ ActionType::EMPTY, ActionType::LESS }
			},
			{ 
				{ ElementType::GREATER, ElementType::LEXPR }, 
				{ ActionType::EMPTY, ActionType::GREATER }
			},
			{ 
				{ ElementType::EQ, ElementType::LEXPR },
				{ ActionType::EMPTY, ActionType::EQ }
			},
			{ 
				{ ElementType::NEQ, ElementType::LEXPR }, 
				{ ActionType::EMPTY, ActionType::NEQ }
			},
			{ 
				{ ElementType::GEQ, ElementType::LEXPR }, 
				{ ActionType::EMPTY, ActionType::GEQ }
			},
			{ 
				{ ElementType::LEQ, ElementType::LEXPR }, 
				{ ActionType::EMPTY, ActionType::LEQ }
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
				{ ActionType::VAR, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::MINUS, ElementType::NEG, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{ ActionType::EMPTY, ActionType::UMINUS, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{ ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::INT_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{ ActionType::INT_CONST, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::DOUBLE_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{ ActionType::DOUBLE_CONST, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::BYTE_CONST, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{ ActionType::BYTE_CONST, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::NOT, ElementType::EXPR, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
				{ ActionType::EMPTY, ActionType::NOT, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::STRING_CONST },
				{ ActionType::STRING_CONST }
			} 
		})
	);
	table_.emplace(
		ElementType::OR_EXPR,
		std::vector<TransitionRule>({
			{ 
				{ ElementType::OR, ElementType::OR_TERM, ElementType::OR_EXPR }, 
				{ ActionType::EMPTY, ActionType::OR, ActionType::EMPTY } 
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
				{ ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::NAME, ElementType::DESC, ElementType::AND_EXPR }, 
				{ ActionType::VAR, ActionType::EMPTY, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::NOT, ElementType::NEG, ElementType::AND_EXPR }, 
				{ ActionType::EMPTY, ActionType::NOT, ActionType::EMPTY } 
			},			
			{ 
				{ ElementType::INT_CONST, ElementType::AND_EXPR }, 
				{ ActionType::INT_CONST, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::DOUBLE_CONST, ElementType::AND_EXPR }, 
				{ ActionType::DOUBLE_CONST, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::BYTE_CONST, ElementType::AND_EXPR },
				{ ActionType::BYTE_CONST, ActionType::EMPTY }
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
				{ ActionType::EMPTY, ActionType::AND, ActionType::EMPTY } 
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
				{ ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::NAME, ElementType::DESC, ElementType::LCOMP_EXPR }, 
				{ ActionType::NAME, ActionType::EMPTY, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::NOT, ElementType::AND_FACTOR, ElementType::LCOMP_EXPR }, 
				{ ActionType::EMPTY, ActionType::NOT, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::INT_CONST, ElementType::LCOMP_EXPR }, 
				{ ActionType::INT_CONST, ActionType::EMPTY} 
			},
			{ 
				{ ElementType::DOUBLE_CONST, ElementType::LCOMP_EXPR },
				{ ActionType::DOUBLE_CONST, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::BYTE_CONST, ElementType::LCOMP_EXPR }, 
				{ ActionType::BYTE_CONST, ActionType::EMPTY } 
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
				{ ActionType::EMPTY, ActionType::LESS } 
			},
			{ 
				{ ElementType::GREATER, ElementType::LEXPR },
				{ ActionType::EMPTY, ActionType::GREATER } 
			},
			{ 
				{ ElementType::EQ, ElementType::LEXPR }, 
				{ ActionType::EMPTY, ActionType::EQ } 
			},
			{ 
				{ ElementType::NEQ, ElementType::LEXPR }, 
				{ ActionType::EMPTY, ActionType::NEQ }
			},
			{ 
				{ ElementType::GEQ, ElementType::LEXPR }, 
				{ ActionType::EMPTY, ActionType::GEQ } 
			},
			{ 
				{ ElementType::LEQ, ElementType::LEXPR }, 
				{ ActionType::EMPTY, ActionType::LEQ } 
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
				{ ActionType::EMPTY, ActionType::PLUS, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::MINUS, ElementType::TERM, ElementType::ADD_EXPR }, 
				{ ActionType::EMPTY, ActionType::MINUS, ActionType::EMPTY} 
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
				{ ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::NAME, ElementType::DESC, ElementType::MULT_EXPR }, 
				{ ActionType::NAME, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::MINUS, ElementType::NEG, ElementType::MULT_EXPR },
				{ ActionType::EMPTY, ActionType::UMINUS, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::INT_CONST, ElementType::MULT_EXPR }, 
				{ ActionType::INT_CONST, ActionType::EMPTY}
			},
			{ 
				{ ElementType::DOUBLE_CONST, ElementType::MULT_EXPR }, 
				{ ActionType::DOUBLE_CONST, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::BYTE_CONST, ElementType::MULT_EXPR }, 
				{ ActionType::BYTE_CONST, ActionType::EMPTY } 
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
				{ ActionType::EMPTY, ActionType::PRODUCT, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::DIVISION, ElementType::FACTOR, ElementType::MULT_EXPR }, 
				{ ActionType::EMPTY, ActionType::DIVISION, ActionType::EMPTY } 
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
				{ ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::NAME, ElementType::DESC }, 
				{ ActionType::NAME, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::MINUS, ElementType::NEG, ElementType::Z }, 
				{ ActionType::EMPTY, ActionType::UMINUS, ActionType::EMPTY }
			},
			{ 
				{ ElementType::INT_CONST }, 
				{ ActionType::INT_CONST } 
			},
			{ 
				{ ElementType::DOUBLE_CONST },
				{ ActionType::DOUBLE_CONST } 
			},
			{ 
				{ ElementType::BYTE_CONST }, 
				{ ActionType::BYTE_CONST }
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
				{ ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
			},
			{ 
				{ ElementType::NAME, ElementType::DESC }, 
				{ ActionType::NAME, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::INT_CONST }, 
				{ ActionType::INT_CONST } 
			},
			{ 
				{ ElementType::DOUBLE_CONST }, 
				{ ActionType::DOUBLE_CONST }
			},
			{ 
				{ ElementType::BYTE_CONST },
				{ ActionType::BYTE_CONST } 
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
				{ ActionType::ARRAY_DEMENSION, ActionType::INT_CONST, ActionType::EMPTY } 
			},
			{ 
				{ ElementType::EMPTY },
				{} 
			} 
		})
	);
	
}

}