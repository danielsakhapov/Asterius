#ifndef BASIC_STRUCTURES
#define BASIC_STRUCTURES

namespace asterius
{

enum class DataType
{
    INT,
    DOBULE,
    STRING
};


//Think more about this
struct Data
{
    DataType type_;
    size_t size_;
    size_t offset_;
    char isRef = false;
};

// HELPER FUNCTIONS

bool isDigit(char c);
bool isLetter(char c);
int charToInt(char c);

}
#endif // !BASIC_STRUCTURES