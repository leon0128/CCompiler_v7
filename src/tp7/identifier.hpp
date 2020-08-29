#ifndef TP7_IDENTIFIER_HPP
#define TP7_IDENTIFIER_HPP

#include <utility>
#include <string>

#include "../resource_controller.hpp"

namespace TP7
{

class Type;

class Identifier : RESOURCE_CONTROLLER::Base
{
public:
    enum class Tag
    {
        NONE
        , OBJECT
        , FUNCTION
        , TAG
        , MEMBER
        , ENUMERATION
        , TYPEDEF
        , LABEL
    };
    enum class StorageTag
    {
        NONE
        , EXTERNAL
        , INTERNAL
        , TH_EX
        , TH_ST
    };

    template<class String>
    Identifier(Tag t
        , String &&ident)
        : Base()
        , tag(t)
        , identifier(std::forward<String>(ident))
        , storageTag(StorageTag::NONE)
        , type(nullptr){}

    const Tag tag;
    const std::string identifier;
    StorageTag storageTag;
    Type *type;
};

}

#endif