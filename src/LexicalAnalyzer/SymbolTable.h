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
    SymbolTableNode() noexcept;
    void insert(const std::string& name, Data& data);
    const Data* find(const std::string& name) const noexcept;
private:
    std::map<std::string, Data> table_;
    size_t block_size_;
};


// table to search ids
class SymbolTable {
public:
    void push();
    void pop();
    const Data* find(const std::string& name) const noexcept;
    void insert(const std::string& name, Data& data);
private:
    std::vector<SymbolTableNode> tables_;
};


}

#endif // !SYMBOL_TABLE
