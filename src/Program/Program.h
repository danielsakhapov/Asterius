#ifndef PROGRAM
#define PROGRAM
#include "Common\Stack.h"
#include "BasicStructures.hpp"
#include <memory>
#include <vector>

namespace asterius 
{

class Command;

class Program final
{
public:
    void execute();
    void addCommand(std::unique_ptr<Command> ptr);
    void addOperand(Data& data);
    void popOperand();
    Data& topOperand();
    void* getOperand(const Data& data);
private:
    Stack<128000> stack_;
    std::vector<std::unique_ptr<Command> > commands_;
    std::stack<Data> operands_;
    size_t next_command_;
};

//base class for commands 
//can be inherited for additional state
class Command
{
public:
    virtual void apply(Program& program) = 0;
};


//Example
class JumpCommand : public Command {
public:
    JumpCommand(size_t pos) noexcept;
    void apply(Program& program) override;
private:
    size_t label_; // position of jump
};


class FunctionCommand : public Command {
public:
    typedef void(*funcPtr)(Program& program);

    FunctionCommand(funcPtr ptr) noexcept;
    void apply(Program& program) override;
private:
    funcPtr func_;
};


class DataCommand : public Command {
public:
    DataCommand(Data& data) noexcept;
    void apply(Program& program) override;
private:
    Data& data_;
};

class IntCommand : public Command {
public:
    IntCommand(int val) noexcept;
    void apply(Program& program) override;
private:
    int val_;
};


//OPERATORS FOR FUNCTION COMMAND?????
void plus(Program& program);
void minus(Program& program);
void product(Program& program);
void divide(Program& program);
void assign(Program& program);
void write(Program& program);

}
#endif