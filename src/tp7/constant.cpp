#include "type.hpp"
#include "constant.hpp"

namespace TP7
{

namespace CONSTANT
{

bool Constant::isZero() const
{
    bool ret = true;

    switch(type->tag)
    {
        case(Type::Tag::BASE):
        {
            switch(type->uni.baseT->tag)
            {
                case(BaseType::Tag::CHAR):
                    ret = uni.c == static_cast<char>(0);
                    break;
                case(BaseType::Tag::S_CHAR):
                    ret = uni.sc == static_cast<signed char>(0);
                    break;
                case(BaseType::Tag::U_CHAR):
                    ret = uni.uc == static_cast<unsigned char>(0);
                    break;
                case(BaseType::Tag::S_SHORT):
                    ret = uni.ss == static_cast<signed short>(0);
                    break;
                case(BaseType::Tag::U_SHORT):
                    ret = uni.us == static_cast<unsigned short>(0);
                    break;
                case(BaseType::Tag::S_INT):
                    ret = uni.si == 0;
                    break;
                case(BaseType::Tag::U_INT):
                    ret = uni.ui == 0u;
                    break;
                case(BaseType::Tag::S_LONG):
                    ret = uni.sl == 0l;
                    break;
                case(BaseType::Tag::U_LONG):
                    ret = uni.ul == 0ul;
                    break;
                case(BaseType::Tag::S_LONG_LONG):
                    ret = uni.sll == 0ll;
                    break;
                case(BaseType::Tag::U_LONG_LONG):
                    ret = uni.ull == 0ull;
                    break;
                case(BaseType::Tag::FLOAT):
                    ret = uni.f == 0.0f;
                    break;
                case(BaseType::Tag::DOUBLE):
                    ret = uni.d == 0.0;
                    break;
                case(BaseType::Tag::LONG_DOUBLE):
                    ret = uni.ld == 0.0l;
                    break;
                case(BaseType::Tag::BOOL):
                    ret = !uni.b;
                    break;

                default:
                    ret = false;
                    Simbol::unexpectTag("TP7::Constant");
            }
            break;
        }
        case(Type::Tag::FUNCTION):
            ret = false;
            break;
        case(Type::Tag::POINTER):
            ret = uni.p == nullptr;
            break;
        
        default:
            ret = false;
            Simbol::unexpectTag("Type");
    }

    return ret;
}



}

}