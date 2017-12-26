#include "Lexer.h"

using namespace std;
using namespace asterius;

enum class Lexer::State {
    START,
    WORD,
    INTEGER,
    DOUBLE,
    WAIT_COMMENT,
    COMMENT,
    STRING,
    FINISH
};

map<string, ElementType> Lexer::keywords_ = 
{
    { "not", ElementType::NOT },
    { "let", ElementType::LET },
    { "be", ElementType::BE },
    { "by", ElementType::BY },
    { "fn", ElementType::FN },
    { "set", ElementType::ASSIGN },
    { "array", ElementType::ARRAY },
    { "of", ElementType::OF },
    { "or", ElementType::OR },
    { "and", ElementType::AND },
    { "main", ElementType::MAIN },
    { "while", ElementType::WHILE },
    { "read",  ElementType::READ },
    { "write", ElementType::WRITE },
    { "if",    ElementType::IF },
    { "else", ElementType::ELSE },
    { "less", ElementType::LESS },
    { "greater", ElementType::GREATER },
    { "equal", ElementType::EQ },
    { "neq", ElementType::NEQ },
    { "geq", ElementType::GEQ },
    { "leq", ElementType::LEQ },
    { "int",   ElementType::INT },
    { "byte",   ElementType::BYTE },
    { "double", ElementType::DOUBLE },
    { "string", ElementType::STRING },
};

vector<vector<Lexer::State> > Lexer::tr = {
    /*1*/{ State::WORD, State::INTEGER, State::FINISH, State::FINISH, State::FINISH, State::WAIT_COMMENT, //6
    State::FINISH,  State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH, //12
    State::STRING, State::FINISH, State::DOUBLE, State::FINISH, State::START, State::START, State::FINISH},

    /*2*/{ State::WORD, State::WORD, State::FINISH, State::FINISH, State::FINISH, State::FINISH, //6
    State::FINISH,  State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH, //12
    State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH },

    /*3*/{ State::FINISH, State::INTEGER, State::FINISH, State::FINISH, State::FINISH, State::FINISH, //6
    State::FINISH,  State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH, //12
    State::FINISH, State::FINISH, State::DOUBLE, State::FINISH, State::FINISH, State::FINISH, State::FINISH },

    /*4*/{ State::FINISH, State::DOUBLE, State::FINISH, State::FINISH, State::FINISH, State::FINISH, //6
    State::FINISH,  State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH, //12
    State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH },

    /*5*/{ State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::COMMENT, //6
    State::FINISH,  State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH, //12
    State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH, State::FINISH },

    /*6*/{ State::COMMENT, State::COMMENT, State::COMMENT, State::COMMENT, State::COMMENT, State::COMMENT, //6
    State::COMMENT,  State::COMMENT, State::COMMENT, State::COMMENT, State::COMMENT, State::COMMENT, //12
    State::COMMENT, State::COMMENT, State::COMMENT, State::COMMENT, State::COMMENT, State::START, State::COMMENT },

    /*7*/{ State::STRING, State::STRING, State::STRING, State::STRING, State::STRING, State::STRING, //6
    State::STRING,  State::STRING, State::STRING, State::STRING, State::STRING, State::STRING, //12
    State::FINISH, State::STRING, State::STRING, State::STRING, State::STRING, State::FINISH, State::STRING }
};

Lexer::Lexer(const string& filename)
    : reader_(filename),
    sem {
        /*1*/{ &Lexer::sem1, &Lexer::sem2, &Lexer::sem3, &Lexer::sem4, &Lexer::sem5, &Lexer::sem0, //6
        &Lexer::sem6, &Lexer::sem7, &Lexer::sem8, &Lexer::sem9, &Lexer::sem10, &Lexer::sem11, //12
        &Lexer::sem12, &Lexer::sem13, &Lexer::sem14, &Lexer::sem15, &Lexer::sem0, &Lexer::sem0, &Lexer::err1 },

        /*2*/{ &Lexer::sem16, &Lexer::sem16, &Lexer::sem17, &Lexer::sem17, &Lexer::sem17, &Lexer::sem17, //6
        &Lexer::err1, &Lexer::err1, &Lexer::sem17, &Lexer::sem17, &Lexer::sem17, &Lexer::sem17, //12
        &Lexer::err1, &Lexer::sem17, &Lexer::err1, &Lexer::sem17, &Lexer::sem17, &Lexer::sem17, &Lexer::err1 },

        /*3*/{ &Lexer::err2, &Lexer::sem18, &Lexer::sem19, &Lexer::sem19, &Lexer::sem19, &Lexer::sem19, //6
        &Lexer::err1, &Lexer::err1, &Lexer::err1, &Lexer::sem19, &Lexer::err1, &Lexer::sem19, //12
        &Lexer::err1, &Lexer::sem19, &Lexer::sem20, &Lexer::sem19, &Lexer::sem19, &Lexer::sem19, &Lexer::err1 },

        /*4*/{ &Lexer::err2, &Lexer::sem21, &Lexer::sem22, &Lexer::sem22, &Lexer::sem22, &Lexer::sem22, //6
        &Lexer::err1, &Lexer::err1, &Lexer::err1, &Lexer::sem22, &Lexer::err1, &Lexer::err1, //12
        &Lexer::err1, &Lexer::sem22, &Lexer::err3, &Lexer::sem22, &Lexer::sem22, &Lexer::sem22, &Lexer::err1 },

        /*5*/{ &Lexer::sem23, &Lexer::sem23, &Lexer::err1, &Lexer::err1, &Lexer::err1, &Lexer::sem0, //6
        &Lexer::err1, &Lexer::err1, &Lexer::sem23, &Lexer::err1, &Lexer::err1, &Lexer::err1, //12
        &Lexer::err1, &Lexer::err1, &Lexer::sem23, &Lexer::err1, &Lexer::sem23, &Lexer::sem23, &Lexer::err1 },

        /*6*/{ &Lexer::sem0, &Lexer::sem0, &Lexer::sem0, &Lexer::sem0, &Lexer::sem0, &Lexer::sem0, //6
        &Lexer::sem0, &Lexer::sem0, &Lexer::sem0, &Lexer::sem0, &Lexer::sem0, &Lexer::sem0, //12
        &Lexer::sem0, &Lexer::sem0, &Lexer::sem0, &Lexer::sem0, &Lexer::sem0, &Lexer::sem0, &Lexer::sem0 },

        /*7*/{ &Lexer::sem16, &Lexer::sem16, &Lexer::sem16, &Lexer::sem16, &Lexer::sem16, &Lexer::sem16, //6
        &Lexer::sem16, &Lexer::sem16, &Lexer::sem16, &Lexer::sem16, &Lexer::sem16, &Lexer::sem16, //12
        &Lexer::sem24, &Lexer::sem16, &Lexer::sem16, &Lexer::sem16, &Lexer::sem16, &Lexer::err4, &Lexer::sem16 }
    }
{
}

Token Lexer::getNextToken()
{
    for (State state = State::START; state != State::FINISH;) {
        size_t col = getCol(peek());
        func fn = sem[(size_t)state][col];
        (this->*fn)();
        //invoke(fn, this);
        state = tr[(int)state][col];
        getch();
    }
    return token_;
}

bool Lexer::eof()
{
    return reader_.peek() == EOF;
}

void Lexer::stepBack()
{
    --position_;
    reader_.seekg(reader_.tellg() - streamoff(1));
}

void Lexer::getch()
{
    if (peek() == '\n')
        position_.newLine();
    else
        ++position_;
    reader_.get();
}

char Lexer::peek()
{
    return (char)reader_.peek();
}

ElementType Lexer::getElementType(const string& name) const noexcept
{
    auto it = keywords_.find(name);
    if (it != keywords_.end())
        return it->second;
    return ElementType::NAME; // suppose user name by default
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
    case '{':
        return 6;
    case '}':
        return 7;
    case '(':
        return 8;
    case ')':
        return 9;
    case '[':
        return 10;
    case ']':
        return 11;
    case '^':
        return 12;
    case ';':
        return 13;
    case '.':
        return 14;
    case ',':
        return 15;
    case '\t':
    case ' ':
        return 16;
    case '\n':
        return 17;
    default:
        return 18;
    }
}

void Lexer::sem0()
{ //do nothing
}

void Lexer::sem1()
{   //read first letter
    name_.clear();
    name_.push_back(peek());
}

void Lexer::sem2()
{   //read first digit
    integer_ = charToInt(peek());
}

void Lexer::sem3()
{
    token_ = Token(ElementType::PLUS, position_, "+");
}

void Lexer::sem4()
{
    token_ = Token(ElementType::MINUS, position_, "-");
}

void Lexer::sem5()
{
    token_ = Token(ElementType::PRODUCT, position_, "*");
}

void Lexer::sem6()
{
    token_ = Token(ElementType::OPEN_FIGURE, position_, "{");
}

void Lexer::sem7()
{
    token_ = Token(ElementType::CLOSE_FIGURE, position_, "}");
}

void Lexer::sem8()
{
    token_ = Token(ElementType::OPEN_BRACKET, position_, "(");
}

void Lexer::sem9()
{
    token_ = Token(ElementType::CLOSE_BRACKET, position_, ")");
}

void Lexer::sem10()
{
    token_ = Token(ElementType::OPEN_SQUARE, position_, "[");
}

void Lexer::sem11()
{
    token_ = Token(ElementType::CLOSE_SQUARE, position_, "]");
}

void Lexer::sem12()
{   //begining of const string;
    name_.clear();
}

void Lexer::sem13()
{
    token_ = Token(ElementType::STATEMENT_END, position_, ";");
}

void Lexer::sem14()
{   //read dot .123 is allowed
    double_ = 0;
    pow_ = 1;
}

void Lexer::sem15()
{
    token_ = Token(ElementType::COMMA, position_, ",");
}

void Lexer::sem16()
{
    //read another char
    name_ += peek();
}

void Lexer::sem17()
{
    //finished reading word
    stepBack(); //Met next symbol step back for cases like smth+smth
    token_ = Token(getElementType(name_), position_, name_);
}

void Lexer::sem18()
{   //read another digit of integer
    integer_ *= 10;
    integer_ += charToInt(peek());
}

void Lexer::sem19()
{   //finished reading integer
    stepBack(); //Met next symbol step back for cases like smth+smth
    token_ = Token(ElementType::INT_CONST, position_, std::to_string(integer_));
}

void Lexer::sem20()
{
    //Met dot after integer
    double_ = integer_;
    pow_ = 1;
}

void Lexer::sem21()
{
    //another digit of float
    pow_ /= 10;
    double_ += pow_ * charToInt(peek());
}

void Lexer::sem22()
{

    stepBack(); //Met next symbol step back for cases like smth+smth
    token_ = Token(ElementType::DOUBLE_CONST, position_, std::to_string(double_));
}

void Lexer::sem23()
{
    token_ = Token(ElementType::DIVISION, position_, "/");
}

void Lexer::sem24()
{   //end of const string
    token_ = Token(ElementType::STRING_CONST, position_, name_);
}

void Lexer::err1()
{
    stringstream ss;
    ss << "unexpected token at: " << to_string(position_) << " symbol:" << peek();
    throw logic_error(ss.str());
}

void Lexer::err2()
{
    stringstream ss;
    ss << "name starts with number at: " << to_string(position_);
    throw logic_error(ss.str());
}

void Lexer::err3()
{
    stringstream ss;
    ss << "already has dot at: " << to_string(position_);
    throw logic_error(ss.str());
}

void Lexer::err4()
{
    stringstream ss;
    ss << "^ expected at: " << to_string(position_);
    throw logic_error(ss.str());
}
