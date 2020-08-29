#include <iostream>
#include "expression_simbol.hpp"
#include "calculator.hpp"

namespace TP7
{

Constant Calculator::calculate(const ConstantExpression *exp)
{
    return calculate(exp->conditional);
}

Constant Calculator::calculate(const ConditionalExpression *exp)
{
    if(exp->conditional == nullptr)
        return calculate(exp->logicalOR);
    else
    {
        const Constant l = calculate(exp->logicalOR);
        if(l.type->tag != Type::Tag::NONE)
        {
            if(!l.isZero())
                return calculate(exp->expression);
            else
                return calculate(exp->conditional);
        }
        else
            return Constant();
    }
}

Constant Calculator::calculate(const LogicalORExpression *exp)
{
    if(exp->vec.size() == 1)
        return calculate(exp->vec.front());
    else
    {
        bool isZero = true;
        for(auto &&e : exp->vec)
        {
            const Constant l = calculate(e);
            if(l.type->tag == Type::Tag::NONE)
                return Constant();

            if(!(isZero = l.isZero()))
                break;
        }

        Constant ret;
        ret.uni.si = isZero ? 0 : 1;
        ret.type->tag = Type::Tag::BASE;
        ret.type->uni.bt = new BaseType();
        ret.type->uni.bt->tag = BaseType::Tag::S_INT;
        
        return ret;
    }
}

Constant Calculator::calculate(const Expression *exp)
{
    return calculate(exp->vec.back());
}

Constant Calculator::calculate(const LogicalANDExpression *exp)
{
    if(exp->vec.size() == 1)
        return calculate(exp->vec.front());
    else
    {
        bool isZero = true;
        for(auto &&e : exp->vec)
        {
            const Constant l = calculate(e);
            if(l.type->tag == Type::Tag::NONE)
                return Constant();
            
            if((isZero = l.isZero()))
                break;
        }

        Constant ret;
        ret.uni.si = isZero ? 0 : 1;
        ret.type->tag = Type::Tag::BASE;
        ret.type->uni.bt = new BaseType();
        ret.type->uni.bt->tag = BaseType::Tag::S_INT;

        return ret;
    }
}

Constant Calculator::calculate(const AssignmentExpression *exp)
{
    if(exp->tag == AssignmentExpression::Tag::CONDITIONAL)
        return calculate(exp->uni.conditional);
    else
    {
        std::cout << "TP7 Calculator error:\n"
            "    what: constant-expression shall not include assignment-operator.\n"
            "    contents: " << exp->string()
            << std::endl;
        return Constant();
    }
}

Constant Calculator::calculate(const ExclusiveORExpression *exp)
{
    
}

}