#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include <map>
#include <vector>
#include <string>
#include "TypeInfo.h"

namespace asterius
{

class SymbolTableNode 
{
public:
    const Token& insert(const Token& token);
    const Token& find(const std::string& name) const noexcept;
private:
    std::map<std::string, Token> tokens_; //c++17 string_view
};


// table to search ids
class SymbolTable {
public:
    void push();
    void pop();
    const Token& find(const std::string& token) const noexcept;
    const Token& insert(const Token& token);
private:
    std::vector<SymbolTableNode> tables_;
};


}

#endif // !SYMBOL_TABLE
