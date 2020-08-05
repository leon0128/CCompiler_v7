#ifndef TP7_STATIC_ASSERT_SIMBOL_HPP
#define TP7_STATIC_ASSERT_SIMBOL_HPP

#include "../simbol.hpp"

class StringLiteral;

namespace TP7
{

class ConstantExpression;
class StaticAssertDeclaration;

class StaticAssertDeclaration : public BaseSimbol
{
public:
    ConstantExpression *constant;
    StringLiteral *str;

    StaticAssertDeclaration() noexcept
        : BaseSimbol()
        , constant(nullptr)
        , str(nullptr){}
    
    std::string string() const override;
};

}

#endif