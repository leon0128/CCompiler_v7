#ifndef TP7_CALCULATOR_HPP
#define TP7_CALCULATOR_HPP

namespace TP7
{

class ConstantExpression;
class ConditionalExpression;
class LogicalORExpression;
class Expression;
class AssignmentExpression;
class LogicalANDExpression;
class InclusiveORExpression;

namespace CONSTANT
{
    class Constant;
}

namespace CALCULATOR
{

    extern CONSTANT::Constant *calculate(const ConstantExpression*);
    extern CONSTANT::Constant *calculate(const ConditionalExpression*);
    extern CONSTANT::Constant *calculate(const LogicalORExpression*);
    extern CONSTANT::Constant *calculate(const Expression*);
    extern CONSTANT::Constant *calculate(const AssignmentExpression*);
    extern CONSTANT::Constant *calculate(const LogicalANDExpression*);
    extern CONSTANT::Constant *calculate(const InclusiveORExpression*);

}

}

#endif