#ifndef TP7_TYPE_HPP
#define TP7_TYPE_HPP

#include "../resource_controller.hpp"

namespace TP7
{

class Type : RESOURCE_CONTROLLER::Base
{
public:
    enum class Tag
    {
        NONE
        , VOID
        , CHAR
        , S_CHAR
        , U_CHAR
        , S_SHORT
        , U_SHORT
        , S_INT
        , U_INT
        , S_LONG
        , U_LONG
        , S_LONG_LONG
        , U_LONG_LONG
        , FLOAT
        , DOUBLE
        , LONG_DOUBLE
        , BOOL
        , FLOAT_COMPLEX
        , DOUBLE_COMPLEX
        , LONG_DOUBLE_COMPLEX
        , ATOMIC
        , STRUCT_OR_UNION
        , ENUM
        , TYPEDEF
    };
};

}

#endif