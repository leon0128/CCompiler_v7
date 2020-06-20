#ifndef TP4_INTEGER_HPP
#define TP4_INTEGER_HPP

#include <cstdint>

namespace TP4
{

class Integer
{
public:
    enum class Tag
    {
        SIGNED,
        UNSIGNED
    } tag;
    
    union Uni
    {
        intmax_t i;
        uintmax_t u;
    } uni;

    Integer() noexcept:
        tag(Tag::SIGNED),
        uni{0}{}
};

}

#endif