#include "Syntax.h"
#include <algorithm>
#include <iterator>

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
    symbol_table_.push(); //globes level
    Token token = lexer_.getNextToken();
    while (!lexer_.eof()) {
        try {           
            if (actionsStack_.empty())
                throw std::logic_error("invalid syntax near: " + to_string(token.getPosition()));
            auto topElement = elementsStack_.back();
            generate(rpn, token);
            elementsStack_.pop_back();
            actionsStack_.pop_back();
            if (!isTerminal(topElement)) {
                transit(topElement, token);
            }
            else {
				match(topElement, token);
                token = lexer_.getNextToken();
            }
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << " ";
            throw;
        }
    }
	while (!actionsStack_.empty()) {
		auto topElement = elementsStack_.back();
		generate(rpn, token);
		elementsStack_.pop_back();
		actionsStack_.pop_back();
		if (!isTerminal(topElement)) {
			transit(topElement, token);
		}
		else {
			match(topElement, token);
		}
	}
    return rpn;
}

void Parser::generate(RPN& rpn, const Token& token)
{
    switch (actionsStack_.back())
    {
    case asterius::ActionType::OR:
		rpn.addCommand(std::make_unique<OrCommand>());
        break;
    case asterius::ActionType::EQ:
		rpn.addCommand(std::make_unique<EqualCommand>());
        break;
    case asterius::ActionType::LEQ:
		rpn.addCommand(std::make_unique<LessOrEqualCommand>());
        break;
    case asterius::ActionType::GEQ:
		rpn.addCommand(std::make_unique<GreaterOrEqualCommand>());
        break;
    case asterius::ActionType::NEQ:
		rpn.addCommand(std::make_unique<NotEqualCommand>());
        break;
    case asterius::ActionType::VAR:
        rpn.addCommand(std::make_unique<OperandCommand>(symbol_table_.find(name_), name_));
        break;
    case asterius::ActionType::AND:
		rpn.addCommand(std::make_unique<AndCommand>());
        break;
    case asterius::ActionType::NOT:
		rpn.addCommand(std::make_unique<NegateCommand>());
        break;
    case asterius::ActionType::INT:
    {
        Variable var(DataType::INT, INT_SIZE, token.getPosition());
        symbol_table_.insert(name_, var);
        if (var.isRelative())
            rpn.addCommand(std::make_unique<CreateVariableCommand>(var, name_));
        else
            rpn.createVariable(var);
    }
        break;
    case asterius::ActionType::BYTE:
	{
		Variable var(DataType::BYTE, BYTE_SIZE, token.getPosition());
		symbol_table_.insert(name_, var);
		if (var.isRelative())
			rpn.addCommand(std::make_unique<CreateVariableCommand>(var, name_));
		else
			rpn.createVariable(var);
	}
        break;
    case asterius::ActionType::NAME:
        name_ = token.getName();
        break;
    case asterius::ActionType::MAIN:
        break;
    case asterius::ActionType::READ:
        rpn.addCommand(std::make_unique<ReadCommand>());
        break;
    case asterius::ActionType::LESS:
		rpn.addCommand(std::make_unique<LessCommand>());
        break;
    case asterius::ActionType::PLUS:
		rpn.addCommand(std::make_unique<AddCommand>());
        break;
    case asterius::ActionType::MINUS:
		rpn.addCommand(std::make_unique<SubtractCommand>());
        break;
    case asterius::ActionType::ARRAY:
        break;
    case asterius::ActionType::INDEX:
        break;
    case asterius::ActionType::EMPTY:
        break;
    case asterius::ActionType::WRITE:
        rpn.addCommand(std::make_unique<WriteCommand>());
        break;
    case asterius::ActionType::UMINUS:
		rpn.addCommand(std::make_unique<NegateCommand>());
        break;
    case asterius::ActionType::ASSIGN:
		rpn.addCommand(std::make_unique<AssignCommand>());
        break;
    case asterius::ActionType::STRING:
        break;
    case asterius::ActionType::DOUBLE:
	{
		Variable var(DataType::FLOAT, FLOAT_SIZE, token.getPosition());
		symbol_table_.insert(name_, var);
		if (var.isRelative())
			rpn.addCommand(std::make_unique<CreateVariableCommand>(var, name_));
		else
			rpn.createVariable(var);
	}
        break;
    case asterius::ActionType::IF_END:
        break;
    case asterius::ActionType::PRODUCT:
		rpn.addCommand(std::make_unique<MultiplyCommand>());
        break;
    case asterius::ActionType::GREATER:
		rpn.addCommand(std::make_unique<GreaterCommand>());
        break;
    case asterius::ActionType::IF_BEGIN:
        break;
    case asterius::ActionType::ELSE_END:
        break;
    case asterius::ActionType::DIVISION:
		rpn.addCommand(std::make_unique<DivideCommand>());
        break;
    case asterius::ActionType::BLOCK_END:
        symbol_table_.pop();
        break;
    case asterius::ActionType::INT_CONST:
        break;
    case asterius::ActionType::FN_CREATE:
        break;
    case asterius::ActionType::WHILE_END:
        break;
    case asterius::ActionType::ELSE_START:
        break;
    case asterius::ActionType::VAR_CREATE:
        break;
    case asterius::ActionType::BYTE_CONST:
        break;
    case asterius::ActionType::ZERO_CONST:
        break;
    case asterius::ActionType::PARAMS_END:
        break;
    case asterius::ActionType::WHILE_BEGIN:
        break;
    case asterius::ActionType::BLOCK_BEGIN:
        symbol_table_.push();
        break;
    case asterius::ActionType::PARAMS_BEGIN:
        break;
    case asterius::ActionType::DOUBLE_CONST:
        break;
    case asterius::ActionType::STRING_CONST:
        break;
    case asterius::ActionType::FUNCTION_CALL:
        break;
    case asterius::ActionType::CONDITION_END:
        break;
    case asterius::ActionType::CONDITION_BEGIN:
        break;
    case asterius::ActionType::ARRAY_DEMENSION:
        break;
    default:
        break;
    }
}

void Parser::transit(ElementType elementType, const Token& token)
{
	if (elementType == ElementType::EMPTY) return;
    const auto& rules = table_.at(elementType);
    for (const auto& it: rules) {
        if (token.getType() == it.elements_[0]) {
            std::copy(it.elements_.crbegin(), it.elements_.crend(), std::back_inserter(elementsStack_));
            std::copy(it.acts_.crbegin(), it.acts_.crend(), std::back_inserter(actionsStack_));
            return;
        }
    }
	if ((rules.back().elements_[0] == ElementType::EMPTY)) {
		std::copy(rules.back().elements_.crbegin(), rules.back().elements_.crend(), std::back_inserter(elementsStack_));
		std::copy(rules.back().acts_.crbegin(), rules.back().acts_.crend(), std::back_inserter(actionsStack_));
	}
	else {
		throw std::logic_error("invalid syntax");
	}
}

void Parser::match(ElementType elementType, const Token& token) const
{
	if (elementType != token.getType())
		throw std::logic_error("invalid syntax near" + to_string(token.getPosition()));
}

bool Parser::isTerminal(ElementType elementType) const noexcept
{
    return toUnderlying(elementType) >= INDEX_OF_FIRST_TERMINAL; // l00kz l1ke maGGG1ck
}



Parser::Parser(Lexer&& lexer)
    : lexer_(std::move(lexer))
{
    elementsStack_.push_back(ElementType::FUNC);
    actionsStack_.push_back(ActionType::EMPTY);

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
                { ActionType::MAIN, ActionType::PARAMS_BEGIN, ActionType::EMPTY, ActionType::PARAMS_END, ActionType::EMPTY }
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
                { ActionType::EMPTY }
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
                { ActionType::EMPTY }
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
            },
			{
				{ ElementType::ARRAY, ElementType::OF, ElementType::LTYPEDEF, ElementType::OF, ElementType::INT_CONST, ElementType::ZARR },
				{ ActionType::ARRAY, ActionType::EMPTY, ActionType::EMPTY, ActionType::ARRAY_DEMENSION, ActionType::INT_CONST, ActionType::EMPTY }
			},
        })
    );
	table_.emplace(
		ElementType::LTYPEDEF,
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
			},
	})
	);
    table_.emplace(
        ElementType::VALUE, 
        std::vector<TransitionRule>({
            { 
                { ElementType::INT_CONST }, 
                { ActionType::ASS_INT_CONST }
            },
            { 
                { ElementType::DOUBLE_CONST },
                { ActionType::ASS_DOUBLE_CONST } 
            },
            { 
                { ElementType::BYTE_CONST }, 
                { ActionType::ASS_BYTE_CONST } 
            },
            { 
                { ElementType::STRING_CONST },
                { ActionType::ASS_STRING_CONST }
            },          
            { 
                { ElementType::EMPTY },
                { ActionType::EMPTY }
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
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::NAME, ActionType::EMPTY, ActionType::EMPTY, ActionType::READ, ActionType::EMPTY }
            },
            { 
                { ElementType::WRITE, ElementType::OPEN_BRACKET, ElementType::EXPR, ElementType::CLOSE_BRACKET, ElementType::STATEMENT_END, ElementType::STATEMENT }, 
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::WRITE, ActionType::EMPTY }
            },
            { 
                { ElementType::NAME, ElementType::DESC, ElementType::ASS, ElementType::STATEMENT_END, ElementType::STATEMENT },
                { ActionType::NAME, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
            },
            { 
                { ElementType::LET, ElementType::NAME, ElementType::BE, ElementType::TYPEDEF, ElementType::VALUE, ElementType::STATEMENT_END, ElementType::STATEMENT },
                { ActionType::VAR_CREATE, ActionType::NAME, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
            },
            { 
                { ElementType::EMPTY },
                { ActionType::EMPTY }
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
                { ActionType::EMPTY }
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
                { ActionType::EMPTY }
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
                { ActionType::VAR }
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
                { ActionType::EMPTY }
            } 
        })
    );
    table_.emplace(
        ElementType::PARAM,
        std::vector<TransitionRule>({
            { 
                { ElementType::NAME, ElementType::DESC, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM },
                { ActionType::NAME, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
            },
            { 
                { ElementType::MINUS, ElementType::NEG, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR, ElementType::NEXT_PARAM },
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::UMINUS, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
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
                { ActionType::EMPTY }
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
                { ActionType::EMPTY }
            } 
        })
    );
    table_.emplace(
        ElementType::EXPR,
        std::vector<TransitionRule>({
            { 
                { ElementType::NAME, ElementType::DESC, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
                { ActionType::NAME, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY } 
            },
            { 
                { ElementType::MINUS, ElementType::NEG, ElementType::MULT_EXPR, ElementType::ADD_EXPR, ElementType::COMP_EXPR, ElementType::AND_EXPR, ElementType::OR_EXPR }, 
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::UMINUS, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
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
                { ElementType::LESS, ElementType::LEXPR, ElementType::Z }, 
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::LESS }
            },
            { 
                { ElementType::GREATER, ElementType::LEXPR, ElementType::Z },
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::GREATER }
            },
            { 
                { ElementType::EQ, ElementType::LEXPR, ElementType::Z },
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::EQ }
            },
            { 
                { ElementType::NEQ, ElementType::LEXPR, ElementType::Z },
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::NEQ }
            },
            { 
                { ElementType::GEQ, ElementType::LEXPR, ElementType::Z },
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::GEQ }
            },
            { 
                { ElementType::LEQ, ElementType::LEXPR, ElementType::Z },
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::LEQ }
            },
            {
                { ElementType::EMPTY },
                { ActionType::EMPTY }
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
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::UMINUS, ActionType::EMPTY, ActionType::EMPTY, ActionType::EMPTY }
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
                { ActionType::EMPTY }
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
                { ActionType::EMPTY }
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
                { ActionType::EMPTY }
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
                { ActionType::EMPTY }
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
                { ActionType::EMPTY }
            } 
        })
    );
    table_.emplace(
        ElementType::ADD_EXPR,
        std::vector<TransitionRule>({
            { 
                { ElementType::PLUS, ElementType::TERM, ElementType::ADD_EXPR },
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::PLUS }
            },
            { 
                { ElementType::MINUS, ElementType::TERM, ElementType::ADD_EXPR }, 
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::MINUS }
            },
            { 
                { ElementType::EMPTY },
                { ActionType::EMPTY }
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
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::UMINUS }
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
                { ActionType::EMPTY } 
            } 
        })
    );
    table_.emplace(
        ElementType::MULT_EXPR,
        std::vector<TransitionRule>({
            { 
                { ElementType::PRODUCT, ElementType::FACTOR, ElementType::MULT_EXPR }, 
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::PRODUCT }
            },
            { 
                { ElementType::DIVISION, ElementType::FACTOR, ElementType::MULT_EXPR }, 
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::DIVISION }
            },
            { 
                { ElementType::EMPTY },
                { ActionType::EMPTY }
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
                { ActionType::EMPTY, ActionType::EMPTY, ActionType::UMINUS }
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
                { ActionType::EMPTY }
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
                { ActionType::EMPTY }
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
                { ActionType::EMPTY }
            } 
        })
    );
    
}

}