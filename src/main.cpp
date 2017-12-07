#include <fstream>
#include <iostream>
#include "LexicalAnalyzer/Lexer.h"
#include "SyntaxAnalyzer/Syntax.h"

using namespace std;
using namespace asterius;

int main()
{
	Lexer lexer("src/Examples/program1.txt");
	Parser parser(std::move(lexer));
	parser.generate();

	return 0;
}
