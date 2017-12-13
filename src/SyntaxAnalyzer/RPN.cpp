#include "RPN.h"

namespace asterius
{

RPN::RPN(const std::vector<Command>& commands) :
	commands_(commands)
{
	//locals_.push(std::vector<std::string>());
}

RPN::RPN(const std::vector<Variable>& stack) :
	stack_(stack)
{}

RPN::RPN(const std::vector<Command>& commands, const std::vector<Variable>& stack) :
	commands_(commands), stack_(stack)
{}

void RPN::addCommand(const Action& act)
{
	if (act.type_ == ActionType::EMPTY) {
		return;
	}
	
	Command cmd;

	// if (act.type_ == ActionType::BLOCK_BEGIN) {
	// 	locals_.push(std::vector<std::string>());
	// }

	// if (act.type_ == ActionType::BLOCK_END) {
	// 	for (const auto& it: locals_.top()) {
	// 		vars_.at(it).pop();
	// 	}
	// 	locals_.pop();
	// }
	
	// if (
	// 	act.type_ == ActionType::INT || act.type_ == ActionType::FN || act.type_ == ActionType::DOUBLE
	// ) {
	// 	std::string varName = *(static_cast<std::string*>(act.data_));
	// 	if (std::find(locals_.top().begin(), locals_.top().end(), varName) != locals_.top().end()) {
	// 		throw std::logic_error("redefining variable");
	// 	}

	// 	locals_.top().push_back(varName);

	// 	if (vars_.find(varName) != vars_.end()) {
	// 		vars_.at(varName).push(commands_.size());
	// 	}
	// 	else {
	// 		vars_.emplace(varName, std::move(std::stack<std::size_t>( {commands_.size()} )));
	// 	}

	// 	delete (std::string*) act.data_;
	// 	act.data_ = (void*) new std::size_t;
	// }

	// if (act.type_ == ActionType::VAR) {
	// 	std::string varName = *(static_cast<std::string*>(act.data_));
	// 	if (vars_.find(varName) != vars_.end()) {			
	// 		delete (std::string*) act.data_;
	// 		act.data_ = (void*) new std::size_t(vars_.at(varName).top());
	// 	}
	// 	else {
	// 		throw std::logic_error("undefined variable");
	// 	}
	// }

	commands_.push_back(std::move(cmd));
}

Variable RPN::execute()
{
	return Variable( DataType::INT, new int(0) );
}

}