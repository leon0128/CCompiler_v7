#ifndef TP7_IDENTIFIER_HPP
#define TP7_IDENTIFIER_HPP

#include <string>

namespace TP7
{

class Type;

namespace IDENTIFIER
{

class Object;

class Identifier
{
public:
    enum class Tag
    {
        NONE
        , OBJECT
    };
    union Uni
    {
        Object *object;
    };

    Identifier()
        : tag(Tag::NONE)
        , uni{nullptr}
        , ident(){}

    constexpr Object *set(Object *o) noexcept
        {tag = Tag::OBJECT; return uni.object = o;}

    Tag tag;
    Uni uni;
    std::string ident;
};

class Object
{
public:
    constexpr Object() noexcept
        : type(nullptr){}

    Type *type;
};

}

}

#endif