#ifndef STACK
#define STACK

#include <stack>
#include <cstring>
#include <exception>

namespace asterius
{

template<size_t size>
class Stack {
public:
    Stack()
        : top_(0),
        block_size_(0)
    {
        memset(buf_, 0, sizeof(buf_));
    }

    void push(size_t block_size)
    {
        //reserve memory for block_size bytes
        if (top_ + block_size_ + block_size >= size)
            throw bad_alloc();
        block_size_ += block_size;
    }

    void pop(size_t block_size)
    {
        //free memory for block_size bytes
        if (block_size > block_size_)
            throw out_of_range("Poped too big block");
        block_size_ -= block_size;
    }

    size_t top() const noexcept
    {
        //returns offset to the begining of new element 
        return top_ + block_size_;
    }

    void* get(int offset) 
    {
        // returns pointer to element by adding offset to current top
        // offset can be < 0
        if (offset >= (int)block_size_ || top_ + offset < 0)
            throw out_of_range("Access violation");
        return buf_ + top_ + offset;
    }

    void pushFrame()
    {
        //saves current frame
        top_ += block_size_;
        block_sizes_.push(block_size_);
        block_size_ = 0;
    }

    void popFrame()
    {
        //pop current frame
        block_size_ = block_sizes_.top();
        top_ -= block_size;
        block_sizes_.pop();
    }

private:
    size_t top_; //begining of current stack frame
    size_t block_size_; //size of current stack frame
    std::stack<size_t> block_sizes_; // stores previous block sizes
    char buf_[size];
};

}

#endif // !STACK
