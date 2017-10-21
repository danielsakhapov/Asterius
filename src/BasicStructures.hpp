
#ifndef BASIC_STRUCTURES
#define BASIC_STRUCTURES

namespace asterius
{

enum class TokenType
{
    Int,
    Double,
    Id,
    Eq,
    Read,
    Write,
};

// FUNCTIONS

bool isDigit(char c);
bool isLetter(char c);
int charToInt(char c);

}
#endif // !BASIC_STRUCTURES