#ifndef RPN_
#define RPN_

#include <stack>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <algorithm>

#include "TypeInfo.h"
#include "RPNStructures.h"
#include "SyntaxStructures.h"
#include "Common/Stack.h"
#include "Common/BasicStructures.h"

namespace asterius
{

class Command;

class RPN
{
public:
    void addCommand(std::unique_ptr<Command> cmd);
    void addOperand(const Variable& variable); //add existing variable to operands
    void createOperand(Variable& variable); // create temp variable and add to operands
    void createVariable(const Variable& variable); //allocates variable on stack
    void* getOperandData(const Variable& variable) const noexcept;
    Variable getNextOperand();
    void setCommand(size_t position);
    void print() const;
    void execute();
private:
    Stack<12800> stack_; //stack to hold data
    std::vector<std::unique_ptr<Command> > commands_; //RPN
    size_t next_; //instruction pointer
    std::vector<std::pair<Variable, bool> > operands_; //bool flag is true for temp variables
};

class Command
{
public:
    explicit Command(const std::string& name);
    const std::string& name() const noexcept;
    virtual void execute(RPN& rpn) = 0;
private:
    std::string name_;
};

class WriteCommand : public Command 
{
public:
    WriteCommand();
    void execute(RPN& rpn) override;
};

class ReadCommand : public Command 
{
public:
    ReadCommand();
    void execute(RPN& rpn) override;
};

class OperandCommand : public Command
{
public:
    OperandCommand(const Variable& variable, const std::string& name);
    void execute(RPN& rpn) override;
private:
    Variable variable_;
};

class CreateVariableCommand : public Command
{
public:
    CreateVariableCommand(const Variable& variable, const std::string& name);
    void execute(RPN& rpn) override;
private:
    Variable variable_;
};

}

#endif