#ifndef TP7_CONSTANT_HPP
#define TP7_CONSTANT_HPP

#include <complex>
#include "type.hpp"

namespace TP7
{

class Constant
{
public:
    union Uni
    {
        constexpr Uni() noexcept
            : c(0){}
        ~Uni() noexcept {}

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
        bool b;
        std::complex<float> cf;
        std::complex<double> cd;
        std::complex<long double> cld;
        unsigned char *p;
    };

    Constant()
        : type(new Type())
        , uni()
        , arr()
    {
        type->tag = Type::Tag::NONE;
    }

    Type *type;
    Uni uni;
    std::vector<Constant*> arr; // for aggregate type

    bool isZero() const;

    friend Constant operator |(const Constant&, const Constant&);
};

}

#endif