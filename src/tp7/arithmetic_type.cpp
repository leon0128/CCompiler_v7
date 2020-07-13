#include <stdexcept>
#include "arithmetic_type.hpp"

namespace TP7
{

bool ArithmeticType::isNonzero() const
{
    bool retval = false;

    switch(tag)
    {
        case(Tag::BOOL):
            retval = uni.b;
            break;
        case(Tag::CHAR):
            retval = static_cast<char>(0) != uni.c;
            break;
        case(Tag::S_CHAR):
            retval = static_cast<signed char>(0) != uni.sc;
            break;
        case(Tag::U_CHAR):
            retval = static_cast<unsigned char>(0) != uni.uc;
            break;
        case(Tag::S_SHORT):
            retval = static_cast<signed short>(0) != uni.ss;
            break;
        case(Tag::U_SHORT):
            retval = static_cast<unsigned short>(0) != uni.us;
            break;
        case(Tag::S_INT):
            retval = static_cast<signed int>(0) != uni.si;
            break;
        case(Tag::U_INT):
            retval = static_cast<unsigned int>(0) != uni.ui;
            break;
        case(Tag::S_LONG):
            retval = static_cast<signed long>(0) != uni.sl;
            break;
        case(Tag::U_LONG):
            retval = static_cast<unsigned long>(0) != uni.ul;
            break;
        case(Tag::S_LONG_LONG):
            retval = static_cast<signed long long>(0) != uni.sll;
            break;
        case(Tag::U_LONG_LONG):
            retval = static_cast<unsigned long long>(0) != uni.ull;
            break;
        case(Tag::FLOAT):
            retval = static_cast<float>(0) != uni.f;
            break;
        case(Tag::DOUBLE):
            retval = static_cast<double>(0) != uni.d;
            break;
        case(Tag::LONG_DOUBLE):
            retval = static_cast<long double>(0) != uni.ld;
            break;
        
        default:
            throw std::runtime_error("unexpected tag");
    }

    return retval;
}

bool ArithmeticType::isInteger() const noexcept
{
    if(tag == Tag::BOOL ||
        tag == Tag::CHAR ||
        tag == Tag::S_CHAR ||
        tag == Tag::U_CHAR ||
        tag == Tag::S_SHORT ||
        tag == Tag::U_SHORT ||
        tag == Tag::S_INT ||
        tag == Tag::U_INT ||
        tag == Tag::S_LONG ||
        tag == Tag::U_LONG ||
        tag == Tag::S_LONG_LONG ||
        tag == Tag::U_LONG_LONG)
        return true;
    else
        return false;
}

bool ArithmeticType::isFloating() const noexcept
{
    if(tag == Tag::FLOAT ||
        tag == Tag::DOUBLE ||
        tag == Tag::LONG_DOUBLE)
        return true;
    else
        return false;
}

bool ArithmeticType::isArithmetic() const noexcept
{
    return isInteger() || isFloating();
}

#define CAST_EXPRESSION_MACRO(member, type) \
    switch(tag) \
    { \
        case(Tag::BOOL): \
            uni.member = static_cast<type>(uni.b); break; \
        case(Tag::CHAR): \
            uni.member = static_cast<type>(uni.c); break; \
        case(Tag::S_CHAR): \
            uni.member = static_cast<type>(uni.sc); break; \
        case(Tag::U_CHAR): \
            uni.member = static_cast<type>(uni.uc); break; \
        case(Tag::S_SHORT): \
            uni.member = static_cast<type>(uni.ss); break; \
        case(Tag::U_SHORT): \
            uni.member = static_cast<type>(uni.us); break; \
        case(Tag::S_INT): \
            uni.member = static_cast<type>(uni.si); break; \
        case(Tag::U_INT): \
            uni.member = static_cast<type>(uni.ui); break; \
        case(Tag::S_LONG): \
            uni.member = static_cast<type>(uni.sl); break; \
        case(Tag::U_LONG): \
            uni.member = static_cast<type>(uni.ul); break; \
        case(Tag::S_LONG_LONG): \
            uni.member = static_cast<type>(uni.sll); break; \
        case(Tag::U_LONG_LONG): \
            uni.member = static_cast<type>(uni.ull); break; \
        case(Tag::FLOAT): \
            uni.member = static_cast<type>(uni.f); break; \
        case(Tag::DOUBLE): \
            uni.member = static_cast<type>(uni.d); break; \
        case(Tag::LONG_DOUBLE): \
            uni.member = static_cast<type>(uni.ld); break; \
\
        default: \
            throw std::runtime_error("unexpected tag"); \
    }

ArithmeticType& ArithmeticType::cast(Tag castTag)
{
    switch(castTag)
    {
        case(Tag::BOOL):
            CAST_EXPRESSION_MACRO(b, bool) break;
        case(Tag::CHAR):
            CAST_EXPRESSION_MACRO(c, char) break;
        case(Tag::S_CHAR):
            CAST_EXPRESSION_MACRO(sc, signed char) break;
        case(Tag::U_CHAR):
            CAST_EXPRESSION_MACRO(uc, unsigned char) break;
        case(Tag::S_SHORT):
            CAST_EXPRESSION_MACRO(ss, signed short) break;
        case(Tag::U_SHORT):
            CAST_EXPRESSION_MACRO(us, unsigned short) break;
        case(Tag::S_INT):
            CAST_EXPRESSION_MACRO(si, signed int) break;
        case(Tag::U_INT):
            CAST_EXPRESSION_MACRO(ui, unsigned int) break;
        case(Tag::S_LONG):
            CAST_EXPRESSION_MACRO(sl, signed long) break;
        case(Tag::U_LONG):
            CAST_EXPRESSION_MACRO(ul, unsigned long) break;
        case(Tag::S_LONG_LONG):
            CAST_EXPRESSION_MACRO(sll, signed long long) break;
        case(Tag::U_LONG_LONG):
            CAST_EXPRESSION_MACRO(ull, unsigned long long) break;
        case(Tag::FLOAT):
            CAST_EXPRESSION_MACRO(f, float) break;
        case(Tag::DOUBLE):
            CAST_EXPRESSION_MACRO(d, double) break;
        case(Tag::LONG_DOUBLE):
            CAST_EXPRESSION_MACRO(ld, long double) break;
        
        default:
            throw std::runtime_error("unexpected tag");
    }

    return *this;
}

#undef CAST_EXPRESSION_MACRO

ArithmeticType::Tag ArithmeticType::resultType(ArithmeticType::Tag lhs, ArithmeticType::Tag rhs)
{
    if(lhs == Tag::NONE ||
        rhs == Tag::NONE)
        return Tag::NONE;
    
    Tag retval = Tag::NONE;

    switch(lhs)
    {
        case(Tag::BOOL):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::S_INT; break;
                case(Tag::CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::S_CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::U_CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::S_SHORT):
                    retval = Tag::S_INT; break;
                case(Tag::U_SHORT):
                    retval = Tag::S_INT; break;
                case(Tag::S_INT):
                    retval = Tag::S_INT; break;
                case(Tag::U_INT):
                    retval = Tag::U_INT; break;
                case(Tag::S_LONG):
                    retval = Tag::S_LONG; break;
                case(Tag::U_LONG):
                    retval = Tag::U_LONG; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::FLOAT):
                    retval = Tag::FLOAT; break;
                case(Tag::DOUBLE):
                    retval = Tag::DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;
        case(Tag::CHAR):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::S_INT; break;
                case(Tag::CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::S_CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::U_CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::S_SHORT):
                    retval = Tag::S_INT; break;
                case(Tag::U_SHORT):
                    retval = Tag::S_INT; break;
                case(Tag::S_INT):
                    retval = Tag::S_INT; break;
                case(Tag::U_INT):
                    retval = Tag::U_INT; break;
                case(Tag::S_LONG):
                    retval = Tag::S_LONG; break;
                case(Tag::U_LONG):
                    retval = Tag::U_LONG; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::FLOAT):
                    retval = Tag::FLOAT; break;
                case(Tag::DOUBLE):
                    retval = Tag::DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;
        case(Tag::S_CHAR):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::S_INT; break;
                case(Tag::CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::S_CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::U_CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::S_SHORT):
                    retval = Tag::S_INT; break;
                case(Tag::U_SHORT):
                    retval = Tag::S_INT; break;
                case(Tag::S_INT):
                    retval = Tag::S_INT; break;
                case(Tag::U_INT):
                    retval = Tag::U_INT; break;
                case(Tag::S_LONG):
                    retval = Tag::S_LONG; break;
                case(Tag::U_LONG):
                    retval = Tag::U_LONG; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::FLOAT):
                    retval = Tag::FLOAT; break;
                case(Tag::DOUBLE):
                    retval = Tag::DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;
        case(Tag::U_CHAR):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::S_INT; break;
                case(Tag::CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::S_CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::U_CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::S_SHORT):
                    retval = Tag::S_INT; break;
                case(Tag::U_SHORT):
                    retval = Tag::S_INT; break;
                case(Tag::S_INT):
                    retval = Tag::S_INT; break;
                case(Tag::U_INT):
                    retval = Tag::U_INT; break;
                case(Tag::S_LONG):
                    retval = Tag::S_LONG; break;
                case(Tag::U_LONG):
                    retval = Tag::U_LONG; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::FLOAT):
                    retval = Tag::FLOAT; break;
                case(Tag::DOUBLE):
                    retval = Tag::DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;
        case(Tag::S_SHORT):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::S_INT; break;
                case(Tag::CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::S_CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::U_CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::S_SHORT):
                    retval = Tag::S_INT; break;
                case(Tag::U_SHORT):
                    retval = Tag::S_INT; break;
                case(Tag::S_INT):
                    retval = Tag::S_INT; break;
                case(Tag::U_INT):
                    retval = Tag::U_INT; break;
                case(Tag::S_LONG):
                    retval = Tag::S_LONG; break;
                case(Tag::U_LONG):
                    retval = Tag::U_LONG; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::FLOAT):
                    retval = Tag::FLOAT; break;
                case(Tag::DOUBLE):
                    retval = Tag::DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;
        case(Tag::U_SHORT):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::S_INT; break;
                case(Tag::CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::S_CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::U_CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::S_SHORT):
                    retval = Tag::S_INT; break;
                case(Tag::U_SHORT):
                    retval = Tag::S_INT; break;
                case(Tag::S_INT):
                    retval = Tag::S_INT; break;
                case(Tag::U_INT):
                    retval = Tag::U_INT; break;
                case(Tag::S_LONG):
                    retval = Tag::S_LONG; break;
                case(Tag::U_LONG):
                    retval = Tag::U_LONG; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::FLOAT):
                    retval = Tag::FLOAT; break;
                case(Tag::DOUBLE):
                    retval = Tag::DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;
        case(Tag::S_INT):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::S_INT; break;
                case(Tag::CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::S_CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::U_CHAR):
                    retval = Tag::S_INT; break;
                case(Tag::S_SHORT):
                    retval = Tag::S_INT; break;
                case(Tag::U_SHORT):
                    retval = Tag::S_INT; break;
                case(Tag::S_INT):
                    retval = Tag::S_INT; break;
                case(Tag::U_INT):
                    retval = Tag::U_INT; break;
                case(Tag::S_LONG):
                    retval = Tag::S_LONG; break;
                case(Tag::U_LONG):
                    retval = Tag::U_LONG; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::FLOAT):
                    retval = Tag::FLOAT; break;
                case(Tag::DOUBLE):
                    retval = Tag::DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;
        case(Tag::U_INT):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::U_INT; break;
                case(Tag::CHAR):
                    retval = Tag::U_INT; break;
                case(Tag::S_CHAR):
                    retval = Tag::U_INT; break;
                case(Tag::U_CHAR):
                    retval = Tag::U_INT; break;
                case(Tag::S_SHORT):
                    retval = Tag::U_INT; break;
                case(Tag::U_SHORT):
                    retval = Tag::U_INT; break;
                case(Tag::S_INT):
                    retval = Tag::U_INT; break;
                case(Tag::U_INT):
                    retval = Tag::U_INT; break;
                case(Tag::S_LONG):
                    retval = Tag::S_LONG; break;
                case(Tag::U_LONG):
                    retval = Tag::U_LONG; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::FLOAT):
                    retval = Tag::FLOAT; break;
                case(Tag::DOUBLE):
                    retval = Tag::DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;
        case(Tag::S_LONG):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::S_LONG; break;
                case(Tag::CHAR):
                    retval = Tag::S_LONG; break;
                case(Tag::S_CHAR):
                    retval = Tag::S_LONG; break;
                case(Tag::U_CHAR):
                    retval = Tag::S_LONG; break;
                case(Tag::S_SHORT):
                    retval = Tag::S_LONG; break;
                case(Tag::U_SHORT):
                    retval = Tag::S_LONG; break;
                case(Tag::S_INT):
                    retval = Tag::S_LONG; break;
                case(Tag::U_INT):
                    retval = Tag::S_LONG; break;
                case(Tag::S_LONG):
                    retval = Tag::S_LONG; break;
                case(Tag::U_LONG):
                    retval = Tag::U_LONG; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::FLOAT):
                    retval = Tag::FLOAT; break;
                case(Tag::DOUBLE):
                    retval = Tag::DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;        
        case(Tag::U_LONG):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::U_LONG; break;
                case(Tag::CHAR):
                    retval = Tag::U_LONG; break;
                case(Tag::S_CHAR):
                    retval = Tag::U_LONG; break;
                case(Tag::U_CHAR):
                    retval = Tag::U_LONG; break;
                case(Tag::S_SHORT):
                    retval = Tag::U_LONG; break;
                case(Tag::U_SHORT):
                    retval = Tag::U_LONG; break;
                case(Tag::S_INT):
                    retval = Tag::U_LONG; break;
                case(Tag::U_INT):
                    retval = Tag::U_LONG; break;
                case(Tag::S_LONG):
                    retval = Tag::U_LONG; break;
                case(Tag::U_LONG):
                    retval = Tag::U_LONG; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::FLOAT):
                    retval = Tag::FLOAT; break;
                case(Tag::DOUBLE):
                    retval = Tag::DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;
        case(Tag::S_LONG_LONG):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::CHAR):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::S_CHAR):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_CHAR):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::S_SHORT):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_SHORT):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::S_INT):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_INT):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::S_LONG):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_LONG):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::S_LONG_LONG; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::FLOAT):
                    retval = Tag::FLOAT; break;
                case(Tag::DOUBLE):
                    retval = Tag::DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;
        case(Tag::U_LONG_LONG):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::CHAR):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::S_CHAR):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::U_CHAR):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::S_SHORT):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::U_SHORT):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::S_INT):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::U_INT):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::S_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::U_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::U_LONG_LONG; break;
                case(Tag::FLOAT):
                    retval = Tag::FLOAT; break;
                case(Tag::DOUBLE):
                    retval = Tag::DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;
        case(Tag::FLOAT):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::FLOAT; break;
                case(Tag::CHAR):
                    retval = Tag::FLOAT; break;
                case(Tag::S_CHAR):
                    retval = Tag::FLOAT; break;
                case(Tag::U_CHAR):
                    retval = Tag::FLOAT; break;
                case(Tag::S_SHORT):
                    retval = Tag::FLOAT; break;
                case(Tag::U_SHORT):
                    retval = Tag::FLOAT; break;
                case(Tag::S_INT):
                    retval = Tag::FLOAT; break;
                case(Tag::U_INT):
                    retval = Tag::FLOAT; break;
                case(Tag::S_LONG):
                    retval = Tag::FLOAT; break;
                case(Tag::U_LONG):
                    retval = Tag::FLOAT; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::FLOAT; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::FLOAT; break;
                case(Tag::FLOAT):
                    retval = Tag::FLOAT; break;
                case(Tag::DOUBLE):
                    retval = Tag::DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;
        case(Tag::DOUBLE):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::DOUBLE; break;
                case(Tag::CHAR):
                    retval = Tag::DOUBLE; break;
                case(Tag::S_CHAR):
                    retval = Tag::DOUBLE; break;
                case(Tag::U_CHAR):
                    retval = Tag::DOUBLE; break;
                case(Tag::S_SHORT):
                    retval = Tag::DOUBLE; break;
                case(Tag::U_SHORT):
                    retval = Tag::DOUBLE; break;
                case(Tag::S_INT):
                    retval = Tag::DOUBLE; break;
                case(Tag::U_INT):
                    retval = Tag::DOUBLE; break;
                case(Tag::S_LONG):
                    retval = Tag::DOUBLE; break;
                case(Tag::U_LONG):
                    retval = Tag::DOUBLE; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::DOUBLE; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::DOUBLE; break;
                case(Tag::FLOAT):
                    retval = Tag::DOUBLE; break;
                case(Tag::DOUBLE):
                    retval = Tag::DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;
        case(Tag::LONG_DOUBLE):
            switch(rhs)
            {
                case(Tag::BOOL):
                    retval = Tag::LONG_DOUBLE; break;
                case(Tag::CHAR):
                    retval = Tag::LONG_DOUBLE; break;
                case(Tag::S_CHAR):
                    retval = Tag::LONG_DOUBLE; break;
                case(Tag::U_CHAR):
                    retval = Tag::LONG_DOUBLE; break;
                case(Tag::S_SHORT):
                    retval = Tag::LONG_DOUBLE; break;
                case(Tag::U_SHORT):
                    retval = Tag::LONG_DOUBLE; break;
                case(Tag::S_INT):
                    retval = Tag::LONG_DOUBLE; break;
                case(Tag::U_INT):
                    retval = Tag::LONG_DOUBLE; break;
                case(Tag::S_LONG):
                    retval = Tag::LONG_DOUBLE; break;
                case(Tag::U_LONG):
                    retval = Tag::LONG_DOUBLE; break;
                case(Tag::S_LONG_LONG):
                    retval = Tag::LONG_DOUBLE; break;
                case(Tag::U_LONG_LONG):
                    retval = Tag::LONG_DOUBLE; break;
                case(Tag::FLOAT):
                    retval = Tag::LONG_DOUBLE; break;
                case(Tag::DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                case(Tag::LONG_DOUBLE):
                    retval = Tag::LONG_DOUBLE; break;
                
                default:
                    throw std::runtime_error("unexpected tag");
            }
            break;
        
        default:
            throw std::runtime_error("unexpected tag");
    }

    return retval;
}

#define INTEGER_OPERATOR_MACRO(ope) \
    ArithmeticType operator ope(const ArithmeticType &lhs, const ArithmeticType &rhs) \
    { \
        ArithmeticType::Tag resultTag = ArithmeticType::resultType(lhs.tag, rhs.tag); \
\
        ArithmeticType ret = ArithmeticType(), clhs = lhs, crhs = rhs; \
        ret.tag = resultTag; \
        clhs.cast(resultTag); \
        crhs.cast(resultTag); \
\
        switch(resultTag) \
        { \
            case(ArithmeticType::Tag::BOOL): \
                ret.uni.b = static_cast<bool>(lhs.uni.b ope rhs.uni.b); break; \
            case(ArithmeticType::Tag::CHAR): \
                ret.uni.c = static_cast<char>(lhs.uni.c ope rhs.uni.c); break; \
            case(ArithmeticType::Tag::S_CHAR): \
                ret.uni.sc = static_cast<signed char>(lhs.uni.sc ope rhs.uni.sc); break; \
            case(ArithmeticType::Tag::U_CHAR): \
                ret.uni.uc = static_cast<unsigned char>(lhs.uni.uc ope rhs.uni.uc); break; \
            case(ArithmeticType::Tag::S_SHORT): \
                ret.uni.ss = static_cast<signed short>(lhs.uni.ss ope rhs.uni.ss); break; \
            case(ArithmeticType::Tag::U_SHORT): \
                ret.uni.us = static_cast<unsigned short>(lhs.uni.us ope rhs.uni.us); break; \
            case(ArithmeticType::Tag::S_INT): \
                ret.uni.si = static_cast<signed int>(lhs.uni.si ope rhs.uni.si); break; \
            case(ArithmeticType::Tag::U_INT): \
                ret.uni.ui = static_cast<unsigned int>(lhs.uni.ui ope rhs.uni.ui); break; \
            case(ArithmeticType::Tag::S_LONG): \
                ret.uni.sl = static_cast<signed long>(lhs.uni.sl ope rhs.uni.sl); break; \
            case(ArithmeticType::Tag::U_LONG): \
                ret.uni.ul = static_cast<unsigned long>(lhs.uni.ul ope rhs.uni.ul); break; \
            case(ArithmeticType::Tag::S_LONG_LONG): \
                ret.uni.sll = static_cast<signed long long>(lhs.uni.sll ope rhs.uni.sll); break; \
            case(ArithmeticType::Tag::U_LONG_LONG): \
                ret.uni.ull = static_cast<unsigned long long>(lhs.uni.ull ope rhs.uni.ull); break; \
\
            default: \
                throw std::runtime_error("expected tag"); \
        } \
\
        return ret; \
    }

#define INTEGER_UNARY_OPERATOR_MACRO(ope) \
    ArithmeticType operator ope(const ArithmeticType &operand) \
    { \
        ArithmeticType ret = ArithmeticType(); \
        ret.tag = operand.tag; \
\
        switch(operand.tag) \
        { \
            case(ArithmeticType::Tag::BOOL): \
                ret.uni.b = static_cast<bool>(ope operand.uni.b); break; \
            case(ArithmeticType::Tag::CHAR): \
                ret.uni.c = static_cast<char>(ope operand.uni.b); break; \
            case(ArithmeticType::Tag::S_CHAR): \
                ret.uni.sc = static_cast<signed char>(ope operand.uni.sc); break; \
            case(ArithmeticType::Tag::U_CHAR): \
                ret.uni.uc = static_cast<unsigned char>(ope operand.uni.uc); break; \
            case(ArithmeticType::Tag::S_SHORT): \
                ret.uni.ss = static_cast<signed short>(ope operand.uni.ss); break; \
            case(ArithmeticType::Tag::U_SHORT): \
                ret.uni.us = static_cast<unsigned short>(ope operand.uni.us); break; \
            case(ArithmeticType::Tag::S_INT): \
                ret.uni.si = static_cast<signed int>(ope operand.uni.si); break; \
            case(ArithmeticType::Tag::U_INT): \
                ret.uni.ui = static_cast<unsigned int>(ope operand.uni.ui); break; \
            case(ArithmeticType::Tag::S_LONG): \
                ret.uni.sl = static_cast<signed long>(ope operand.uni.sl); break; \
            case(ArithmeticType::Tag::U_LONG): \
                ret.uni.ul = static_cast<unsigned long>(ope operand.uni.ul); break; \
            case(ArithmeticType::Tag::S_LONG_LONG): \
                ret.uni.sll = static_cast<signed long long>(ope operand.uni.sll); break; \
            case(ArithmeticType::Tag::U_LONG_LONG): \
                ret.uni.ull = static_cast<unsigned long long>(ope operand.uni.ull); break; \
\
            default: \
                throw std::runtime_error("expected tag"); \
        } \
\
        return ret; \
    }

#define ARITHMETIC_OPERATOR_MACRO(ope) \
    ArithmeticType operator ope(const ArithmeticType &lhs, const ArithmeticType &rhs) \
    { \
        ArithmeticType::Tag resultTag = ArithmeticType::resultType(lhs.tag, rhs.tag); \
\
        ArithmeticType ret = ArithmeticType(), clhs = lhs, crhs = rhs; \
        ret.tag = resultTag; \
        clhs.cast(resultTag); \
        crhs.cast(resultTag); \
\
        switch(resultTag) \
        { \
            case(ArithmeticType::Tag::BOOL): \
                ret.uni.b = static_cast<bool>(lhs.uni.b ope rhs.uni.b); break; \
            case(ArithmeticType::Tag::CHAR): \
                ret.uni.c = static_cast<char>(lhs.uni.c ope rhs.uni.c); break; \
            case(ArithmeticType::Tag::S_CHAR): \
                ret.uni.sc = static_cast<signed char>(lhs.uni.sc ope rhs.uni.sc); break; \
            case(ArithmeticType::Tag::U_CHAR): \
                ret.uni.uc = static_cast<unsigned char>(lhs.uni.uc ope rhs.uni.uc); break; \
            case(ArithmeticType::Tag::S_SHORT): \
                ret.uni.ss = static_cast<signed short>(lhs.uni.ss ope rhs.uni.ss); break; \
            case(ArithmeticType::Tag::U_SHORT): \
                ret.uni.us = static_cast<unsigned short>(lhs.uni.us ope rhs.uni.us); break; \
            case(ArithmeticType::Tag::S_INT): \
                ret.uni.si = static_cast<signed int>(lhs.uni.si ope rhs.uni.si); break; \
            case(ArithmeticType::Tag::U_INT): \
                ret.uni.ui = static_cast<unsigned int>(lhs.uni.ui ope rhs.uni.ui); break; \
            case(ArithmeticType::Tag::S_LONG): \
                ret.uni.sl = static_cast<signed long>(lhs.uni.sl ope rhs.uni.sl); break; \
            case(ArithmeticType::Tag::U_LONG): \
                ret.uni.ul = static_cast<unsigned long>(lhs.uni.ul ope rhs.uni.ul); break; \
            case(ArithmeticType::Tag::S_LONG_LONG): \
                ret.uni.sll = static_cast<signed long long>(lhs.uni.sll ope rhs.uni.sll); break; \
            case(ArithmeticType::Tag::U_LONG_LONG): \
                ret.uni.ull = static_cast<unsigned long long>(lhs.uni.ull ope rhs.uni.ull); break; \
            case(ArithmeticType::Tag::FLOAT): \
                ret.uni.f = static_cast<float>(lhs.uni.f ope rhs.uni.f); break; \
            case(ArithmeticType::Tag::DOUBLE): \
                ret.uni.d = static_cast<double>(lhs.uni.d ope rhs.uni.d); break; \
            case(ArithmeticType::Tag::LONG_DOUBLE): \
                ret.uni.ld = static_cast<long double>(lhs.uni.ld ope rhs.uni.ld); break; \
\
            default: \
                throw std::runtime_error("expected tag"); \
        } \
\
        return ret; \
    }

#define ARITHMETIC_UNARY_OPERATOR_MACRO(ope) \
    ArithmeticType operator ope(const ArithmeticType &operand) \
    { \
        ArithmeticType ret = ArithmeticType(); \
        ret.tag = operand.tag; \
\
        switch(operand.tag) \
        { \
            case(ArithmeticType::Tag::BOOL): \
                ret.uni.b = static_cast<bool>(ope operand.uni.b); break; \
            case(ArithmeticType::Tag::CHAR): \
                ret.uni.c = static_cast<char>(ope operand.uni.b); break; \
            case(ArithmeticType::Tag::S_CHAR): \
                ret.uni.sc = static_cast<signed char>(ope operand.uni.sc); break; \
            case(ArithmeticType::Tag::U_CHAR): \
                ret.uni.uc = static_cast<unsigned char>(ope operand.uni.uc); break; \
            case(ArithmeticType::Tag::S_SHORT): \
                ret.uni.ss = static_cast<signed short>(ope operand.uni.ss); break; \
            case(ArithmeticType::Tag::U_SHORT): \
                ret.uni.us = static_cast<unsigned short>(ope operand.uni.us); break; \
            case(ArithmeticType::Tag::S_INT): \
                ret.uni.si = static_cast<signed int>(ope operand.uni.si); break; \
            case(ArithmeticType::Tag::U_INT): \
                ret.uni.ui = static_cast<unsigned int>(ope operand.uni.ui); break; \
            case(ArithmeticType::Tag::S_LONG): \
                ret.uni.sl = static_cast<signed long>(ope operand.uni.sl); break; \
            case(ArithmeticType::Tag::U_LONG): \
                ret.uni.ul = static_cast<unsigned long>(ope operand.uni.ul); break; \
            case(ArithmeticType::Tag::S_LONG_LONG): \
                ret.uni.sll = static_cast<signed long long>(ope operand.uni.sll); break; \
            case(ArithmeticType::Tag::U_LONG_LONG): \
                ret.uni.ull = static_cast<unsigned long long>(ope operand.uni.ull); break; \
            case(ArithmeticType::Tag::FLOAT): \
                ret.uni.f = static_cast<float>(ope operand.uni.f); break; \
            case(ArithmeticType::Tag::DOUBLE): \
                ret.uni.d = static_cast<double>(ope operand.uni.d); break; \
            case(ArithmeticType::Tag::LONG_DOUBLE): \
                ret.uni.ld = static_cast<long double>(ope operand.uni.ld); break; \
\
            default: \
                throw std::runtime_error("expected tag"); \
        } \
\
        return ret; \
    }

INTEGER_OPERATOR_MACRO(|)
INTEGER_OPERATOR_MACRO(^)
INTEGER_OPERATOR_MACRO(&)
ARITHMETIC_OPERATOR_MACRO(==)
ARITHMETIC_OPERATOR_MACRO(!=)
ARITHMETIC_OPERATOR_MACRO(<)
ARITHMETIC_OPERATOR_MACRO(>)
ARITHMETIC_OPERATOR_MACRO(<=)
ARITHMETIC_OPERATOR_MACRO(>=)
INTEGER_OPERATOR_MACRO(<<)
INTEGER_OPERATOR_MACRO(>>)
ARITHMETIC_OPERATOR_MACRO(+)
ARITHMETIC_OPERATOR_MACRO(-)
ARITHMETIC_OPERATOR_MACRO(*)
ARITHMETIC_OPERATOR_MACRO(/)
INTEGER_OPERATOR_MACRO(%)
ARITHMETIC_UNARY_OPERATOR_MACRO(+)
ARITHMETIC_UNARY_OPERATOR_MACRO(-)
INTEGER_UNARY_OPERATOR_MACRO(~)
ARITHMETIC_UNARY_OPERATOR_MACRO(!)

#undef INTEGER_OPERATOR_MACRO
#undef INTEGER_UNARY_OPERATOR_MACRO
#undef ARITHMETIC_OPERATOR_MACRO
#undef ARITHMETIC_UNARY_OPERATOR_MACRO

}