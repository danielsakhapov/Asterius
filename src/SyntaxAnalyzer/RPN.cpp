#include "RPN.h"
#include <functional>

namespace asterius
{

void RPN::addCommand(std::unique_ptr<Command> cmd)
{
    commands_.emplace_back(std::move(cmd));
}

void RPN::addCommand(std::unique_ptr<Command> cmd, size_t position)
{
	if (commands_[position])
		commands_[position].release();
	commands_[position] = std::move(cmd);
}

size_t RPN::getSize()
{
	return commands_.size();
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

void RPN::createVariable(const Variable& variable, void* src)
{
	stack_.addVariable(variable, src);
}

void RPN::createOperand(Variable& variable, void* src)
{
    // create temp variable and add to operands
    stack_.createVariable(variable, src);
    operands_.emplace_back(variable, true);
}

void RPN::createOperand(Variable&& variable, void* src)
{
	// create temp variable and add to operands
	stack_.createVariable(variable, src);
	operands_.emplace_back(variable, true);
}

std::pair<Variable, void*> RPN::getNextOperand()
{
    const auto& variable = operands_.back().first;
    std::pair<Variable, void*> ret(variable, stack_.getData(variable));
    if (operands_.back().second) //temp variable remove from stack
        stack_.removeVariable(variable);
    operands_.pop_back();
    return ret;
}

void RPN::setCommand(size_t position) noexcept
{
    assert(position < commands_.size());
    next_ = position;
}

void RPN::print() const
{
	int pos = 0;
	for (auto&& ptr : commands_) {
		std::cout << pos << ' ' << ptr->name() << '\n';
		++pos;
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
    //auto [variable, data] = rpn.getNextOperand();
    auto pr = rpn.getNextOperand();
    Variable variable = pr.first;
    void* data = pr.second;
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
    auto pr = rpn.getNextOperand();
	Variable variable = pr.first;
	void* data = pr.second;
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
    auto pr = rpn.getNextOperand();
	Variable variable = pr.first;
	void* data = pr.second;
    switch (variable.type())
    {
    case DataType::INT:
    {
        int res = -get_val<int>(data);
        rpn.createOperand(make_variable<int>(), &res);
        break;
    }
    case DataType::FLOAT:
    {
        double res = -get_val<double>(data);
        rpn.createOperand(make_variable<double>(), &res);
        break;
    }
    case DataType::BYTE:
    {
        char res = -get_val<char>(data);
        rpn.createOperand(make_variable<char>(), &res);
        break;
    }
	default:
		throw std::logic_error("Wrong type!");
    }
}

// JumpCommand class
JumpCommand::JumpCommand()
    :Command("jump")
{
}

void JumpCommand::execute(RPN& rpn)
{
    auto pr = rpn.getNextOperand();
	Variable variable = pr.first;
	void* data = pr.second;
	if (variable.type() != DataType::INT)
	{
		throw std::logic_error("Wrong type!");
	}
    rpn.setCommand(get_val<int>(data));
}

// JumpIfNotCommand class
JumpIfNotCommand::JumpIfNotCommand()
    :Command("jump if")
{
}

void JumpIfNotCommand::execute(RPN& rpn)
{
    auto pr = rpn.getNextOperand();
	Variable variable = pr.first;
	void* data = pr.second;
	if (variable.type() != DataType::INT)
	{
		throw std::logic_error("Wrong type!");
	}
    pr = rpn.getNextOperand();
    Variable condition = pr.first;
    void* conditionData = pr.second;
	if (condition.type() != DataType::BYTE)
	{
		throw std::logic_error("Wrong type!");
	}
    if (!get_val<char>(conditionData))
        rpn.setCommand(get_val<int>(data));
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
    auto pr = rpn.getNextOperand();
    auto rightVariable = pr.first;
    void* rightData = pr.second;
    pr = rpn.getNextOperand();
    auto leftVariable = pr.first;
    void* leftData = pr.second;
	if (leftVariable.type() != DataType::BYTE || rightVariable.type() != DataType::BYTE)
	{
		throw std::logic_error("Wrong type!");
	}
    if (get_val<char>(leftData) && get_val<char>(rightData))
    {
        char res = 1;
        rpn.createOperand(make_variable<char>(), &res);
    }
    else
    {
        char res = 0;
        rpn.createOperand(make_variable<char>(), &res);
    }
}

// OrCommand class
OrCommand::OrCommand()
    :Command("or")
{
}

void OrCommand::execute(RPN& rpn)
{
    auto pr = rpn.getNextOperand();
    auto rightVariable = pr.first;
    void* rightData = pr.second;
    pr = rpn.getNextOperand();
    auto leftVariable = pr.first;
    void* leftData = pr.second;
	if (leftVariable.type() != DataType::BYTE || rightVariable.type() != DataType::BYTE)
	{
		throw std::logic_error("Wrong type!");
	}
    if (get_val<char>(leftData) || get_val<char>(rightData))
    {
        char res = 1;
        rpn.createOperand(make_variable<char>(), &res);
    }
    else
    {
        char res = 0;
        rpn.createOperand(make_variable<char>(), &res);
    }
}

// NotCommand class
NotCommand::NotCommand()
    :Command("not")
{
}

void NotCommand::execute(RPN& rpn)
{
    auto pr = rpn.getNextOperand();
    auto variable = pr.first;
    void* data = pr.second;
	if (variable.type() != DataType::BYTE)
	{
		throw std::logic_error("Wrong type!");
	}
    char res = !get_val<char>(data) ? 1 : 0;
    rpn.createOperand(make_variable<char>(), &res);
}

// AssignCommand class
AssignCommand::AssignCommand()
    :Command("assign")
{
}

void AssignCommand::execute(RPN& rpn)
{
    auto pr = rpn.getNextOperand();
    auto rightVariable = pr.first;
    void* rightData = pr.second;
    pr = rpn.getNextOperand();
    auto leftVariable = pr.first;
    void* leftData = pr.second;
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
            *(int*)leftData = static_cast<int>(rightValue);
            break;
        }
        case DataType::BYTE:
        {
            char rightValue = get_val<char>(rightData);
            *(int*)leftData = rightValue;
            break;
        }
        default:
			throw std::logic_error("Wrong type!");
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
			throw std::logic_error("Wrong type!");
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
            *(char*)leftData = static_cast<char>(rightValue);
            break;
        }
        case DataType::FLOAT:
        {
            double rightValue = get_val<double>(rightData);
            *(char*)leftData = static_cast<char>(rightValue);
            break;
        }
        case DataType::BYTE:
        {
            char rightValue = get_val<char>(rightData);
            *(char*)leftData = rightValue;
            break;
        }
        default:
			throw std::logic_error("Wrong type!");
        }
        break;
    }
    default:
		throw std::logic_error("Wrong type!");
    }
}

// IndexCommand class
IndexCommand::IndexCommand()
	:Command("index")
{
}

void IndexCommand::execute(RPN& rpn)
{
	auto pr = rpn.getNextOperand();
	auto variable = pr.first;
	if (variable.type()!= DataType::INT)
	{
		throw std::logic_error("Array index must be integer number!");
	}
	void* data = pr.second;
	int index = get_val<int>(data);
	pr = rpn.getNextOperand();
	variable = pr.first;
	if (variable.type() != DataType::ARRAY)
	{
		throw std::logic_error("Array expected!");
	}
	data = pr.second;
	auto passport = get_val<array_passport>(data);
	if (passport.size_ <= index || index < 0)
	{
		throw std::logic_error("Array index out of bounce!");
	}
	int offset = variable.offset() + (int)passport.block_offset_ + (int)passport.element_size_ * index;
	Variable result(passport.element_type_, passport.element_size_);
	result.setOffset(offset);
    //result.setRelative(false);
	rpn.addOperand(result);
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
    auto pr = rpn.getNextOperand();
    auto rightVariable = pr.first;
    void* rightData = pr.second;
    pr = rpn.getNextOperand();
    auto leftVariable = pr.first;
    void* leftData = pr.second;
    switch (leftVariable.type())
    {
    case DataType::INT:
    {
        switch (rightVariable.type())
        {
        case DataType::INT:
        {
            int res = op(get_val<int>(leftData), get_val<int>(rightData)); //вызов нужной операции
            rpn.createOperand(make_variable<int>(), &res);
        }
        break;
        case DataType::FLOAT:
        {
            double res = op(get_val<int>(leftData), get_val<double>(rightData));
            rpn.createOperand(make_variable<double>(), &res);
        }
        break;
        case DataType::BYTE:
        {
            int res = op(get_val<int>(leftData), get_val<char>(rightData));
            rpn.createOperand(make_variable<int>(), &res);
        }
        break;
        default:
			throw std::logic_error("Wrong type!");
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
            rpn.createOperand(make_variable<double>(), &res);
        }
        break;
        case DataType::FLOAT:
        {
            double res = op(get_val<double>(leftData), get_val<double>(rightData));
            rpn.createOperand(make_variable<double>(), &res);
        }
        break;
        case DataType::BYTE:
        {
            double res = op(get_val<double>(leftData), get_val<char>(rightData));
            rpn.createOperand(make_variable<double>(), &res);
        }
        break;
        default:
			throw std::logic_error("Wrong type!");
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
            rpn.createOperand(make_variable<int>(), &res);
        }
        break;
        case DataType::FLOAT:
        {
            double res = op(get_val<char>(leftData), get_val<double>(rightData));
            rpn.createOperand(make_variable<double>(), &res);
        }
        break;
        case DataType::BYTE:
        {
            char res = op(get_val<char>(leftData), get_val<char>(rightData));
            rpn.createOperand(make_variable<char>(), &res);
        }
        break;
        default:
			throw std::logic_error("Wrong type!");
        }
    }
    break;
    default:
		throw std::logic_error("Wrong type!");
    }
}

template<class operation>
void performCompare(RPN& rpn, operation op)
{
    auto pr = rpn.getNextOperand();
    auto rightVariable = pr.first;
    void* rightData = pr.second;
    pr = rpn.getNextOperand();
    auto leftVariable = pr.first;
    void* leftData = pr.second;
    switch (leftVariable.type())
    {
    case DataType::INT:
    {
        switch (rightVariable.type())
        {
        case DataType::INT:
        {
            char res = op(get_val<int>(leftData), get_val<int>(rightData)) ? 1 : 0; //вызов нужной операции, op - это указатель на нужную функцию
            rpn.createOperand(make_variable<char>(), &res);
        }
        break;
        case DataType::FLOAT:
        {
            char res = op(get_val<int>(leftData), get_val<double>(rightData)) ? 1 : 0;
            rpn.createOperand(make_variable<char>(), &res);
        }
        break;
        case DataType::BYTE:
        {
            char res = op(get_val<int>(leftData), get_val<char>(rightData)) ? 1 : 0;
            rpn.createOperand(make_variable<char>(), &res);
        }
        break;
        default:
			throw std::logic_error("Wrong type!");
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
            rpn.createOperand(make_variable<char>(), &res);
        }
        break;
        case DataType::FLOAT:
        {
            char res = op(get_val<double>(leftData), get_val<double>(rightData)) ? 1 : 0;
            rpn.createOperand(make_variable<char>(), &res);
        }
        break;
        case DataType::BYTE:
        {
            char res = op(get_val<double>(leftData), get_val<char>(rightData)) ? 1 : 0;
            rpn.createOperand(make_variable<char>(), &res);
        }
        break;
        default:
			throw std::logic_error("Wrong type!");
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
            rpn.createOperand(make_variable<char>(), &res);
        }
        break;
        case DataType::FLOAT:
        {
            char res = op(get_val<char>(leftData), get_val<double>(rightData)) ? 1 : 0;
            rpn.createOperand(make_variable<char>(), &res);
        }
        break;
        case DataType::BYTE:
        {
            char res = op(get_val<char>(leftData), get_val<char>(rightData)) ? 1 : 0;
            rpn.createOperand(make_variable<char>(), &res);
        }
        break;
        default:
			throw std::logic_error("Wrong type!");
        }
    }
    break;
    default:
		throw std::logic_error("Wrong type!");
    }
}

}