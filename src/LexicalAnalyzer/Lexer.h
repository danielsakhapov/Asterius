#ifndef LEXER
#define LEXER

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include "TypeInfo.h"
#include "SymbolTable.h"
#include "Program/Program.h"
#include "BasicStructures.hpp"

namespace asterius
{
 
class Lexer final
{
public:
	typedef void(Lexer::*func)();
    explicit Lexer(const std::string& filename, Program& program);
    Token getNextToken();
    bool eof();
private:
	enum class State;

    void getch();
    char peek();
    TokenType findKeyword(const std::string& name) const noexcept;
    void unknown();
	size_t getCol(char c);

	//sem functions
	
	void sem1();
	void sem2();
	void sem3();
	void sem4();

	//Private data
    std::ifstream reader_;
    size_t line_;
    size_t character_;
    Program& program_;
	State state_;
	Token token_;
    static std::map<std::string, TokenType> keywords_;
	static std::vector<std::vector<State> > tr;
	std::vector<std::vector<func> > sem; // 8 20
};

}

#endif // !LEXER