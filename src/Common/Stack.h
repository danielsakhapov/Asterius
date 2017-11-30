#ifndef STACK
#define STACK

#include <stack>
#include <cstring>
#include <exception>

#include "LexicalAnalyzer/TypeInfo.h"

namespace asterius
{

template<size_t size>
class Stack {
public:
    Stack()
        : top_(0)
    {
		block_ends_.push(0);
        memset(buf_, 0, sizeof(buf_));
    }

	void push(Data& data, void* src)
	{
		//reserve memory and fill from src
		this->push(data);
		memcpy(data.data_, src, sizeof(data.size_));
	}

    void push(Data& data)
    {
        //reserve memory for block_size bytes
        if (top_ + data.size_ >= size)
            throw std::bad_alloc();
		data.data_ = this->top();
		top_ += data.size_;
    }

    void pop(const Data& data)
    {
        //free memory for block_size bytes
        if (block_ends_.empty() || top_ - data.size_ < block_ends_.top())
            throw std::out_of_range("Poped too big block");
		top_ -= data.size_;
    }

    void pushFrame()
    {
        //saves current frame
		block_ends_.push(top_);
    }

	void* top() const noexcept
	{
		return (void*)(buf_ + top_);
	}

    void popFrame()
    {
        //pop current frame
		if (block_sizes_.empty())
			throw std::logic_error("pop from empty stack");
        top_ = block_ends_.top();
		block_ends_.pop();
    }

private:
    size_t top_;
    std::stack<size_t> block_ends_;
    char buf_[size];
};

}

#endif // !STACK
