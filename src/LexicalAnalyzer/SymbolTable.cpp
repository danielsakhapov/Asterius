#include "SymbolTable.h"

using namespace std;
using namespace asterius;

const Token& SymbolTableNode::insert(const Token& token)
{
    return (tokens_.emplace(token.getName(), token).first)->second; // token
}

const Token& SymbolTableNode::find(const string& name) const noexcept
{
    auto it = tokens_.find(name);
    if (it == tokens_.end())
        return Token::none;
    return it->second;
}

void SymbolTable::push()
{
    tables_.emplace_back();
}

void SymbolTable::pop()
{
    tables_.pop_back();
}

const Token& SymbolTable::find(const string& name) const noexcept
{
    for (auto it = tables_.crbegin(); it != tables_.crend(); ++it) {
        const auto& token = it->find(name);
        if (&token != &Token::none)
            return token;
    }
    return Token::none;
}

const Token& SymbolTable::insert(const Token& token)
{
    return tables_.back().insert(token);
}