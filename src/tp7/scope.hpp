#ifndef TP7_SCOPE_HPP
#define TP7_SCOPE_HPP

#include <unordered_map>
#include <utility>
#include <string>

namespace TP7
{

class Identifier;

class Scope
{
public:
    enum class ScopeTag
    {
        NONE
        , FILE
        , BLOCK
        , FUNCTION
        , PROTOTYPE
    };
    enum class NameSpaceTag
    {
        NONE
        , LABEL
        , TAG
        , MEMBER
        , OTHER
    };

    static void create(ScopeTag tag);
    static void destroy();
    
    template<class String>
    static Identifier *find(NameSpaceTag, String&&);
    static bool insert(NameSpaceTag, Identifier*, ScopeTag = ScopeTag::NONE);

private:
    static Scope *CHILD;

    Scope(Scope *parent
        , ScopeTag tag)
        : mParent(parent)
        , mTag(tag)
        , mMap()
    {
        mMap.emplace(NameSpaceTag::LABEL, std::unordered_map<std::string, Identifier*>());
        mMap.emplace(NameSpaceTag::MEMBER, std::unordered_map<std::string, Identifier*>());
        mMap.emplace(NameSpaceTag::TAG, std::unordered_map<std::string, Identifier*>());
        mMap.emplace(NameSpaceTag::OTHER, std::unordered_map<std::string, Identifier*>());
    }

    Scope *mParent;
    ScopeTag mTag;
    std::unordered_map<NameSpaceTag, std::unordered_map<std::string, Identifier*>> mMap;
};

template<class String>
Identifier *Scope::find(NameSpaceTag tag, String &&ident)
{
    Identifier *res = nullptr;

    for(Scope *scope = CHILD; scope != nullptr; scope = scope->mParent)
    {
        auto iter = scope->mMap.find(tag)->second.find(std::forward<String>(ident));
        if(iter != scope->mMap.find(tag)->second.end())
        {
            res = iter->second;
            break;
        }
    }

    return res;
}

}

#endif