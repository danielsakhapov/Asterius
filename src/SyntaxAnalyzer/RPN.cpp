#include "RPN.h"

namespace asterius
{

RPN::RPN(const std::vector<Command>& commands) :
	commands_(commands)
{
	//locals_.push(std::vector<std::string>());
}

void RPN::addCommand(Command&& cmd)
{
	if (cmd.type_ == CommandType::BLOCK_BEGIN) {
		locals_.push(std::vector<std::string>());
	}

	if (cmd.type_ == CommandType::BLOCK_END) {
		for (const auto& it: locals_.top()) {
			vars_.at(it).pop();
		}
		locals_.pop();
	}
	
	if (
		cmd.type_ == CommandType::INT || cmd.type_ == CommandType::FN || cmd.type_ == CommandType::DOUBLE
	) {
		std::string varName = *(static_cast<std::string*>(cmd.data_));
		if (std::find(locals_.top().begin(), locals_.top().end(), varName) != locals_.top().end()) {
			throw std::logic_error("redefining variable");
		}

		locals_.top().push_back(varName);

		if (vars_.find(varName) != vars_.end()) {
			vars_.at(varName).push(commands_.size());
		}
		else {
			vars_.emplace(varName, std::move(std::stack<std::size_t>( {commands_.size()} )));
		}

		delete (std::string*) cmd.data_;
		cmd.data_ = (void*) new std::size_t;
	}

	if (cmd.type_ == CommandType::VAR) {
		std::string varName = *(static_cast<std::string*>(cmd.data_));
		if (vars_.find(varName) != vars_.end()) {			
			delete (std::string*) cmd.data_;
			cmd.data_ = (void*) new std::size_t(vars_.at(varName).top());
		}
		else {
			throw std::logic_error("undefined variable");
		}
	}

	commands_.push_back(std::move(cmd));
}

Command& RPN::getCommandRevRef(size_t i)
{
	return commands_[commands_.size() - 2];
}

Variable RPN::execute()
{
	return Variable( DataType::INT, new int(0) );
}

}