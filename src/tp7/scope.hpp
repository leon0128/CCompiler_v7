#ifndef TP7_SCOPE_HPP
#define TP7_SCORE_HPP

#include <unordered_map>

namespace TP7
{

class Anything;
class Scope;

class Scope
{
public:
    enum class KindTag;
    enum class NamespaceTag;

    explicit Scope(KindTag, Scope *parent = nullptr) noexcept;

    inline KindTag getTag() const noexcept {return mTag;}
    inline std::unordered_map<std::string, Anything*> &getMap(NamespaceTag tag) noexcept;

private:
    Scope *mParent;
    KindTag mTag;
    std::unordered_map<std::string, Anything*>
        mLabelMap,
        mTagMap,
        mMemberMap,
        mOrdinaryMap;
};

enum class Scope::KindTag
{
    FILE,
    FUNCTION,
    BLOCK,
    PROTOTYPE
};

enum class Scope::NamespaceTag
{
    LABEL,
    TAG,
    MEMBER,
    ORDINARY
};

Scope::Scope(KindTag tag, Scope *parent) noexcept:
    mParent(parent),
    mTag(tag),
    mLabelMap(),
    mTagMap(),
    mMemberMap(),
    mOrdinaryMap()
{
}

std::unordered_map<std::string, Anything*> &Scope::getMap(NamespaceTag tag) noexcept
{
    switch(tag)
    {
        case(NamespaceTag::LABEL):
            return mLabelMap;
        case(NamespaceTag::TAG):
            return mTagMap;
        case(NamespaceTag::MEMBER):
            return mMemberMap;
        case(NamespaceTag::ORDINARY):
            return mOrdinaryMap;
    }
}

}

#endif