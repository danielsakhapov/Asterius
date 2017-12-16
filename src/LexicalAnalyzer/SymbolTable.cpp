#include <cassert>
#include <sstream>
#include <exception>
#include "SymbolTable.h"

using namespace std;
using namespace asterius;

SymbolTableNode::SymbolTableNode() noexcept
    : block_size_(0)
{
}

size_t SymbolTableNode::insert(const string& name, size_t size)
{
    auto pr = table_.emplace(name, size);
    if (!pr.second)  { //already defined
        stringstream ss;
        ss << "name: " << name << " was already defined";
        throw logic_error(ss.str());
    }
	size_t ret = block_size_;
    block_size_ += size;
	return ret;
}

const size_t* SymbolTableNode::find(const string& name) const noexcept
{
    auto it = table_.find(name);
    if (it != table_.end())
        return &(it->second);
    return nullptr;
}

size_t SymbolTableNode::block_size() const noexcept
{
	return block_size_;
}

void SymbolTable::push()
{
    tables_.emplace_back();
}

void SymbolTable::pop()
{
	assert(!tables_.empty());
    tables_.pop_back();
}

size_t SymbolTable::insert(const string& name, size_t size)
{
	assert(!tables_.empty());
	return tables_.back().insert(name, size);
}

size_t SymbolTable::find(const string& name) const
{
	int shift = 0;
    for (auto it = tables_.crbegin(); it != tables_.crend(); ++it, shift += it->block_size()) {
        const auto offsetPtr = it->find(name); //search from top to bot
		if (offsetPtr)
			return *offsetPtr - shift;
    }
	stringstream ss;
	ss << "undeclared identifier: " << name;
	throw logic_error(ss.str());
}

size_t SymbolTable::block_size() const noexcept
{
	assert(!tables_.empty());
	return tables_.back().block_size();
}