#include "BasicStructures.h"

namespace asterius
{

//FUNCTIONS

bool isDigit(char c)
{
    return '0' <= c && c <= '9';
}

bool isLetter(char c)
{
    return 'a' <= c && c <= 'z'
        || 'A' <= c && c <= 'Z';
}

int charToInt(char c)
{
    return c - '0';
}

}