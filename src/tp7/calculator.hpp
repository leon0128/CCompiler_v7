#ifndef TP7_CALCULATOR_HPP
#define TP7_CALCULATOR_HPP

#include "constant.hpp"

namespace TP7
{

class ConstantExpression;
class ConditionalExpression;
class LigicalORExpression;
class Expression;
class LogicalANDExpression;
class AssignmentExpression;
class InclusiveORExpression;
class ExclusiveORExpression;

class Calculator
{
public:
    static Constant calculate(const ConstantExpression*);
    static Constant calculate(const ConditionalExpression*);
    static Constant calculate(const LogicalORExpression*);
    static Constant calculate(const Expression*);
    static Constant calculate(const LogicalANDExpression*);
    static Constant calculate(const AssignmentExpression*);
    static Constant calculate(const InclusiveORExpression*);
    static Constant calculate(const ExclusiveORExpression*);
};

}

#endif