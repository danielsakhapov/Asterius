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

void SymbolTableNode::insert(const string& name, Data& data)
{
    auto pr = table_.emplace(name, data);
    if (!pr.second)  { //already defined
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

void SymbolTable::insert(const string& name, Data& data)
{
	assert(!tables_.empty());
	if (tables_.size() > 1) // level 0 for globes
		data.setRelative(true);
	return tables_.back().insert(name, data);
}

const Data& SymbolTable::find(const string& name) const
{
    for (auto it = tables_.crbegin(); it != tables_.crend(); ++it) {
        const auto dataPtr = it->find(name); //search from top to bot
        if (dataPtr)
            return *dataPtr;
    }
	stringstream ss;
	ss << "undeclared identifier " << name;
	throw logic_error(ss.str());
}

size_t SymbolTable::block_size() const noexcept
{
	assert(!tables_.empty());
	return tables_.back().block_size();
}