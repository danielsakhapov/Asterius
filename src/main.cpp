#include <fstream>
#include <iostream>
#include "LexicalAnalyzer/Lexer.h"
#include "SyntaxAnalyzer/Syntax.h"

using namespace std;
using namespace asterius;

int main()
{
	Lexer lexer("src/Examples/program.txt");
	Parser parser(std::move(lexer));
	RPN mainRpn = parser.analyze();
	mainRpn.print();
	mainRpn.execute();
	return 0;
}
