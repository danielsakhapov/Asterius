#include "Syntax.h"

namespace asterius
{

Parser::Parser(Lexer&& lexer)
	: lexer_(std::move(lexer))
{
	stack_.push(ElementType::FUNC);
	table_.emplace(
		ElementType::FUNC,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::FN, { ElementType::SUB_FUNC } ),
			TransitionRule( ElementType::LET, { ElementType::NAME, ElementType::BE, ElementType::TYPEDEF, ElementType::VALUE, ElementType::SEPARATOR, ElementType::FUNC } )
		})
	);
	table_.emplace(
		ElementType::SUB_FUNC,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::NAME, { ElementType::OPEN_BRACKET, ElementType::ARGS, ElementType::CLOSE_BRACKET, ElementType::BLOCK, ElementType::FUNC } ),
			TransitionRule( ElementType::MAIN, { ElementType::OPEN_BRACKET, ElementType::ARGS, ElementType::CLOSE_BRACKET, ElementType::BLOCK } )
		})
	);
	table_.emplace(
		ElementType::ARGS,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::LET, { ElementType::NAME, ElementType::BE, ElementType::TYPEDEF, ElementType::VALUE, ElementType::NEXT_ARG } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::NEXT_ARG,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::COMMA, { ElementType::LET, ElementType::NAME, ElementType::BE, ElementType::TYPEDEF, ElementType::VALUE, ElementType::NEXT_ARG } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::TYPEDEF, 
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::INT, {  } ),
			TransitionRule( ElementType::DOUBLE, {  } ),
			TransitionRule( ElementType::BYTE, {  } ),
			TransitionRule( ElementType::STRING, {  } ),
		})
	);
	table_.emplace(
		ElementType::VALUE, 
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::INT_CONST, {  } ),
			TransitionRule( ElementType::DOUBLE_CONST, {  } ),
			TransitionRule( ElementType::BYTE_CONST, {  } ),
			TransitionRule( ElementType::STRING_CONST, {  } ),
			TransitionRule( ElementType::ARRAY, { ElementType::OF, ElementType::INT_CONST, ElementType::ZARR } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::BLOCK,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::OPEN_FBRACKET, { ElementType::STATEMENT, ElementType::CLOSE_FBRACKET } )
		})
	);
	table_.emplace(
		ElementType::STATEMENT,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::WHILE, { ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::BLOCK, ElementType::STATEMENT } ),
			TransitionRule( ElementType::IF, { ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::BLOCK, ElementType::ELSEST } ),
			TransitionRule( ElementType::READ, { ElementType::OPEN_BRACKET, ElementType::NAME, ElementType::DESC, ElementType::CLOSE_BRACKET, ElementType::SEPARATOR, ElementType::STATEMENT } ),
			TransitionRule( ElementType::WRITE, { ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::SEPARATOR, ElementType::STATEMENT } ),
			TransitionRule( ElementType::NAME, { ElementType::DESC, ElementType::SET, ElementType::EXPR, ElementType::SEPARATOR, ElementType::STATEMENT } ),
			TransitionRule( ElementType::LET, { ElementType::NAME, ElementType::BE, ElementType::TYPEDEF, ElementType::VALUE, ElementType::SEPARATOR, ElementType::STATEMENT } ),
			TransitionRule( ElementType::NAME, { ElementType::DESC, ElementType::SEPARATOR, ElementType::STATEMENT } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::ELSEST,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::ELSE, { ElementType::BLOCK, ElementType::STATEMENT } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::DESC,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::OPEN_IBRACKET, { ElementType::EXPR, ElementType::CLOSE_IBRACKET, ElementType::INDEX } ),
			TransitionRule( ElementType::OPEN_BRACKET, { ElementType::PARAM, ElementType::CLOSE_BRACKET } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::INDEX,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::OPEN_IBRACKET, { ElementType::EXPR, ElementType::CLOSE_IBRACKET, ElementType::INDEX } ),			
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::PARAM,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::NAME, { ElementType::DESC, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM } ),
			TransitionRule( ElementType::MINUS, { ElementType::NEG, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM } ),
			TransitionRule( ElementType::OPEN_BRACKET, { ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM } ),
			TransitionRule( ElementType::INT_CONST, { ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM } ),
			TransitionRule( ElementType::DOUBLE_CONST, { ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM } ),
			TransitionRule( ElementType::BYTE_CONST, { ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM } ),
			TransitionRule( ElementType::NOT, { ElementType::EXPR, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM } ),
			TransitionRule( ElementType::STRING_CONST, { ElementType::NEXT_PARAM } )
		})
	);
	table_.emplace(
		ElementType::NEXT_PARAM,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::NAME, { ElementType::COMMA, ElementType::EXPR, ElementType::NEXT_PARAM } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::EXPR,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::NAME, { ElementType::DESC, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::MINUS, { ElementType::NEG, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::OPEN_BRACKET, { ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::INT_CONST, { ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::DOUBLE_CONST, { ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::BYTE_CONST, { ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::NOT, { ElementType::EXPR, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::STRING_CONST, {} )
		})
	);
	table_.emplace(
		ElementType::COMP_EXPR,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::LESS, { ElementType::LEXPR } ),
			TransitionRule( ElementType::GREATER, { ElementType::LEXPR } ),
			TransitionRule( ElementType::EQ, { ElementType::LEXPR } ),
			TransitionRule( ElementType::NEQ, { ElementType::LEXPR } ),
			TransitionRule( ElementType::GEQ, { ElementType::LEXPR } ),
			TransitionRule( ElementType::LEQ, { ElementType::LEXPR } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::LEXPR,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::NAME, { ElementType::DESC, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::MINUS, { ElementType::NEG, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::OPEN_BRACKET, { ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::INT_CONST, { ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::DOUBLE_CONST, { ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::BYTE_CONST, { ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::NOT, { ElementType::EXPR, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::STRING_CONST, {} )
		})
	);
	table_.emplace(
		ElementType::OR_EXPR,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::OR, { ElementType::OR_TERM, ElementType::OR_EXPR } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::OR_TERM,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::OPEN_BRACKET, { ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::AND_EXPR } ),
			TransitionRule( ElementType::NAME, { ElementType::DESC, ElementType::AND_EXPR } ),
			TransitionRule( ElementType::NOT, { ElementType::NEG, ElementType::AND_EXPR } ),
			TransitionRule( ElementType::NOT, { ElementType::NEG, ElementType::AND_EXPR } ),
			TransitionRule( ElementType::INT_CONST, { ElementType::AND_EXPR } ),
			TransitionRule( ElementType::DOUBLE_CONST, { ElementType::AND_EXPR } ),
			TransitionRule( ElementType::BYTE_CONST, { ElementType::AND_EXPR } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::AND_EXPR,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::AND, { ElementType::AND_FACTOR, ElementType::AND_EXPR } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::AND_FACTOR,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::OPEN_BRACKET, { ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::LCOMP_EXPR } ),
			TransitionRule( ElementType::NAME, { ElementType::DESC, ElementType::LCOMP_EXPR } ),
			TransitionRule( ElementType::NOT, { ElementType::AND_FACTOR, ElementType::LCOMP_EXPR } ),
			TransitionRule( ElementType::INT_CONST, { ElementType::LCOMP_EXPR } ),
			TransitionRule( ElementType::DOUBLE_CONST, { ElementType::LCOMP_EXPR } ),
			TransitionRule( ElementType::BYTE_CONST, { ElementType::LCOMP_EXPR } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::LCOMP_EXPR,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::LESS, { ElementType::LEXPR } ),
			TransitionRule( ElementType::GREATER, { ElementType::LEXPR } ),
			TransitionRule( ElementType::EQ, { ElementType::LEXPR } ),
			TransitionRule( ElementType::NEQ, { ElementType::LEXPR } ),
			TransitionRule( ElementType::GEQ, { ElementType::LEXPR } ),
			TransitionRule( ElementType::LEQ, { ElementType::LEXPR } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::ADD_EXPR,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::PLUS, { ElementType::TERM, ElementType::ADD_EXPR } ),
			TransitionRule( ElementType::MINUS, { ElementType::TERM, ElementType::ADD_EXPR } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::TERM,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::OPEN_BRACKET, { ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::MULT_EXPR } ),
			TransitionRule( ElementType::NAME, { ElementType::DESC, ElementType::MULT_EXPR } ),
			TransitionRule( ElementType::MINUS, { ElementType::NEG, ElementType::MULT_EXPR } ),
			TransitionRule( ElementType::INT_CONST, { ElementType::MULT_EXPR } ),
			TransitionRule( ElementType::DOUBLE_CONST, { ElementType::MULT_EXPR } ),
			TransitionRule( ElementType::BYTE_CONST, { ElementType::MULT_EXPR } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::MULT_EXPR,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::STAR, { ElementType::FACTOR, ElementType::MULT_EXPR } ),
			TransitionRule( ElementType::SLASH, { ElementType::FACTOR, ElementType::MULT_EXPR } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::FACTOR,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::OPEN_BRACKET, { ElementType::EXPR, ElementType::CLOSE_BRACKET } ),
			TransitionRule( ElementType::NAME, { ElementType::DESC } ),
			TransitionRule( ElementType::MINUS, { ElementType::NEG, ElementType::Z } ),
			TransitionRule( ElementType::INT_CONST, { ElementType::DESC } ),
			TransitionRule( ElementType::DOUBLE_CONST, { ElementType::DESC } ),
			TransitionRule( ElementType::BYTE_CONST, { ElementType::DESC } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::NEG,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::OPEN_BRACKET, { ElementType::EXPR, ElementType::CLOSE_BRACKET } ),
			TransitionRule( ElementType::NAME, { ElementType::DESC } ),
			TransitionRule( ElementType::INT_CONST, { ElementType::DESC } ),
			TransitionRule( ElementType::DOUBLE_CONST, { ElementType::DESC } ),
			TransitionRule( ElementType::BYTE_CONST, { ElementType::DESC } )
		})
	);
	table_.emplace(
		ElementType::Z,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::EMPTY, {})
		})
	);
	table_.emplace(
		ElementType::ZARR,
		std::move(std::vector<TransitionRule>{
			TransitionRule( ElementType::BY, { ElementType::INT_CONST, ElementType::ZARR } ),
			TransitionRule( ElementType::EMPTY, {})
		})
	);
}



RPN Parser::generate()
{
	RPN rpn({});
	Command bb;
	ElementType prevElement;
	bb.type_ = CommandType::BLOCK_BEGIN;
	rpn.addCommand(std::move(bb));
	std::stack<std::string> nameStack;
	while (!lexer_.eof()) {
		try {
			Token token = lexer_.getNextToken();
			#ifdef SYNTAX_DEBUG
			std::cout << token.getName() << std::endl;
			#endif
			transit(token);
			if (isTerminal(stack_.top())) {
				if (stack_.top() == ElementType::EMPTY)
					stack_.pop();
				ElementType element = stack_.top();								
				if (element != tokenToElement(token)) {
					throw std::logic_error("hmm");
				}
				// if (element == ElementType::NAME) {
				// 	if (prevElement == ElementType::LET) {
				// 		nameStack.push(token.getName());
				// 	}
				// 	else {
				// 		Command cmd;
				// 		cmd.type_ = CommandType::VAR;
				// 		cmd.data_ = (void*) new std::string(token.getName());
				// 		rpn.addCommand(std::move(cmd));
				// 	}
				// }
				// if (element == ElementType::INT) {
				// 	Command cmdi;
				// 	cmdi.type_ = CommandType::INT_CONST;
				// 	cmdi.data_ = (void*) new int(0);
				// 	rpn.addCommand(std::move(cmdi));
				// 	Command cmd;
				// 	cmd.type_ = CommandType::INT;
				// 	cmd.data_ = (void*) new std::string(nameStack.top());
				// 	nameStack.pop();
				// 	rpn.addCommand(std::move(cmd));
				// }
				// if (element == ElementType::INT_CONST && prevElement == ElementType::INT) {					
				// 	*((int*)rpn.getCommandRevRef(2).data_) = stoi(token.getName());					
				// }
				// prevElement = element;
				stack_.pop();
			}			
		}
		catch (const std::exception& ex) {
			std::cerr << ex.what() << " ";
			break;
		}
	}
	Command be;
	be.type_ = CommandType::BLOCK_END;
 	rpn.addCommand(std::move(be));
}



void Parser::transit(const Token& token)
{
	if (!isTerminal(stack_.top())) {
		try {
			const auto& rules = table_.at(stack_.top());
			for (const auto& it: rules) {
				if (it.term_ == tokenToElement(token) || it.term_ == ElementType::EMPTY) {
					stack_.pop();					
					for (auto it2 = it.vec_.rbegin(); it2 != it.vec_.rend(); ++it2) {
						stack_.push(*it2);
					}
					stack_.push(it.term_);
					return;
				}				
			}
			throw std::logic_error("wow! l00kz like 1'm c0nfuzzzed");
		}
		catch (const std::exception& ex) {
			#ifdef SYNTAX_DEBUG
			std::cerr << ex.what() << std::endl;
			#endif
		}
	}
}



ElementType Parser::tokenToElement(const Token& token) const noexcept
{
	if (token.getType() == TokenType::OPEN_FIGURE) 
		return ElementType::OPEN_FBRACKET;

	if (token.getType() == TokenType::CLOSE_FIGURE) 
		return ElementType::CLOSE_FBRACKET;

	if (token.getType() == TokenType::ID)
 		return ElementType::NAME;

	if (token.getType() == TokenType::NOT) 
 		return ElementType::NOT;

	if (token.getType() == TokenType::LET) 
 		return ElementType::LET;

	if (token.getType() == TokenType::BE) 
 		return ElementType::BE;

	if (token.getType() == TokenType::BY) 
 		return ElementType::BY;

	if (token.getType() == TokenType::FN) 
 		return ElementType::FN;

	if (token.getType() == TokenType::ASSIGN) 
 		return ElementType::SET;

	if (token.getType() == TokenType::OPEN_SQUARE) 
 		return ElementType::OPEN_IBRACKET;

	if (token.getType() == TokenType::CLOSE_SQUARE) 
 		return ElementType::CLOSE_IBRACKET;

	if (token.getType() == TokenType::STRING) 
 		return ElementType::STRING;

	if (token.getType() == TokenType::ARRAY) 
 		return ElementType::ARRAY;

	if (token.getType() == TokenType::INT && token.getName() == "int")
		return ElementType::INT;

	if (token.getType() == TokenType::DOUBLE && token.getName() == "double")
		return ElementType::DOUBLE;

	if (token.getType() == TokenType::BYTE && token.getName() == "byte")
		return ElementType::BYTE;

	if (token.getType() == TokenType::INT)
		return ElementType::INT_CONST;

	if (token.getType() == TokenType::DOUBLE)
		return ElementType::DOUBLE_CONST;

	if (token.getType() == TokenType::BYTE)
		return ElementType::BYTE_CONST;

	if (token.getType() == TokenType::MINUS) 
 		return ElementType::MINUS;

	if (token.getType() == TokenType::PLUS) 
 		return ElementType::PLUS;

	if (token.getType() == TokenType::PRODUCT) 
 		return ElementType::STAR;

	if (token.getType() == TokenType::DIVISION) 
 		return ElementType::SLASH;

	if (token.getType() == TokenType::OF) 
 		return ElementType::OF;

	if (token.getType() == TokenType::OR) 
 		return ElementType::OR;

	if (token.getType() == TokenType::AND) 
 		return ElementType::AND;

	if (token.getType() == TokenType::STATEMENT_END) 
 		return ElementType::SEPARATOR;

	if (token.getType() == TokenType::OPEN_BRACKET) 
 		return ElementType::OPEN_BRACKET;

	if (token.getType() == TokenType::CLOSE_BRACKET) 
 		return ElementType::CLOSE_BRACKET;

	if (token.getType() == TokenType::MAIN) 
 		return ElementType::MAIN;

	if (token.getType() == TokenType::COMMA) 
 		return ElementType::COMMA;

	if (token.getType() == TokenType::STRING) 
 		return ElementType::STRING_CONST;

	if (token.getType() == TokenType::WHILE) 
 		return ElementType::WHILE;

	if (token.getType() == TokenType::READ) 
 		return ElementType::READ;

	if (token.getType() == TokenType::WRITE) 
 		return ElementType::WRITE;

	if (token.getType() == TokenType::IF) 
 		return ElementType::IF;

	if (token.getType() == TokenType::ELSE) 
 		return ElementType::ELSE;

	if (token.getType() == TokenType::LESS) 
 		return ElementType::LESS;

	if (token.getType() == TokenType::GREATER) 
 		return ElementType::GREATER;

	if (token.getType() == TokenType::EQ) 
 		return ElementType::EQ;

	if (token.getType() == TokenType::NEQ) 
 		return ElementType::NEQ;

	if (token.getType() == TokenType::GEQ) 
 		return ElementType::GEQ;

	if (token.getType() == TokenType::LEQ) 
 		return ElementType::LEQ;
}



bool Parser::isTerminal(const ElementType& element) const noexcept
{
	if (element == ElementType::EMPTY)
		return true;

	if (element == ElementType::INT)
		return true;

	if (element == ElementType::DOUBLE)
		return true;

	if (element == ElementType::BYTE)
		return true;

	if (element == ElementType::OPEN_FBRACKET)
		return true;

	if (element == ElementType::CLOSE_FBRACKET)
		return true;

	if (element == ElementType::NAME)
		return true;

	if (element == ElementType::NOT)
		return true;

	if (element == ElementType::LET)
		return true;

	if (element == ElementType::BE)
		return true;

	if (element == ElementType::BY)
		return true;

	if (element == ElementType::FN)
		return true;

	if (element == ElementType::SET)
		return true;

	if (element == ElementType::OPEN_IBRACKET)
		return true;

	if (element == ElementType::CLOSE_IBRACKET)
		return true;

	if (element == ElementType::STRING)
		return true;

	if (element == ElementType::ARRAY)
		return true;

	if (element == ElementType::INT_CONST)
		return true;

	if (element == ElementType::DOUBLE_CONST)
		return true;

	if (element == ElementType::BYTE_CONST)
		return true;

	if (element == ElementType::MINUS)
		return true;

	if (element == ElementType::PLUS)
		return true;

	if (element == ElementType::STAR)
		return true;

	if (element == ElementType::SLASH)
		return true;

	if (element == ElementType::OF)
		return true;

	if (element == ElementType::OR)
		return true;

	if (element == ElementType::AND)
		return true;

	if (element == ElementType::SEPARATOR)
		return true;

	if (element == ElementType::OPEN_BRACKET)
		return true;

	if (element == ElementType::CLOSE_BRACKET)
		return true;

	if (element == ElementType::MAIN)
		return true;

	if (element == ElementType::COMMA)
		return true;

	if (element == ElementType::STRING_CONST)
		return true;

	if (element == ElementType::WHILE)
		return true;

	if (element == ElementType::READ)
		return true;

	if (element == ElementType::WRITE)
		return true;

	if (element == ElementType::IF)
		return true;

	if (element == ElementType::ELSE)
		return true;

	if (element == ElementType::LESS)
		return true;

	if (element == ElementType::GREATER)
		return true;

	if (element == ElementType::EQ)
		return true;

	if (element == ElementType::NEQ)
		return true;

	if (element == ElementType::GEQ)
		return true;

	if (element == ElementType::LEQ)
		return true;

	return false;
}

}