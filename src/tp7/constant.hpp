#ifndef TP7_CONSTANT_HPP
#define TP7_CONSTANT_HPP

#include <vector>

namespace TP7
{

namespace TYPE
{
    class Type;
}

namespace CONSTANT
{

class Constant;

class Constant
{
public:
    union Uni
    {
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
        void *p;
    };
    std::vector<Constant*> agg;

    Constant()
        : uni{0}
        , agg()
        , type(nullptr){}

    bool isZero() const;

    bool inclusiveORExpression(const Constant*);

    Uni uni;
    Type *type;
};

}

}

#endif