#include "Lexer.h"

using namespace std;
using namespace asterius;

Lexer::Lexer(const string& filename)
    : reader_(filename),
    line_(1),
    character_(1)
{
}

//Token Lexer::getNextToken()
//{
//    for ( ; ; getch()) {
//        switch (peek())
//        {// skip space, tab, new line symbols
//        case ' ':
//        case '\t': 
//            continue;
//        case '\n':
//            character_ = 0;
//            ++line_;
//            continue;
//        case '=':
//            if (match('='))
//                //test equal
//            //retutrn assign;
//        default:
//            break;
//        }
//        if (isDigit(peek())) { // integer
//            int num = charToInt(peek());
//            getch();
//            while (isDigit(peek())) {
//                num = num * 10 + charToInt(peek());
//                getch();
//            }
//            if (peek() != '.')
//                return unique_ptr<Token>(new Number(num));
//            getch(); // double
//            double dbl = num;
//            double tail = 0.1;
//            while (isDigit(peek())) {
//                dbl += tail * charToInt(peek());
//                getch();
//                tail /= 10;
//            }
//            return unique_ptr<Token>(new Double(dbl));
//        }
//        else if (isLetter(peek())) { // word
//            //TODO
//        }
//        //TODO 
//        return ;
//    }
//}

void Lexer::getch()
{
    ++character_;
    reader_.get();
}

char Lexer::peek()
{
    return (char)reader_.peek();
}

bool Lexer::match(char c)
{
    getch();
    return peek() == c;
}