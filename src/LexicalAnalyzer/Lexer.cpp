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

map<string, TokenType> Lexer::keywords_ = 
{
    { "while", TokenType::WHILE },
    { "if",    TokenType::IF },
    { "int",   TokenType::INT },
    { "double", TokenType::DOUBLE },
    { "read",  TokenType::READ },
    { "write", TokenType::WRITE },
    { "set", TokenType::ASSIGN }
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

Lexer::Lexer(const string& filename, Program& program)
    : reader_(filename),
    line_(1),
    character_(1),
    program_(program),
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
        state = tr[(size_t)state][col];
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
    --character_;
    reader_.seekg(reader_.tellg() - streamoff(1));
}

void Lexer::getch()
{
    if (peek() == '\n') {
        ++line_;
        character_ = 1;
    }
    else
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
{
    name_.clear();
    name_.push_back(peek());
}

void Lexer::sem2()
{
    integer_ = charToInt(peek());
}

void Lexer::sem3()
{
    token_ = Token(TokenType::PLUS, "+");
}

void Lexer::sem4()
{
    token_ = Token(TokenType::MINUS, "-");
}

void Lexer::sem5()
{
    token_ = Token(TokenType::PRODUCT, "*");
}

void Lexer::sem6()
{
    token_ = Token(TokenType::OPEN_FIGURE, "{");
}

void Lexer::sem7()
{
    token_ = Token(TokenType::CLOSE_FIGURE, "}");
}

void Lexer::sem8()
{
    token_ = Token(TokenType::OPEN_BRACKET, "(");
}

void Lexer::sem9()
{
    token_ = Token(TokenType::CLOSE_BRACKET, ")");
}

void Lexer::sem10()
{
    token_ = Token(TokenType::OPEN_SQUARE, "[");
}

void Lexer::sem11()
{
    token_ = Token(TokenType::CLOSE_SQUARE, "]");
}

void Lexer::sem12()
{
    name_.clear();
}

void Lexer::sem13()
{
    token_ = Token(TokenType::STATEMENT_END, ";");
}

void Lexer::sem14()
{
    double_ = 0;
    pow_ = 1;
}

void Lexer::sem15()
{
    token_ = Token(TokenType::COMMA, ",");
}

void Lexer::sem16()
{
    name_ += peek();
}

void Lexer::sem17()
{
    stepBack();
    auto it = keywords_.find(name_);
    if (it == keywords_.end())
        token_ = Token(TokenType::ID, name_);
    else
        token_ = Token(it->second, name_);
}

void Lexer::sem18()
{
    integer_ *= 10;
    integer_ += charToInt(peek());
}

void Lexer::sem19()
{
    stepBack();
    token_ = Token(TokenType::INT, to_string(integer_));
}

void Lexer::sem20()
{
    double_ = integer_;
    pow_ = 1;
}

void Lexer::sem21()
{
    pow_ /= 10;
    double_ += pow_ * charToInt(peek());
}

void Lexer::sem22()
{
    stepBack();
    token_ = Token(TokenType::DOUBLE, to_string(double_));
}

void Lexer::sem23()
{
    token_ = Token(TokenType::DIVISION, "/");
}

void Lexer::sem24()
{
    token_ = Token(TokenType::STRING, name_);
}

void Lexer::err1()
{
    stringstream ss;
    ss << "unexpected token at: " << line_ << ':' << character_ << " symbol:" << peek();
    throw logic_error(ss.str());
}

void Lexer::err2()
{
    stringstream ss;
    ss << "name starts with number at: " << line_ << ':' << character_;
    throw logic_error(ss.str());
}

void Lexer::err3()
{
    stringstream ss;
    ss << "already has dot at: " << line_ << ':' << character_;
    throw logic_error(ss.str());
}

void Lexer::err4()
{
    stringstream ss;
    ss << "^ expected at: " << line_ << ':' << character_;
    throw logic_error(ss.str());
}
