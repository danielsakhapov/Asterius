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

    void stepBack();
    void getch();
    char peek();
    TokenType findKeyword(const std::string& name) const noexcept;
    size_t getCol(char c);

    //sem functions
    void sem0();
    void sem1();
    void sem2();
    void sem3();
    void sem4();
    void sem5();
    void sem6();
    void sem7();
    void sem8();
    void sem9();
    void sem10();
    void sem11();
    void sem12();
    void sem13();
    void sem14();
    void sem15();
    void sem16();
    void sem17();
    void sem18();
    void sem19();
    void sem20();
    void sem21();
    void sem22();
    void sem23();
    void sem24();

    void err1();
    void err2();
    void err3();
    void err4();

    //Private data
    std::ifstream reader_;
    size_t line_;
    size_t character_;
    Program& program_;

    //temp variables
    static std::map<std::string, TokenType> keywords_;
    std::string name_;
    size_t integer_;
    double double_;
    double pow_;
    Token token_;

    //tables
    static std::vector<std::vector<State> > tr;
    std::vector<std::vector<func> > sem; // 8 20
};

}

#endif // !LEXER