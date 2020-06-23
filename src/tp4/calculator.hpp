#ifndef TP4_Calculator_HPP
#define TP4_Calculator_HPP

#include <vector>
#include <utility>
#include <unordered_map>

#include "../simbol.hpp"
#include "integer.hpp"

namespace TP4

{
class Calculator
{
private:
    using OperandFunction_t = bool (Calculator::*)(Integer&);
    using OperatorFunction_t = Integer (*)(const Integer&, const Integer&);

public:
    static bool execute(std::vector<PPToken*>&,
                        Integer&);

private:
    Calculator(std::vector<PPToken*>&,
               Integer&);
    
    void calculate();

    // helper for calcluate()
    bool calcAdditiveExpression(Integer&);
    bool calcANDExpression(Integer&);
    bool calcAssignmentExpression(Integer&);
    bool calcCastExpression(Integer&);
    bool calcConditionalExpression(Integer&);
    bool calcConstantExpression(Integer&);
    bool calcEqualityExpression(Integer&);
    bool calcExclusiveORExpression(Integer&);
    bool calcExpression(Integer&);
    bool calcInclusiveORExpression(Integer&);
    bool calcLogicalANDExpression(Integer&);
    bool calcLogicalORExpression(Integer&);
    bool calcMultiplicativeExpression(Integer&);
    bool calcPostfixExpression(Integer&);
    bool calcPrimaryExpression(Integer&);
    bool calcRelationalExpression(Integer&);
    bool calcShiftExpression(Integer&);
    bool calcUnaryExpression(Integer&);

    // helper for calc function
    bool calculateUsedOperator(OperandFunction_t,
                               std::vector<std::pair<Punctuator::Tag, OperatorFunction_t>>&&,
                               Integer &result);

    // helper for calc
    bool isMatch(std::size_t idx,
                 Punctuator::Tag tag) const noexcept;

    std::vector<PPToken*> &mPtvec;
    Integer &mResult;
    std::size_t mIdx;
    bool mIsValid;
};

}

#endif