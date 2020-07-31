#ifndef TP7_CALCULATOR_HPP
#define TP7_CALCULATOR_HPP

#include <unordered_map>
#include <unordered_set>

#include "expression_simbol.hpp"
#include "arithmetic_type.hpp"

namespace TP7
{

class Calculator
{
public:
    static ArithmeticType calculateConstantExpression(ConstantExpression*);

private:
    static ArithmeticType calc(ConstantExpression*);
    static ArithmeticType calc(Expression*);
    static ArithmeticType calc(AssignmentExpression*);
    static ArithmeticType calc(ConditionalExpression*);
    static ArithmeticType calc(LogicalORExpression*);
    static ArithmeticType calc(LogicalANDExpression*);
    static ArithmeticType calc(InclusiveORExpression*);
    static ArithmeticType calc(ExclusiveORExpression*);
    static ArithmeticType calc(ANDExpression*);
    static ArithmeticType calc(EqualityExpression*);
    static ArithmeticType calc(RelationalExpression*);
    static ArithmeticType calc(ShiftExpression*);
    static ArithmeticType calc(AdditiveExpression*);
    static ArithmeticType calc(MultiplicativeExpression*);
    static ArithmeticType calc(CastExpression*);
    static ArithmeticType calc(UnaryExpression*);
    static ArithmeticType calc(PostfixExpression*);
    static ArithmeticType calc(PrimaryExpression*);

    static ArithmeticType convertConstant(Constant*);
    static ArithmeticType convertIntegerConstant(IntegerConstant*);
    static ArithmeticType convertFloatingConstant(FloatingConstant*);
    static ArithmeticType convertCharacterConstant(CharacterConstant*);
};

}

#endif