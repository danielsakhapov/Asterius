#include "RPN.h"

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

void RPN::createOperand(Variable& variable)
{
    // create temp variable and add to operands
    stack_.createVariable(variable);
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

void RPN::setCommand(size_t position)
{
    assert(position < commands_.size());
    next_ = position;
}

void RPN::print() const
{
    for (auto&& ptr: commands_) {
        std::cout << ptr->name() << '\n';
    }
}

//Commands

Command::Command(const std::string& name)
    : name_(name)
{
}

const std::string& Command::name() const noexcept
{
    return name_;
}

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
        std::cout << *((int*)data);
    case DataType::FLOAT:
        //OTHER TYPES
    default:
        break;
    }
}

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
    case DataType::FLOAT:
        //OTHER TYPES
    default:
        break;
    }
}

OperandCommand::OperandCommand(const Variable& variable, const std::string& name)
    : Command("operand " + name),
    variable_(variable)
{
}

void OperandCommand::execute(RPN& rpn)
{
    rpn.addOperand(variable_);
}

CreateVariableCommand::CreateVariableCommand(const Variable& variable, const std::string& name)
    : Command("create variable " + name),
    variable_(variable)
{
}

void CreateVariableCommand::execute(RPN& rpn)
{
    rpn.createVariable(variable_);
}

}