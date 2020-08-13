#ifndef TP7_CONSTANT_HPP
#define TP7_CONSTANT_HPP

#include <complex>
#include "../resource_controller.hpp"

namespace TP7
{

class Type;
class Constant : public ::RESOURCE_CONTROLLER::Base
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
        std::complex<float> cf;
        std::complex<double> cd;
        std::complex<long double> cld;

    };

    Constant()
        : Base()
        , type(nullptr)
        , uni{0}{}
    
    Type *type;
    Uni uni;
};

}

#endif