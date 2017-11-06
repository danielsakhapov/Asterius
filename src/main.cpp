#include <fstream>
#include <iostream>
#include "LexicalAnalyzer\Lexer.h"
#include "SyntaxAnalyzer\Syntax.h"
#include "Common\Stack.h"
#include "Program\Program.h"

using namespace std;
using namespace asterius;

Program program;

int main()
{
    //Lexer lexer("C:\\Users\\Влад\\Documents\\Visual Studio 2015\\Projects\\Asterius\\src\\Programs\\program1.txt");
    Data ad;
    ad.size_ = 4;
    program.addCommand(make_unique<DataCommand>(ad));
    program.addCommand(make_unique<IntCommand>(5));
    program.addCommand(make_unique<FunctionCommand>(asterius::assign));
    Data bd;
    bd.size_ = 4;
    program.addCommand(make_unique<DataCommand>(bd));
    program.addCommand(make_unique<IntCommand>(10));
    program.addCommand(make_unique<FunctionCommand>(asterius::assign));
    Data cd;
    cd.size_ = 4;
    program.addCommand(make_unique<DataCommand>(cd));
    program.addCommand(make_unique<DataCommand>(ad));
    program.addCommand(make_unique<DataCommand>(bd));
    program.addCommand(make_unique<FunctionCommand>(asterius::plus));
    program.addCommand(make_unique<FunctionCommand>(asterius::assign));
    program.addCommand(make_unique<DataCommand>(cd));
    program.addCommand(make_unique<FunctionCommand>(asterius::write));
    program.execute();
    return 0;
}
