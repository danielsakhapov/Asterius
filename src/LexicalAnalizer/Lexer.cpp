#include "Lexer.h"
#include <string>

using namespace std;
using namespace asterius;

Token::Token(int id) : id(id)
{

}

Lexer::Lexer(const string& filename) : m_reader(filename, ios::in)
{
}

Token Lexer::getNextToken()
{
	int c = m_reader.get();
	return Token(c);
}