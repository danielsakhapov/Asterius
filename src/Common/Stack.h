#ifndef STACK
#define STACK

#include <vector>
#include <cassert>
#include <cstring>
#include "LexicalAnalyzer/TypeInfo.h"

namespace asterius 
{

template<size_t size_>
class Stack
{
public:
	Stack()
		: block_begin_(0),
		top_(0)
	{
		memset(buf_, 0, sizeof(buf_));
	}

	void addVariable(const Variable& variable) noexcept
	{
		top_ += variable.size();
	}

	void createVariable(Variable& variable) noexcept
	{
		variable.setOffset(top_ - block_begin_);
		addVariable(variable);
	}

	void removeVariable(const Variable& variable) noexcept
	{
		assert(top_ >= block_begin_ + variable.size());
		top_ -= variable.size();

	}

	void* getData(const Variable& variable) const noexcept
	{
		assert(variable.calc_offset(block_begin_) < top_);
		return (void*)(buf_ + variable.calc_offset(block_begin_));
	}

	void push()
	{
		block_begins_.push_back(block_begin_);
		block_begin_ = top_;
	}

	void pop()
	{
		assert(!block_begins_.empty());
		block_begin_ = block_begins_.back();
		block_begins_.pop_back();
	}
private:
	size_t block_begin_;
	size_t top_;
	std::vector<size_t> block_begins_;
	char buf_[size_];
};

}
#endif // !STACK
