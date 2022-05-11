#include "identifier.hpp"
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

bool Scope::insert(NameSpaceTag nsTag, IDENTIFIER::Identifier *ident, ScopeTag sTag)
{
    Scope *scope = CHILD;
    if(sTag != ScopeTag::NONE)
    {
        for(; scope != nullptr; scope = scope->mParent)
        {
            if(scope->mTag == sTag)
                break;
        }
    }

    if(scope != nullptr)
        return scope->mMap.find(nsTag)->second.insert(std::make_pair(ident->ident, ident)).second;
    else
        return false;
}

}