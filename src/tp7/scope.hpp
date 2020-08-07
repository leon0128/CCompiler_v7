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

private:
    static Scope *CHILD;

    Scope(Scope *parent
        , ScopeTag tag)
        : mParent(parent)
        , mTag(tag)
        , mMap(){}

    Scope *mParent;
    ScopeTag mTag;
    std::unordered_map<NameSpaceTag, std::unordered_map<std::string, Identifier*>> mMap;
};

}

#endif