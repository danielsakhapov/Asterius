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
    data.offset_ = stack_.top();
    stack_.push(data.size_);
}

void Program::addConstant(void* src, Data& data)
{
    data.offset_ = constants_.top();
    constants_.push(data.size_);
    void* dest = constants_.get(data.offset_);
    memcpy(dest, src, data.size_);
}

void Program::addOperand(Data& data)
{
    if (!data.isRef_) { //temp value save in stack
        data.offset_ = stack_.top();
        stack_.push(data.size_);
    }
    operands_.push(data);
}

void Program::popOperand() noexcept
{
    const Data& data = operands_.top();
    if (!data.isRef_)//temp value free memory
        stack_.pop(data.size_);
    operands_.pop();
}

const Data& Program::topOperand() const noexcept
{
    return operands_.top();
}

void* Program::getOperand(const Data& data) const noexcept
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
    int* op1 = (int*)program.getOperand(data1);
    program.popOperand();

    const Data& data2 = program.topOperand();
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
    int* op2 = (int*)program.getOperand(data2);
    program.popOperand();
    const Data& data1 = program.topOperand();
    int* op1 = (int*)program.getOperand(data1);
    program.popOperand();
    *op1 = *op2;
}

void asterius::write(Program& program)
{
    //test example
    const Data& data1 = program.topOperand();
    int* op1 = (int*)program.getOperand(data1);
    program.popOperand();
    cout << *op1;
}

void asterius::read(Program& program)
{
    //test example
    const Data& data1 = program.topOperand();
    int* op1 = (int*)program.getOperand(data1);
    program.popOperand();
    cin >> *op1;
}

//................