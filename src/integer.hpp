#ifndef INTEGER_HPP
#define INTEGER_HPP

#include <cstdint>

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

#endif