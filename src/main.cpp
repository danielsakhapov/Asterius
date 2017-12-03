#include <fstream>
#include <iostream>
#include "LexicalAnalyzer/Lexer.h"
#include "SyntaxAnalyzer/Syntax.h"
#include "Common/Stack.h"
#include "Program/Program.h"

using namespace std;
using namespace asterius;

Program program;

int main()
{
    Lexer lex("src/Examples/program1.txt", program);
    while (!lex.eof()) {
        try {
            Token token = lex.getNextToken();
            cout << token.getName() << " ";
        }
        catch (const exception& ex) {
            cerr << ex.what() << " ";
        }
    }
    //test example
    /*Data ad, bd, cd;
    ad.size_ = 4;
    bd.size_ = 4;
    cd.size_ = 4;
    program.addVariable(ad); ad.isRef_ = true;
    program.addVariable(bd); bd.isRef_ = true;
    program.addVariable(cd); cd.isRef_ = true;

    program.addCommand(make_unique<DataCommand>(ad));
    program.addCommand(make_unique<FunctionCommand>(asterius::read));
    program.addCommand(make_unique<DataCommand>(bd));
    program.addCommand(make_unique<FunctionCommand>(asterius::read));

    program.addCommand(make_unique<DataCommand>(cd));
    program.addCommand(make_unique<DataCommand>(ad));
    program.addCommand(make_unique<DataCommand>(bd));
    program.addCommand(make_unique<FunctionCommand>(asterius::plus));
    program.addCommand(make_unique<FunctionCommand>(asterius::assign));
    program.addCommand(make_unique<DataCommand>(cd));
    program.addCommand(make_unique<FunctionCommand>(asterius::write));
    program.execute();*/
    return 0;
}
