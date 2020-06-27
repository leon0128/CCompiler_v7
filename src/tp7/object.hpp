#ifndef TP7_OBJECT_HPP
#define TP7_OBJECT_HPP

#include <string>
#include <utility>

#include "type.hpp"

namespace TP7
{

class Anything;

class Anything
{
public:
    enum class Tag
    {
        ANYTHING,
        OBJECT,
        ARRAY
    };

    template<class String>
    inline explicit constexpr Anything(String &&str, Tag tag = Tag::ANYTHING) noexcept:
        mIdentifier(std::forward<String>(str)),
        mTag(tag)
    {
    }
    virtual ~Anything() noexcept = 0;

    inline constexpr const std::string &getIdentifier() const noexcept {return mIdentifier;}
    inline constexpr Tag getTag() const noexcept {return mTag;}

private:
    std::string mIdentifier;
    Tag mTag;
};

Anything::~Anything() noexcept
{
}

class Object : public Anything
{
public:
    template<class String>
    inline constexpr Object(String &&ident) noexcept:
        Anything(std::forward<String>(ident)),
        declarationSpecifiers(){}

    DeclarationSpecifiers declarationSpecifiers;
};

}

#endif