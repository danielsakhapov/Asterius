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
    void addVariable(Data& data);
    void addOperand(Data& data);//shouldn't be reference?
    void popOperand() noexcept;
    const Data& topOperand() const noexcept;
    void* getOperand(const Data& data) const noexcept;
private:
    Stack<128000> stack_;
    std::vector<std::unique_ptr<Command> > commands_;
    std::stack<Data> operands_;
    size_t next_command_;
};


class Command
{
public:
    virtual void apply(Program& program) = 0;
};


//JumpCommand
class JumpCommand : public Command {
public:
    JumpCommand(size_t pos) noexcept;
    void apply(Program& program) override;
private:
    size_t label_; // position of jump
};


//Hold function
class FunctionCommand : public Command {
public:
    typedef void(*funcPtr)(Program& program);

    FunctionCommand(funcPtr ptr) noexcept;
    void apply(Program& program) override;
private:
    funcPtr func_;
};

//Hold variable type and refernce to a variable 
class DataCommand : public Command {
public:
    DataCommand(const Data& data) noexcept;
    void apply(Program& program) override;
private:
    Data data_;
};

//OPERATORS FOR FUNCTION COMMAND?????
void plus(Program& program);
void minus(Program& program);
void product(Program& program);
void divide(Program& program);
void assign(Program& program);
void write(Program& program);
void read(Program& program);

}
#endif