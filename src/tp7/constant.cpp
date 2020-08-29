#include <iostream>
#include <exception>
#include "type.hpp"
#include "constant.hpp"

namespace TP7
{

bool Constant::isZero() const
{
    bool ret = false;

    switch(type->tag)
    {
        case(Type::Tag::NONE):
            ret = false;
            break;
        case(Type::Tag::BASE):
            switch(type->uni.bt->tag)
            {
                case(BaseType::Tag::NONE):
                    ret = false;
                    break;
                case(BaseType::Tag::VOID):
                    ret = false;
                    break;
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
                    ret = uni.b == false;
                    break;
                case(BaseType::Tag::FLOAT_COMPLEX):
                    ret = uni.cf == 0.0f;
                    break;
                case(BaseType::Tag::DOUBLE_COMPLEX):
                    ret = uni.cd == 0.0;
                    break;
                case(BaseType::Tag::LONG_DOUBLE_COMPLEX):
                    ret = uni.cld == 0.0l;
                    break;
            }
            break;
        case(Type::Tag::STRUCT_OR_UNION):
            ret = false;
            break;
        case(Type::Tag::ENUMERATION):
            ret = uni.si == 0;
            break;
        case(Type::Tag::TYPEDEF):
        {
            Constant temp(*this);
            temp.type = type->uni.tt->type;
            ret = temp.isZero();
            break;
        }
        case(Type::Tag::ARRAY):
            ret = uni.p == static_cast<unsigned char*>(0);
            break;
        case(Type::Tag::FUNCTION):
            ret = false;
            break;
    }

    return ret;
}

}