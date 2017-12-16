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
    size_t insert(const std::string& name, size_t size);
    const size_t* find(const std::string& name) const noexcept;
	size_t block_size() const noexcept;
private:
    std::map<std::string, size_t> table_;
    size_t block_size_;
};


// table to search ids
class SymbolTable {
public:
    void push();
    void pop();
    size_t insert(const std::string& name, size_t size);
    size_t find(const std::string& name) const;
	size_t block_size() const noexcept;
private:
    std::vector<SymbolTableNode> tables_;
};


}

#endif // !SYMBOL_TABLE
