#ifndef TP7_IDENTIFIER_HPP
#define TP7_IDENTIFIER_HPP

#include <utility>
#include <string>

#include "../resource_controller.hpp"

namespace TP7
{

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

    template<class String>
    constexpr Identifier(Tag t
        , String &&ident)
        : Base()
        , tag(t)
        , identifier(std::forward<String>(ident)){}

    const Tag tag;
    const std::string identifier;
};

}

#endif