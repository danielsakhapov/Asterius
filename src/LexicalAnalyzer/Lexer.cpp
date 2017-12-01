#include "Lexer.h"

using namespace std;
using namespace asterius;

enum class Lexer::State {
	START,
	WORD,
	INTEGER,
	FLOAT,
	ASSIGN,
	WAIT_COMMENT,
	COMMENT,
	STRING,
	FINISH
};

map<string, TokenType> Lexer::keywords_ = 
{
    { "while", TokenType::WHILE },
    { "if",    TokenType::IF },
    { "int",   TokenType::INT },
    { "float", TokenType::FLOAT },
    { "read",  TokenType::READ },
    { "write", TokenType::WRITE }
};

vector<vector<Lexer::State> > Lexer::tr = {
	{State::START},
	{State::START}//...
};

Lexer::Lexer(const string& filename, Program& program)
    : reader_(filename),
    line_(1),
    character_(1),
    program_(program),
	token_(TokenType::NONE),
	sem {
		{ &Lexer::sem1, &Lexer::sem2, &Lexer::sem3 },
		{ &Lexer::sem1, &Lexer::sem2, &Lexer::sem3 }//...
	}
{
}

Token Lexer::getNextToken()
{
	for (state_ = State::START; state_ != State::FINISH;) {
		size_t col = getCol(peek());
		func fn = sem[(size_t)state_][col];
		invoke(fn, this);
		getch();
		state_ = tr[(size_t)state_][col];
	}
	return token_;
}

bool Lexer::eof()
{
    return reader_.peek() == EOF;
}

void Lexer::getch()
{
    ++character_;
    reader_.get();
}

char Lexer::peek()
{
    return (char)reader_.peek();
}

TokenType Lexer::findKeyword(const string& name) const noexcept
{
    auto it = keywords_.find(name);
    if (it != keywords_.end())
        return it->second;
    return TokenType::NONE;
}

void Lexer::unknown()
{
    stringstream tmp;
    tmp << "Unknown symbol at line " << line_ << " character " << character_;
    throw logic_error(tmp.str());
}

size_t Lexer::getCol(char c)
{
	if (isLetter(c))
		return 0;
	if (isDigit(c))
		return 1;
	switch (c)
	{
	case '+':
		return 2;
	case '-':
		return 3;
	case '*':
		return 4;
	case '/':
		return 5;
	case '=':
		return 6;
	case '{':
		return 7;
	case '}':
		return 8;
	case '(':
		return 9;
	case ')':
		return 10;
	case '[':
		return 11;
	case ']':
		return 12;
	case '^':
		return 13;
	case ';':
		return 14;
	case ':':
		return 15;
	case '.':
		return 16;
	case ' ':
		return 17;
	case '\n':
		return 18;
	default:
		return 19;
	}
}

void Lexer::sem1()
{

}

void Lexer::sem2()
{

}

void Lexer::sem3()
{

}

void Lexer::sem4()
{

}