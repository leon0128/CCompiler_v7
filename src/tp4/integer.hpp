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
        std::intmax_t i;
        std::uintmax_t u;
    } uni;

    constexpr Integer() noexcept:
        tag(Tag::SIGNED),
        uni{0}{}

    inline constexpr Integer operator +() noexcept {return *this;}
    inline constexpr Integer operator -() noexcept
    {
        Integer retval;
        if(tag == Tag::SIGNED)
            retval.uni.i = -uni.i;
        else
            retval.uni.u = -uni.u;
        return retval;
    }
    inline constexpr Integer operator ~() noexcept
    {
        Integer retval;
        if(tag == Tag::SIGNED)
            retval.uni.i = ~uni.i;
        else
            retval.uni.u = ~uni.u;
        return retval;
    }
    inline constexpr Integer operator !() noexcept
    {
        Integer retval;
        if(tag == Tag::SIGNED)
            retval.uni.i = !uni.i;
        else
            retval.uni.u = !uni.u;
        return retval;
    }
};

#define TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(ope) \
inline constexpr TP4::Integer operator ope(const TP4::Integer &lhs, const TP4::Integer &rhs) noexcept\
{\
    TP4::Integer retval;\
\
    if(lhs.tag == TP4::Integer::Tag::SIGNED)\
    {\
        if(rhs.tag == TP4::Integer::Tag::SIGNED)\
        {\
            retval.tag = TP4::Integer::Tag::SIGNED;\
            retval.uni.i = lhs.uni.i ope rhs.uni.i;\
        }\
        else\
        {\
            retval.tag = TP4::Integer::Tag::UNSIGNED;\
            retval.uni.u = static_cast<std::uintmax_t>(lhs.uni.i) ope lhs.uni.u;\
        }\
    }\
    else\
    {\
        if(rhs.tag == TP4::Integer::Tag::SIGNED)\
        {\
            retval.tag = TP4::Integer::Tag::UNSIGNED;\
            retval.uni.u = lhs.uni.u ope static_cast<std::uintmax_t>(rhs.uni.i);\
        }\
        else\
        {\
            retval.tag = TP4::Integer::Tag::UNSIGNED;\
            retval.uni.u = lhs.uni.u || rhs.uni.u;\
        }\
    }\
\
    return retval;\
}

// operator function
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(||)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(&&)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(|)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(^)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(&)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(==)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(!=)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(<)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(>)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(<=)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(>=)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(<<)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(>>)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(+)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(-)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(*)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(/)
TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION(%)

}

#undef TP4_INTEGER_OPERATOR_FUNCTION_DEFINITION

#endif