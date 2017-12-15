#include <sstream>
#include <exception>
#include "SymbolTable.h"

using namespace std;
using namespace asterius;

SymbolTableNode::SymbolTableNode() noexcept
    : block_size_(0)
{
}

void SymbolTableNode::insert(const string& name, Data& data)
{
    auto pr = table_.emplace(name, data);
    if (!pr.second)  {
        stringstream ss;
        ss << "name: " << name << " was already defined at: " << to_string(data.position());
        throw logic_error(ss.str());
    }
    data.setOffset(block_size_);
    block_size_ += data.size();
}

const Data* SymbolTableNode::find(const string& name) const noexcept
{
    auto it = table_.find(name);
    if (it != table_.end())
        return &(it->second);
    return nullptr;
}

void SymbolTable::push()
{
    tables_.emplace_back();
}

void SymbolTable::pop()
{
    tables_.pop_back();
}

const Data* SymbolTable::find(const string& name) const noexcept
{
    for (auto it = tables_.crbegin(); it != tables_.crend(); ++it) {
        const auto& dataPtr = it->find(name);
        if (dataPtr)
            return dataPtr;
    }
    return nullptr;
}

void SymbolTable::insert(const string& name, Data& data)
{
    return tables_.back().insert(name, data);
}