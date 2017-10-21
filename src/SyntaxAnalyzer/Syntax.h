#ifndef SYNTAX
#define SYNTAX
#include "LexicalAnalyzer\Lexer.h"

namespace asterius
{

class Parser final
{
public:
    Parser(Lexer&& lexer);
private:
    Lexer lexer_;
};

}
#endif // !SYNTAX