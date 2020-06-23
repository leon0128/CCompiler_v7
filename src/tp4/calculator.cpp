#include <iostream>

#include "../simbol.hpp"
#include "../global.hpp"
#include "integer_converter.hpp"
#include "integer.hpp"
#include "calculator.hpp"

bool TP4::Calculator::execute(std::vector<PPToken*> &ptvec,
                              Integer &result)
{
    Calculator calculator(ptvec, result);

    calculator.calculate();

    return calculator.mIsValid;
}

TP4::Calculator::Calculator(std::vector<PPToken*> &ptvec,
                            Integer &result):
    mPtvec(ptvec),
    mResult(result),
    mIdx(0),
    mIsValid(true)
{
}

void TP4::Calculator::calculate()
{
    calcConstantExpression(mResult);

    if(mIdx != mPtvec.size())
    {
        mIsValid = false;
        std::cout << "TP4 Calculator error:\n"
                     "    what: constant-expression is not evaluated to the end.\n"
                     "    filename: " << Global::CURRENT_FILENAME
                  << std::endl;
    }
}

bool TP4::Calculator::calcAdditiveExpression(Integer &res)
{
    return calculateUsedOperator(&Calculator::calcMultiplicativeExpression,
                                 {{Punctuator::Tag::PLUS, operator +},
                                  {Punctuator::Tag::MINUS, operator -}},
                                 res);
}

bool TP4::Calculator::calcANDExpression(Integer &res)
{
    return calculateUsedOperator(&Calculator::calcEqualityExpression,
                                 {{Punctuator::Tag::AMP, operator &}},
                                 res);
}

bool TP4::Calculator::calcAssignmentExpression(Integer &res)
{
    return calcConditionalExpression(res);
}

bool TP4::Calculator::calcCastExpression(Integer &res)
{
    return calcUnaryExpression(res);
}

bool TP4::Calculator::calcConditionalExpression(Integer &res)
{
    auto befidx = mIdx;
    auto mididx = mIdx;
    bool isLogical = false;
    bool isConditional = false;
    Integer lhs, rhs;

    if(calcLogicalORExpression(res))
    {
        isLogical = true;
        mididx = mIdx;

        if(isMatch(mIdx, Punctuator::Tag::QUE))
        {
            mIdx++;
            if(calcExpression(lhs))
            {
                if(isMatch(mIdx, Punctuator::Tag::COLON))
                {
                    mIdx++;
                    if(calcConditionalExpression(rhs))
                        isConditional = true;
                }
            }
        }
    }

    if(isLogical)
    {
        if(!isConditional)
            mIdx = mididx;
        else
        {
            if(res.tag == Integer::Tag::SIGNED)
                res = res.uni.i ? lhs : rhs;
            else
                res = res.uni.u ? lhs : rhs;
        }

        return true;
    }
    else
    {
        mIdx = befidx;
        return false;
    }
}

bool TP4::Calculator::calcConstantExpression(Integer &res)
{
    return calcConditionalExpression(res);
}

bool TP4::Calculator::calcEqualityExpression(Integer &res)
{
    return calculateUsedOperator(&Calculator::calcRelationalExpression,
                                 {{Punctuator::Tag::EQUAL, operator ==},
                                  {Punctuator::Tag::NOT_EQ, operator !=}},
                                 res);
}

bool TP4::Calculator::calcExclusiveORExpression(Integer &res)
{
    return calculateUsedOperator(&Calculator::calcANDExpression,
                                 {{Punctuator::Tag::EX_OR, operator ^}},
                                 res);
}

bool TP4::Calculator::calcExpression(Integer &res)
{
    bool isValid = false;

    if(calcAssignmentExpression(res))
        isValid = true;

    while(isValid)
    {
        if(isMatch(mIdx, Punctuator::Tag::COMMA))
        {
            mIdx++;
            if(!calcAssignmentExpression(res))
            {
                mIdx--;
                break;
            }
        }
        else
            break;
    }

    return isValid;
}

bool TP4::Calculator::calcInclusiveORExpression(Integer &res)
{
    return calculateUsedOperator(&Calculator::calcExclusiveORExpression,
                                 {{Punctuator::Tag::IN_OR, operator |}},
                                 res);
}

bool TP4::Calculator::calcLogicalANDExpression(Integer &res)
{
    return calculateUsedOperator(&Calculator::calcInclusiveORExpression,
                                 {{Punctuator::Tag::AND, operator &&}},
                                 res);
}

bool TP4::Calculator::calcLogicalORExpression(Integer &res)
{
    return calculateUsedOperator(&Calculator::calcLogicalANDExpression,
                                 {{Punctuator::Tag::OR, operator ||}},
                                 res);
}

bool TP4::Calculator::calcMultiplicativeExpression(Integer &res)
{
    return calculateUsedOperator(&Calculator::calcCastExpression,
                                 {{Punctuator::Tag::AST, operator *},
                                  {Punctuator::Tag::SLASH, operator /},
                                  {Punctuator::Tag::PER, operator %}},
                                 res);
}

bool TP4::Calculator::calcPostfixExpression(Integer &res)
{
    return calcPrimaryExpression(res);
}

bool TP4::Calculator::calcPrimaryExpression(Integer &res)
{
    bool isValid = false;
    auto befidx = mIdx;

    // ( expression )
    if(isMatch(mIdx, Punctuator::Tag::L_PAREN))
    {
        mIdx++;
        if(calcExpression(res))
        {
            if(isMatch(mIdx, Punctuator::Tag::R_PAREN))
            {
                mIdx++;
                isValid = true;
            }
        }
    }
    // constant (integer-constant or character-constant)
    else if(IntegerConverter::convert(mPtvec[mIdx], res))
    {
        mIdx++;
        isValid = true;
    }

    if(isValid)
        return true;
    else
    {
        mIdx = befidx;
        return false;
    }
}

bool TP4::Calculator::calcRelationalExpression(Integer &res)
{
    return calculateUsedOperator(&Calculator::calcShiftExpression,
                                 {{Punctuator::Tag::LESS, operator <},
                                  {Punctuator::Tag::GREATER, operator >},
                                  {Punctuator::Tag::LESS_EQ, operator <=},
                                  {Punctuator::Tag::GREATER_EQ, operator >=}},
                                 res);
}

bool TP4::Calculator::calcShiftExpression(Integer &res)
{
    return calculateUsedOperator(&Calculator::calcAdditiveExpression,
                                 {{Punctuator::Tag::L_SHIFT, operator <<},
                                  {Punctuator::Tag::R_SHIFT, operator >>}},
                                 res);
}

bool TP4::Calculator::calcUnaryExpression(Integer &res)
{ 
    bool isUnaryOperator = true;
    Integer (Integer::*unaryFunc)();
    if(isMatch(mIdx, Punctuator::Tag::PLUS))
        unaryFunc = &Integer::operator+;
    else if(isMatch(mIdx, Punctuator::Tag::MINUS))
        unaryFunc = &Integer::operator-;
    else if(isMatch(mIdx, Punctuator::Tag::TILDE))
        unaryFunc = &Integer::operator~;
    else if(isMatch(mIdx, Punctuator::Tag::EXC))
        unaryFunc = &Integer::operator!;
    else
        isUnaryOperator = false;
        
    if(isUnaryOperator)
    {
        mIdx++;
        if(calcCastExpression(res))
        {
            res = (res.*unaryFunc)();
            return true;
        }
        else
        {
            mIdx--;
            return false;
        }
    }
    else
        return calcPostfixExpression(res);
}

bool TP4::Calculator::calculateUsedOperator(OperandFunction_t calleeFunc,
                                            std::vector<std::pair<Punctuator::Tag, OperatorFunction_t>> &&opecodeVec,
                                            Integer &result)
{
    bool isValid = false;

    if((this->*calleeFunc)(result))
        isValid = true;
    
    bool isContinued = true;
    while(isContinued)
    {
        for(std::size_t i = 0; i < opecodeVec.size(); i++)
        {
            if(isMatch(mIdx, opecodeVec[i].first))
            {
                mIdx++;
                Integer rhs;
                if((this->*calleeFunc)(rhs))
                {
                    result = opecodeVec[i].second(result, rhs);
                    break;
                }
                else
                {
                    isContinued = false;
                    mIdx--;
                    break;
                }
            }

            if(i + 1 == opecodeVec.size())
                isContinued = false;
        }
    }

    return isValid;
}

bool TP4::Calculator::isMatch(std::size_t idx,
                              Punctuator::Tag tag) const noexcept
{
    if(idx < mPtvec.size())
        return (mPtvec[idx]->tag == PPToken::Tag::PUNCTUATOR) ? (mPtvec[idx]->uni.punctuator->tag == tag) : false;
    else
        return false;
}