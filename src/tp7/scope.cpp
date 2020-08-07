#include "scope.hpp"

namespace TP7
{

Scope *Scope::CHILD = nullptr;

void Scope::create(ScopeTag tag)
{
    CHILD = new Scope(CHILD, tag);
}

void Scope::destroy()
{
    Scope *parent = CHILD->mParent;
    delete CHILD;
    CHILD = parent;   
}

}