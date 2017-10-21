#include "Syntax.h"

namespace asterius
{

Parser::Parser(Lexer&& lexer)
    : lexer_(std::move(lexer))
{
}

}