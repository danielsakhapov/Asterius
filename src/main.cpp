#include <fstream>
#include <iostream>
#include "Lexer.h"
#include "Syntax.h"

using namespace std;
using namespace asterius;

int main()
{
	Lexer lex("src/Examples/program1.txt");
	while (!lex.eof()) {
		try {
			Token token = lex.getNextToken();
			cout << token.getName() << " ";
		}
		catch (const exception& ex) {
			cerr << ex.what() << " ";
		}
	}

	return 0;
}
