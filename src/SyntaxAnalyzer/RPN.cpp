#include "RPN.h"
#include <functional>

namespace asterius
{

void RPN::addCommand(std::unique_ptr<Command> cmd)
{
	commands_.emplace_back(std::move(cmd));
}

void RPN::execute()
{
	auto sz = commands_.size();
	for (next_ = 0; next_ < sz; ++next_) {
		commands_[next_]->execute(*this);
	}
}

void RPN::beginBlock()
{
	stack_.push();
}

void RPN::endBlock()
{
	stack_.pop();
}

void RPN::addOperand(const Variable& variable)
{
	// add existing variable to operands
	operands_.emplace_back(variable, false);
}

void RPN::createVariable(const Variable& variable)
{
	//allocates variable on stack
	stack_.addVariable(variable);
}

void RPN::createOperand(Variable& variable, void* src)
{
	// create temp variable and add to operands
	stack_.createVariable(variable, src);
	operands_.emplace_back(variable, true);
}

void* RPN::getOperandData(const Variable& variable) const noexcept
{
	return stack_.getData(variable);
}

Variable RPN::getNextOperand()
{
	Variable variable = operands_.back().first;
	if (operands_.back().second) //temp variable remove from stack
	    stack_.removeVariable(variable);
	operands_.pop_back();
	return variable;
}

void RPN::setCommand(size_t position) noexcept
{
	assert(position < commands_.size());
	next_ = position;
}

void RPN::print() const
{
	for (auto&& ptr : commands_) {
		std::cout << ptr->name() << '\n';
	}
}

//Commands

// Command class
Command::Command(const std::string& name)
	: name_(name) // name_ = name;
{
}

const std::string& Command::name() const noexcept
{
	return name_;
}

// WriteCommand class
WriteCommand::WriteCommand()
	: Command("write")
{
}

void WriteCommand::execute(RPN& rpn)
{
	Variable variable = rpn.getNextOperand();
	void* data = rpn.getOperandData(variable);
	switch (variable.type())
	{
	case DataType::INT:
		std::cout << *((int*)data) << std::endl;
		break;
	case DataType::FLOAT:
		std::cout << *((double*)data) << std::endl;
		break;
	case DataType::BYTE:
		std::cout << (int)*((char*)data) << std::endl;
		break;
	default:
		break;
	}
}

// ReadCommand class
ReadCommand::ReadCommand()
	: Command("read")
{
}

void ReadCommand::execute(RPN& rpn)
{
	Variable variable = rpn.getNextOperand();
	void* data = rpn.getOperandData(variable);
	switch (variable.type())
	{
	case DataType::INT:
		std::cin >> *((int*)data);
		break;
	case DataType::FLOAT:
		std::cin >> *((double*)data);
		break;
	case DataType::BYTE:
		std::cin >> *((char*)data);
		break;
	default:
		break;
	}
}

// OperandCommand class
OperandCommand::OperandCommand(const Variable& variable, const std::string& name)
	: Command("operand " + name),
	variable_(variable)
{
}

void OperandCommand::execute(RPN& rpn)
{
	rpn.addOperand(variable_);
}

// CreateVariableCommand class
CreateVariableCommand::CreateVariableCommand(const Variable& variable, const std::string& name)
	: Command("create variable " + name),
	variable_(variable)
{
}

void CreateVariableCommand::execute(RPN& rpn)
{
	rpn.createVariable(variable_);
}

// AddCommand class
AddCommand::AddCommand()
	: Command("operator add")
{
}

void AddCommand::execute(RPN& rpn)
{
	perform(rpn, std::plus<void>());
}

// SubtractCommand class
SubtractCommand::SubtractCommand()
	:Command("operator subtract")
{
}

void SubtractCommand::execute(RPN& rpn)
{
	perform(rpn, std::minus<void>());
}

// MultiplyCommand class
MultiplyCommand::MultiplyCommand()
	:Command("operator multiply")
{
}

void MultiplyCommand::execute(RPN& rpn)
{
	perform(rpn, std::multiplies<void>());
}

// DivideCommand class
DivideCommand::DivideCommand()
	:Command("operator divide")
{
}

void DivideCommand::execute(RPN& rpn)
{
	perform(rpn, std::divides<void>());
}

// NegateCommand class
NegateCommand::NegateCommand()
	:Command("negate")
{
}

void NegateCommand::execute(RPN& rpn)
{
	auto variable = rpn.getNextOperand();
	void* data = rpn.getOperandData(variable);
	switch (variable.type())
	{
	case DataType::INT:
	{
		int res = -get_val<int>(data);
		Variable result(DataType::INT, INT_SIZE);
		rpn.createOperand(result, &res);
		break;
	}
	case DataType::FLOAT:
	{
		double res = -get_val<double>(data);
		Variable result(DataType::FLOAT, FLOAT_SIZE);
		rpn.createOperand(result, &res);
		break;
	}
	case DataType::BYTE:
	{
		char res = -get_val<char>(data);
		Variable result(DataType::BYTE, BYTE_SIZE);
		rpn.createOperand(result, &res);
		break;
	}
	}
}

// JumpCommand class
JumpCommand::JumpCommand()
	:Command("jump")
{
}

void JumpCommand::execute(RPN& rpn)
{
	auto variable = rpn.getNextOperand();
	void* data = rpn.getOperandData(variable);
	rpn.setCommand(get_val<int>(data));
}

// JumpIfNotCommand class
JumpIfNotCommand::JumpIfNotCommand()
	:Command("jump if")
{
}

void JumpIfNotCommand::execute(RPN& rpn)
{
	auto variable = rpn.getNextOperand();
	auto condition = rpn.getNextOperand();
	void* conditionData = rpn.getOperandData(condition);
	if (!get_val<char>(conditionData))
	{
		void* data = rpn.getOperandData(variable);
		rpn.setCommand(get_val<int>(data));
	}
}

// EqualCommand class
EqualCommand::EqualCommand()
	:Command("equal to")
{
}

void EqualCommand::execute(RPN& rpn)
{
	performCompare(rpn, std::equal_to<void>());
}

// NotEqualCommand class
NotEqualCommand::NotEqualCommand()
	:Command("not equal to")
{
}

void NotEqualCommand::execute(RPN& rpn)
{
	performCompare(rpn, std::not_equal_to<void>());
}

// LessCommand class
LessCommand::LessCommand()
	:Command("less")
{
}

void LessCommand::execute(RPN& rpn)
{
	performCompare(rpn, std::less<void>());
}

// GreaterCommand class
GreaterCommand::GreaterCommand()
	:Command("greater")
{
}

void GreaterCommand::execute(RPN& rpn)
{
	performCompare(rpn, std::greater<void>());
}

// LessOrEqualCommand class
LessOrEqualCommand::LessOrEqualCommand()
	:Command("less or equal to")
{
}

void LessOrEqualCommand::execute(RPN& rpn)
{
	performCompare(rpn, std::less_equal<void>());
}

// GreaterOrEqualCommand class
GreaterOrEqualCommand::GreaterOrEqualCommand()
	:Command("greater or equal to")
{
}

void GreaterOrEqualCommand::execute(RPN& rpn)
{
	performCompare(rpn, std::greater_equal<void>());
}

// AndCommand class
AndCommand::AndCommand()
	:Command("and")
{
}

void AndCommand::execute(RPN& rpn)
{
	auto leftVariable = rpn.getNextOperand();
	auto rightVariable = rpn.getNextOperand();
	void* leftData = rpn.getOperandData(leftVariable);
	void* rightData = rpn.getOperandData(rightVariable);
	if (get_val<char>(leftData) && get_val<char>(rightData))
	{
		char res = 1;
		Variable result(DataType::BYTE, BYTE_SIZE);
		rpn.createOperand(result, &res);
	}
	else
	{
		char res = 0;
		Variable result(DataType::BYTE, BYTE_SIZE);
		rpn.createOperand(result, &res);
	}
}

// OrCommand class
OrCommand::OrCommand()
	:Command("or")
{
}

void OrCommand::execute(RPN& rpn)
{
	auto leftVariable = rpn.getNextOperand();
	auto rightVariable = rpn.getNextOperand();
	void* leftData = rpn.getOperandData(leftVariable);
	void* rightData = rpn.getOperandData(rightVariable);
	if (get_val<char>(leftData) || get_val<char>(rightData))
	{
		char res = 1;
		Variable result(DataType::BYTE, BYTE_SIZE);
		rpn.createOperand(result, &res);
	}
	else
	{
		char res = 0;
		Variable result(DataType::BYTE, BYTE_SIZE);
		rpn.createOperand(result, &res);
	}
}

// NotCommand class
NotCommand::NotCommand()
	:Command("not")
{
}

void NotCommand::execute(RPN& rpn)
{
	auto variable = rpn.getNextOperand();
	void* data = rpn.getOperandData(variable);
	char res = !get_val<char>(data) ? 1 : 0;
	Variable result(DataType::BYTE, BYTE_SIZE);
	rpn.createOperand(result, &res);
}

// AssignCommand class
AssignCommand::AssignCommand()
	:Command("assign")
{
}

void AssignCommand::execute(RPN& rpn)
{
	auto rightVariable = rpn.getNextOperand();
	auto leftVariable = rpn.getNextOperand();
	void* leftData = rpn.getOperandData(leftVariable);
	void* rightData = rpn.getOperandData(rightVariable);
	switch (leftVariable.type())
	{
	case DataType::INT:
	{
		switch (rightVariable.type())
		{
		case DataType::INT:
		{
			int rightValue = get_val<int>(rightData);
			*(int*)leftData = rightValue;
			break;
		}
		case DataType::FLOAT:
		{
			double rightValue = get_val<double>(rightData);
			*(int*)leftData = rightValue;
			break;
		}
		case DataType::BYTE:
		{
			char rightValue = get_val<char>(rightData);
			*(int*)leftData = rightValue;
			break;
		}
		default:
			break;
		}
		break;
	}
	case DataType::FLOAT:
	{
		switch (rightVariable.type())
		{
		case DataType::INT:
		{
			int rightValue = get_val<int>(rightData);
			*(double*)leftData = rightValue;
			break;
		}
		case DataType::FLOAT:
		{
			double rightValue = get_val<double>(rightData);
			*(double*)leftData = rightValue;
			break;
		}
		case DataType::BYTE:
		{
			char rightValue = get_val<char>(rightData);
			*(double*)leftData = rightValue;
			break;
		}
		default:
			break;
		}
		break;
	}
	case DataType::BYTE:
	{
		switch (rightVariable.type())
		{
		case DataType::INT:
		{
			int rightValue = get_val<int>(rightData);
			*(char*)leftData = rightValue;
			break;
		}
		case DataType::FLOAT:
		{
			double rightValue = get_val<double>(rightData);
			*(char*)leftData = rightValue;
			break;
		}
		case DataType::BYTE:
		{
			char rightValue = get_val<char>(rightData);
			*(char*)leftData = rightValue;
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

// BeginBlockCommand
BeginBlockCommand::BeginBlockCommand()
	:Command("begin block")
{
}

void BeginBlockCommand::execute(RPN& rpn)
{
	rpn.beginBlock();
}

// EndBlockCommand
EndBlockCommand::EndBlockCommand()
	:Command("end block")
{
}

void EndBlockCommand::execute(RPN& rpn)
{
	rpn.endBlock();
}

template<class value_type>
value_type get_val(void* ptr)
{
	return *static_cast<value_type*>(ptr); //возвращает значение определенного типа, static_cast - преобразует выражения одного статического типа в объекты и значения другого статического типа.
}

template<class operation>
void perform(RPN& rpn, operation op)
{
	auto rightVariable = rpn.getNextOperand();
	auto leftVariable = rpn.getNextOperand();

	void* leftData = rpn.getOperandData(leftVariable);
	void* rightData = rpn.getOperandData(rightVariable);
	switch (leftVariable.type())
	{
	case DataType::INT:
	{
		switch (rightVariable.type())
		{
		case DataType::INT:
		{
			int res = op(get_val<int>(leftData), get_val<int>(rightData)); //вызов нужной операции
			Variable result(DataType::INT, INT_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		case DataType::FLOAT:
		{
			double res = op(get_val<int>(leftData), get_val<double>(rightData));
			Variable result(DataType::FLOAT, FLOAT_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		case DataType::BYTE:
		{
			int res = op(get_val<int>(leftData), get_val<char>(rightData));
			Variable result(DataType::INT, INT_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		default:
			break;
		}
	}
	break;
	case DataType::FLOAT:
	{
		switch (rightVariable.type())
		{
		case DataType::INT:
		{
			double res = op(get_val<double>(leftData), get_val<int>(rightData));
			Variable result(DataType::FLOAT, FLOAT_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		case DataType::FLOAT:
		{
			double res = op(get_val<double>(leftData), get_val<double>(rightData));
			Variable result(DataType::FLOAT, FLOAT_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		case DataType::BYTE:
		{
			double res = op(get_val<double>(leftData), get_val<char>(rightData));
			Variable result(DataType::FLOAT, FLOAT_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		default:
			break;
		}
	}
	break;
	case DataType::BYTE:
	{
		int leftByte = *(char*)leftData;
		switch (rightVariable.type())
		{
		case DataType::INT:
		{
			int res = op(get_val<char>(leftData), get_val<int>(rightData));
			Variable result(DataType::INT, INT_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		case DataType::FLOAT:
		{
			double res = op(get_val<char>(leftData), get_val<double>(rightData));
			Variable result(DataType::FLOAT, FLOAT_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		case DataType::BYTE:
		{
			char res = op(get_val<char>(leftData), get_val<char>(rightData));
			Variable result(DataType::BYTE, BYTE_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		default:
			break;
		}
	}
	break;
	default:
		break;
	}
}

template<class operation>
void performCompare(RPN& rpn, operation op)
{
	auto rightVariable = rpn.getNextOperand();
	auto leftVariable = rpn.getNextOperand();

	void* leftData = rpn.getOperandData(leftVariable);
	void* rightData = rpn.getOperandData(rightVariable);
	switch (leftVariable.type())
	{
	case DataType::INT:
	{
		switch (rightVariable.type())
		{
		case DataType::INT:
		{
			char res = op(get_val<int>(leftData), get_val<int>(rightData)) ? 1 : 0; //вызов нужной операции, op - это указатель на нужную функцию
			Variable result(DataType::BYTE, BYTE_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		case DataType::FLOAT:
		{
			char res = op(get_val<int>(leftData), get_val<double>(rightData)) ? 1 : 0;
			Variable result(DataType::BYTE, BYTE_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		case DataType::BYTE:
		{
			char res = op(get_val<int>(leftData), get_val<char>(rightData)) ? 1 : 0;
			Variable result(DataType::BYTE, BYTE_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		default:
			break;
		}
	}
	break;
	case DataType::FLOAT:
	{
		switch (rightVariable.type())
		{
		case DataType::INT:
		{
			char res = op(get_val<double>(leftData), get_val<int>(rightData)) ? 1 : 0;
			Variable result(DataType::BYTE, BYTE_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		case DataType::FLOAT:
		{
			char res = op(get_val<double>(leftData), get_val<double>(rightData)) ? 1 : 0;
			Variable result(DataType::BYTE, BYTE_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		case DataType::BYTE:
		{
			char res = op(get_val<double>(leftData), get_val<char>(rightData)) ? 1 : 0;
			Variable result(DataType::BYTE, BYTE_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		default:
			break;
		}
	}
	break;
	case DataType::BYTE:
	{
		int leftByte = *(char*)leftData;
		switch (rightVariable.type())
		{
		case DataType::INT:
		{
			char res = op(get_val<char>(leftData), get_val<int>(rightData)) ? 1 : 0;
			Variable result(DataType::BYTE, BYTE_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		case DataType::FLOAT:
		{
			char res = op(get_val<char>(leftData), get_val<double>(rightData)) ? 1 : 0;
			Variable result(DataType::BYTE, BYTE_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		case DataType::BYTE:
		{
			char res = op(get_val<char>(leftData), get_val<char>(rightData)) ? 1 : 0;
			Variable result(DataType::BYTE, BYTE_SIZE);
			rpn.createOperand(result, &res);
		}
		break;
		default:
			break;
		}
	}
	break;
	default:
		break;
	}
}

}