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

void Program::addVariable(Data& data)
{
    stack_.push(data);
}

void Program::addConstant(Data& data, void* src)
{
    stack_.push(data, src);
}

void Program::addOperand(Data& data, void* src)
{
    if (!data.isRef_) { //temp value save in stack
        stack_.push(data, src);
    }
    operands_.push(data);
}

void Program::addOperand(Data& data)
{
    if (!data.isRef_) { //temp value save in stack
        stack_.push(data);
    }
    operands_.push(data);
}

void Program::popOperand() noexcept
{
    const Data& data = operands_.top();
    if (!data.isRef_)//temp value free memory
        stack_.pop(data);
    operands_.pop();
}

const Data& Program::topOperand() const noexcept
{
    return operands_.top();
}

JumpCommand::JumpCommand(size_t pos) noexcept
    : label_(pos)
{
}

void JumpCommand::apply(Program& program)
{
    cout << "Jumped to position " << label_;
    program.popOperand();
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

DataCommand::DataCommand(const Data& data) noexcept
    : data_(data)
{
}

void DataCommand::apply(Program& program)
{
    program.addOperand(data_);
}

//OPERATORS

void asterius::plus(Program& program)
{
    //test example
    const Data& data1 = program.topOperand();
    int* op1 = (int*)data1.data_;
    program.popOperand();

    const Data& data2 = program.topOperand();
    int* op2 = (int*)data2.data_;
    program.popOperand();

    Data res_data;
    res_data.size_ = 4;
    res_data.type_ = DataType::INT;
    int res = *op1 + *op2;
    program.addOperand(res_data, &res);
}

void asterius::minus(Program& program)
{
    program.popOperand();
    //DO SOME STUFF
}

void asterius::product(Program& program)
{
    program.popOperand();
    //DO SOME STUFF
}

void asterius::divide(Program& program)
{
    program.popOperand();
    //DO SOME STUFF
}

void asterius::assign(Program& program)
{
    //test example
    const Data& data2 = program.topOperand();
    int* op2 = (int*)data2.data_;
    program.popOperand();
    const Data& data1 = program.topOperand();
    int* op1 = (int*)data1.data_;
    program.popOperand();
    *op1 = *op2;
}

void asterius::write(Program& program)
{
    //test example
    const Data& data = program.topOperand();
    int* op = (int*)data.data_;
    program.popOperand();
    cout << *op;
}

void asterius::read(Program& program)
{
    //test example
    const Data& data = program.topOperand();
    int* op = (int*)data.data_;
    program.popOperand();
    cin >> *op;
}

//................