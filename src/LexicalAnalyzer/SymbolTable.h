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
    void insert(const std::string& name, Variable& data);
    const Variable* find(const std::string& name) const noexcept;
	size_t block_size() const noexcept;
private:
    std::map<std::string, Variable> table_;
    size_t block_size_;
};


// table to search ids
class SymbolTable {
public:
    void push();
    void pop();
    void insert(const std::string& name, Variable& data);
    const Variable& find(const std::string& name) const;
	size_t block_size() const noexcept;
private:
    std::vector<SymbolTableNode> tables_;
};


}

#endif // !SYMBOL_TABLE
