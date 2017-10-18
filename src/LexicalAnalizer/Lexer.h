#include <fstream>
#include <string>

namespace asterius
{

class Token
{
public:
	Token(int id);
	int id;
private:
};

class Word : Token
{
public:
	std::string m_data;
};

class Lexer final
{
public:
	explicit Lexer(const std::string& fileName);
	Token getNextToken();
private:
	std::ifstream m_reader;
};

}