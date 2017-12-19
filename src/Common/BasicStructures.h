#ifndef BASIC_STRUCTURES
#define BASIC_STRUCTURES

namespace asterius
{

//DEFINES

#ifdef _DEBUG
  #define DEBUG_CODE(code) code
#else // DEBUG
  #define DEBUG_CODE(code)
#endif

// HELPER FUNCTIONS

bool isDigit(char c);
bool isLetter(char c);
int charToInt(char c);

}
#endif // !BASIC_STRUCTURES