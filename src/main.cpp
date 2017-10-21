#include <fstream>
#include "LexicalAnalyzer\Lexer.h"
#include "SyntaxAnalyzer\Syntax.h"

using namespace std;
using namespace asterius;

int main()
{
    Lexer lexer("??");
    Parser parser(std::move(lexer));
    return 0;
}
