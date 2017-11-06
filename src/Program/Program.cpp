#include <iostream>
#include "Program.h"

using namespace std;
using namespace asterius;

void Program::execute()
{
    for (; next_command_ < commands_.size(); ++next_command_) {
        commands_[next_command_]->apply(*this);//++next_command jumps should be 1 step before??
    }
}

void Program::addCommand(unique_ptr<Command> ptr)
{
    commands_.emplace_back(std::move(ptr));
}

void Program::addOperand(Data& data)
{
    if (!data.isRef) {
        data.offset_ = stack_.top();
        stack_.push(data.size_);
    }
    operands_.push(data);
}

void Program::popOperand()
{
    const Data& data = operands_.top();
    if (!data.isRef)
        stack_.pop(data.size_);
    operands_.pop();
}

Data& Program::topOperand()
{
    return operands_.top();
}

void* Program::getOperand(const Data& data)
{
    return stack_.get(data.offset_);
}

JumpCommand::JumpCommand(size_t pos) noexcept
    : label_(pos)
{
}

void JumpCommand::apply(Program& program)
{
    cout << "Jumped to position " << label_;
    //DO SOME JUMP STUFF
}

FunctionCommand::FunctionCommand(funcPtr ptr) noexcept
    : func_(ptr)
{
}

void FunctionCommand::apply(Program& program)
{
    func_(program);
}

DataCommand::DataCommand(Data& data) noexcept
    : data_(data)
{
}

void DataCommand::apply(Program& program)
{
    program.addOperand(data_);
    program.topOperand().isRef = true;
    data_.isRef = true; // rework
}

IntCommand::IntCommand(int val) noexcept
    : val_(val)
{
}

void IntCommand::apply(Program& program)
{
    //test example
    Data data;
    data.size_ = 4;
    data.type_ = DataType::INT;
    program.addOperand(data);
    int* ptr = (int*)program.getOperand(data);
    *ptr = val_;
}

//OPERATORS

void asterius::plus(Program& program)
{
    //test example
    Data data1 = program.topOperand();//group to 1 func
    int* op1 = (int*)program.getOperand(data1);
    program.popOperand();

    Data data2 = program.topOperand();
    int* op2 = (int*)program.getOperand(data2);
    program.popOperand();

    Data res_data;
    res_data.size_ = 4;
    res_data.type_ = DataType::INT;
    program.addOperand(res_data);
    int* res = (int*)program.getOperand(res_data);
    *res = *op1 + *op2;
}

void asterius::minus(Program& program)
{
    //DO SOME STUFF
}

void asterius::product(Program& program)
{
    //DO SOME STUFF
}

void asterius::divide(Program& program)
{
    //DO SOME STUFF
}

void asterius::assign(Program& program)
{
    //test example
    Data data2 = program.topOperand();
    int* op2 = (int*)program.getOperand(data2);
    program.popOperand();
    Data data1 = program.topOperand();
    int* op1 = (int*)program.getOperand(data1);
    program.popOperand();
    *op1 = *op2;
}

void asterius::write(Program& program)
{
    //test example
    Data data1 = program.topOperand();
    int* op1 = (int*)program.getOperand(data1);
    program.popOperand();
    cout << *op1;
}

//................