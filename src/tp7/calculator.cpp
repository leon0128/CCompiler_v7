#include <iostream>

#include "calculator.hpp"

namespace TP7
{

ArithmeticType Calculator::calculateConstantExpression(ConstantExpression *constantExpression)
{
    return calc(constantExpression);
}

ArithmeticType Calculator::calc(Expression *expression)
{
    if(expression->vec.size() == 1)
        return calc(expression->vec.front());
    else
    {
        std::cout << "TP7 Calculator error:\n"
            "    what: constant-expression cannot include comma operators.\n"
            << std::flush;
        return ArithmeticType();
    }
}

ArithmeticType Calculator::calc(AssignmentExpression *assignmentExpression)
{
    if(assignmentExpression->tag == AssignmentExpression::Tag::CONDITIONAL)
        return calc(assignmentExpression->uni.conditional);
    else
    {
        std::cout << "TP7 Calculator error:\n"
            "    what: constant-expression cannot include assignment-expression.\n"
            << std::endl;
        return ArithmeticType();
    }
}

ArithmeticType Calculator::calc(ConstantExpression *constantExpression)
{
    return calc(constantExpression->conditional);
}

ArithmeticType Calculator::calc(ConditionalExpression *conditionalExpression)
{
    ArithmeticType logicalOR = calc(conditionalExpression->logicalOR);

    if(conditionalExpression->conditional != nullptr)
        return logicalOR.tag != ArithmeticType::Tag::NONE
            ? logicalOR.isNonzero()
                ? calc(conditionalExpression->expression)
                    : calc(conditionalExpression->conditional)
                        : logicalOR;
    else
        return logicalOR;
}

ArithmeticType Calculator::calc(LogicalORExpression *logicalORExpression)
{
    if(logicalORExpression->vec.size() == 1)
        return calc(logicalORExpression->vec.front());
    else
    {
        for(auto &&l : logicalORExpression->vec)
        {
            ArithmeticType res = calc(l);
            if(res.tag != ArithmeticType::Tag::NONE)
            {
                if(res.isNonzero())
                {
                    ArithmeticType ret = ArithmeticType();
                    ret.tag = ArithmeticType::Tag::S_INT;
                    ret.uni.si = 1;
                    return ret;
                }
            }
            else
                return res;
        }

        ArithmeticType ret = ArithmeticType();
        ret.tag = ArithmeticType::Tag::S_INT;
        ret.uni.si = 0;
        return ret;
    }
}

ArithmeticType Calculator::calc(LogicalANDExpression *logicalANDExpression)
{
    if(logicalANDExpression->vec.size() == 1)
        return calc(logicalANDExpression->vec.front());
    else
    {
        for(auto &&e : logicalANDExpression->vec)
        {
            ArithmeticType res = calc(e);
            if(res.tag != ArithmeticType::Tag::NONE)
            {
                if(!res.isNonzero())
                {
                    ArithmeticType ret = ArithmeticType();
                    ret.tag = ArithmeticType::Tag::S_INT;
                    ret.uni.si = 0;
                    return ret;
                }
            }
            else
                return res;
        }

        ArithmeticType ret = ArithmeticType();
        ret.tag = ArithmeticType::Tag::S_INT;
        ret.uni.si = 1;
        return ret;
    }
}

ArithmeticType Calculator::calc(InclusiveORExpression *inclusiveORExpression)
{
    if(inclusiveORExpression->vec.size() == 1)
        return calc(inclusiveORExpression->vec.front());
    else
    {
        ArithmeticType ret = calc(inclusiveORExpression->vec.front());
        if(!ret.isInteger())
        {
            std::cout << "TP7 Calculator error:\n"
                "    what: inclusive-OR-expression cannot include type other than integer type.\n"
                << std::flush;
            return ArithmeticType();
        }

        for(std::size_t i = 1; i < inclusiveORExpression->vec.size(); i++)
        {
            ArithmeticType rhs = calc(inclusiveORExpression->vec[i]);
            if(!ret.isInteger())
            {
                std::cout << "TP7 Calculator error:\n"
                    "    what: inclusive-OR-expression cannot include type other than integer type.\n"
                    << std::flush;
                return ArithmeticType();
            }
            ret = ret | rhs;
        }

        return ret;
    }
}

ArithmeticType Calculator::calc(ExclusiveORExpression *exclusiveORExpression)
{
    if(exclusiveORExpression->vec.size() == 1)
        return calc(exclusiveORExpression->vec.front());
    else
    {
        ArithmeticType ret = calc(exclusiveORExpression->vec.front());
        if(!ret.isInteger())
        {
            std::cout << "TP7 Calculator error:\n"
                "    what: exclusive-OR-expression cannot include type other than integer type.\n"
                << std::flush;
            return ArithmeticType();
        }

        for(std::size_t i = 1; i < exclusiveORExpression->vec.size(); i++)
        {
            ArithmeticType rhs = calc(exclusiveORExpression->vec[i]);
            if(!ret.isInteger())
            {
                std::cout << "TP7 Calculator error:\n"
                    "    what: exclusive-OR-expression cannot include type other than integer type.\n"
                    << std::flush;
                return ArithmeticType();
            }
            ret = ret ^ rhs;
        }

        return ret;
    }
}

ArithmeticType Calculator::calc(ANDExpression *andExpression)
{
    if(andExpression->vec.size() == 1)
        return calc(andExpression->vec.front());
    else
    {
        ArithmeticType ret = calc(andExpression->vec.front());
        if(!ret.isInteger())
        {
            std::cout << "TP7 Calculator error:\n"
                "    what: AND-expression cannot include type other than integer type.\n"
                << std::flush;
            return ArithmeticType();
        }

        for(std::size_t i = 1; i < andExpression->vec.size(); i++)
        {
            ArithmeticType rhs = calc(andExpression->vec[i]);
            if(!ret.isInteger())
            {
                std::cout << "TP7 Calculator error:\n"
                    "    what: AND-expression cannot include type other than integer type.\n"
                    << std::flush;
                return ArithmeticType();
            }
            ret = ret & rhs;
        }

        return ret;
    }
}

ArithmeticType Calculator::calc(EqualityExpression *equalityExpression)
{
    if(equalityExpression->relvec.size() == 1)
        return calc(equalityExpression->relvec.front());
    else
    {
        ArithmeticType ret = calc(equalityExpression->relvec.front());
        if(!ret.isArithmetic())
            return ret;

        for(std::size_t i = 1; i < equalityExpression->relvec.size(); i++)
        {
            ArithmeticType rhs = calc(equalityExpression->relvec[i]);
            if(!rhs.isArithmetic())
                return ArithmeticType();
            
            if(equalityExpression->tagvec[i - 1]->tag == Punctuator::Tag::EQUAL)
                ret = ret == rhs;
            else if(equalityExpression->tagvec[i - 1]->tag == Punctuator::Tag::NOT_EQ)
                ret = ret != rhs;
            else
            {
                std::cerr << "TP7 Calculator implementation error:\n"
                    "    what: equality-expression cannot include operator othre than equal and not-equal.\n"
                    << std::flush;
                return ArithmeticType();
            }

            ret.cast(ArithmeticType::Tag::S_INT);
        }

        return ret;
    }
}

ArithmeticType Calculator::calc(RelationalExpression *relationalExpression)
{
    if(relationalExpression->sftvec.size() == 1)
        return calc(relationalExpression->sftvec.front());
    else
    {
        ArithmeticType ret = calc(relationalExpression->sftvec.front());
        if(!ret.isArithmetic())
            return ret;

        for(std::size_t i = 1; i < relationalExpression->sftvec.size(); i++)
        {
            ArithmeticType rhs = calc(relationalExpression->sftvec[i]);
            if(!rhs.isArithmetic())
                return ArithmeticType();
            
            if(relationalExpression->tagvec[i - 1]->tag == Punctuator::Tag::LESS)
                ret = ret < rhs;
            else if(relationalExpression->tagvec[i - 1]->tag == Punctuator::Tag::GREATER)
                ret = ret > rhs;
            else if(relationalExpression->tagvec[i - 1]->tag == Punctuator::Tag::LESS_EQ)
                ret = ret <= rhs;
            else if(relationalExpression->tagvec[i - 1]->tag == Punctuator::Tag::GREATER_EQ)
                ret = ret >= rhs;
            else
            {
                std::cerr << "TP7 Calculator implementation error:\n"
                    "    what: relational-expression cannot include operator othre than less, greater, less-eq, greater-eq operator.\n"
                    << std::flush;
                return ArithmeticType();
            }

            ret.cast(ArithmeticType::Tag::S_INT);
        }

        return ret;
    }
}

ArithmeticType Calculator::calc(ShiftExpression *expression)
{
    if(expression->addvec.size() == 1)
        return calc(expression->addvec.front());
    else
    {
        ArithmeticType ret = calc(expression->addvec.front());
        if(!ret.isInteger())
        {
            std::cout << "TP7 Calculator error:\n"
                "    what: shift-expression cannot include type other than integer type.\n"
                << std::flush;
            return ArithmeticType();
        }

        for(std::size_t i = 1; i < expression->addvec.size(); i++)
        {
            ArithmeticType rhs = calc(expression->addvec[i]);
            ArithmeticType::Tag resTag = ret.tag;
            if(!ret.isInteger())
            {
                std::cout << "TP7 Calculator error:\n"
                    "    what: shift-expression cannot include type other than integer type.\n"
                    << std::flush;
                return ArithmeticType();
            }
            
            if(expression->tagvec[i - 1]->tag == Punctuator::Tag::L_SHIFT)
                ret = ret << rhs;
            else if(expression->tagvec[i - 1]->tag == Punctuator::Tag::R_SHIFT)
                ret = ret >> rhs;
            else
            {
                std::cerr << "TP7 Calculator implementation error:\n"
                    "    what: shift-expression cannot include operator othre than l-shift or r-shift.\n"
                    << std::flush;
                return ArithmeticType();
            }

            ret.cast(resTag);
        }

        return ret;
    }
}

ArithmeticType Calculator::calc(AdditiveExpression *expression)
{
    if(expression->mulvec.size() == 1)
        return calc(expression->mulvec.front());
    else
    {
        ArithmeticType ret = calc(expression->mulvec.front());
        if(!ret.isArithmetic())
            return ArithmeticType();

        for(std::size_t i = 1; i < expression->mulvec.size(); i++)
        {
            ArithmeticType rhs = calc(expression->mulvec[i]);
            if(!ret.isArithmetic())
                return ArithmeticType();
            
            if(expression->tagvec[i - 1]->tag == Punctuator::Tag::PLUS)
                ret = ret + rhs;
            else if(expression->tagvec[i - 1]->tag == Punctuator::Tag::MINUS)
                ret = ret - rhs;
            else
            {
                std::cerr << "TP7 Calculator implementation error:\n"
                    "    what: additive-expression cannot include operator othre than plus and minus operator.\n"
                    << std::flush;
                return ArithmeticType();
            }
        }

        return ret;
    }
}

ArithmeticType Calculator::calc(MultiplicativeExpression *expression)
{
    if(expression->castvec.size() == 1)
        return calc(expression->castvec.front());
    else
    {
        ArithmeticType ret = calc(expression->castvec.front());
        if(!ret.isArithmetic())
            return ArithmeticType();

        for(std::size_t i = 1; i < expression->castvec.size(); i++)
        {
            ArithmeticType rhs = calc(expression->castvec[i]);
            if(!ret.isArithmetic())
                return ArithmeticType();
            
            if(expression->tagvec[i - 1]->tag == Punctuator::Tag::AST)
                ret = ret * rhs;
            else if(expression->tagvec[i - 1]->tag == Punctuator::Tag::SLASH)
                ret = ret / rhs;
            else if(expression->tagvec[i - 1]->tag == Punctuator::Tag::PER)
            {
                if(ret.isInteger() && rhs.isInteger())
                    ret = ret % rhs;
                if(!ret.isInteger() || !rhs.isInteger())
                {
                    std::cout << "TP7 Calculator error:\n"
                        "    what: % operator cannot have type other than integer.\n"
                        << std::flush;
                    return ArithmeticType();
                }
            }
            else
            {
                std::cerr << "TP7 Calculator implementation error:\n"
                    "    what: multiplicative-expression cannot include operator othre than asterisk, slash or percent operator.\n"
                    << std::flush;
                return ArithmeticType();
            }
        }

        return ret;
    }
}

ArithmeticType Calculator::calc(CastExpression *expression)
{
    ArithmeticType ret = ArithmeticType();

    switch(expression->tag)
    {
        case(CastExpression::Tag::UNARY):
            ret = calc(expression->uni.unary);
            break;
        
        default:
            std::cerr << "TP7 Calculator implementation error:\n"
                "    what: cast-expression.\n"
            << std::flush;
    }

    return ret;
}

ArithmeticType Calculator::calc(UnaryExpression *expression)
{
    ArithmeticType ret = ArithmeticType();

    switch(expression->tag)
    {
        case(UnaryExpression::Tag::POSTFIX):
            ret = calc(expression->uni.postfix);
            break;
        case(UnaryExpression::Tag::INC):
        case(UnaryExpression::Tag::DEC):
            std::cout << "TP7 Calculator error:\n"
                "    what: constant-expression cannnot include increment and decrement\n"
                << std::endl;
            break;
        case(UnaryExpression::Tag::UNARY):
        {
            ArithmeticType rhs = calc(expression->uni.sUnary.cast);
            if(!rhs.isArithmetic())
                return ArithmeticType();

            switch(expression->uni.sUnary.tag->tag)
            {
                case(Punctuator::Tag::PLUS):
                    ret = +rhs; break;
                case(Punctuator::Tag::MINUS):
                    ret = -rhs; break;
                case(Punctuator::Tag::TILDE):
                    if(rhs.isInteger())
                        ret = ~rhs;
                    else
                    {
                        std::cout << "TP7 Calculator error:\n"
                            "    what: tolde-operator cannot include type other than integer.\n"
                            << std::flush;
                        return ArithmeticType();
                    }
                    break;
                case(Punctuator::Tag::EXC):
                    ret = !rhs;
                    ret.cast(ArithmeticType::Tag::S_INT);
                    break;
                
                default:
                    Simbol::unexpectTag("UnaryExpression");
            }
            break;
        }

        default:
            Simbol::unexpectTag("UnaryExpression");
    }

    return ret;
}

ArithmeticType Calculator::calc(PostfixExpression *expression)
{
    ArithmeticType ret = ArithmeticType();

    switch(expression->tag)
    {
        case(PostfixExpression::Tag::PRIMARY):
            ret = calc(expression->uni.primary);
            break;
        
        default:
            Simbol::unexpectTag("PostfixExpression");
    }

    return ret;
}

ArithmeticType Calculator::calc(PrimaryExpression *expression)
{
    ArithmeticType ret = ArithmeticType();

    switch(expression->tag)
    {
        case(PrimaryExpression::Tag::CONSTANT):
            ret = convertConstant(expression->uni.constant);
            break;
        case(PrimaryExpression::Tag::EXPRESSION):
            ret = calc(expression->uni.expression);
            break;

        default:
            Simbol::unexpectTag("PrimaryExpression");
    }

    return ret;
}

ArithmeticType Calculator::convertConstant(Constant *constant)
{
    ArithmeticType ret = ArithmeticType();

    switch(constant->tag)
    {
        case(Constant::Tag::INTEGER):
            ret = convertIntegerConstant(constant->uni.integer);
            break;
        case(Constant::Tag::FLOATING):
            ret = convertFloatingConstant(constant->uni.floating);
            break;
        case(Constant::Tag::CHARACTER):
            ret = convertCharacterConstant(constant->uni.character);
            break;
        
        default:
            Simbol::unexpectTag("Constant");
    }

    return ret;
}

ArithmeticType Calculator::convertIntegerConstant(IntegerConstant *integer)
{
    
}

}