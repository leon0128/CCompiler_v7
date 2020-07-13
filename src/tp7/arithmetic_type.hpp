#ifndef TP7_ARITHMETIC_TYPE_HPP
#define TP7_ARITHMETIC_TYPE_HPP

#include <cstdint>
#include <cfloat>

namespace TP7
{

class ArithmeticType
{
public:
    enum class Tag
    {
        NONE,
        BOOL,
        CHAR,
        S_CHAR,
        U_CHAR,
        S_SHORT,
        U_SHORT,
        S_INT,
        U_INT,
        S_LONG,
        U_LONG,
        S_LONG_LONG,
        U_LONG_LONG,
        FLOAT,
        DOUBLE,
        LONG_DOUBLE
    };
    union Uni
    {
        bool b;
        char c;
        signed char sc;
        unsigned char uc;
        signed short ss;
        unsigned short us;
        signed int si;
        unsigned int ui;
        signed long sl;
        unsigned long ul;
        signed long long sll;
        unsigned long long ull;
        float f;
        double d;
        long double ld;
    };

    constexpr ArithmeticType() noexcept:
        tag(Tag::NONE),
        uni{false}{}
    Tag tag;
    Uni uni;

    bool isNonzero() const;
    bool isInteger() const noexcept;
    bool isFloating() const noexcept;
    bool isArithmetic() const noexcept;
    ArithmeticType& cast(Tag);
    
    static Tag resultType(Tag lhs, Tag rhs);
};

ArithmeticType operator |(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator ^(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator &(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator ==(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator !=(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator <(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator >(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator <=(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator >=(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator <<(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator >>(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator +(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator -(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator *(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator /(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator %(const ArithmeticType&, const ArithmeticType&);
ArithmeticType operator +(const ArithmeticType&);
ArithmeticType operator -(const ArithmeticType&);
ArithmeticType operator ~(const ArithmeticType&);
ArithmeticType operator !(const ArithmeticType&);

}

#endif