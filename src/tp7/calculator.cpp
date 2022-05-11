#include "expression_simbol.hpp"
#include "type.hpp"
#include "constant.hpp"
#include "calculator.hpp"

namespace TP7
{

namespace CALCULATOR
{

CONSTANT::Constant *calculate(const ConstantExpression *ce)
{
    return calculate(ce->conditional);
}

CONSTANT::Constant *calculate(const ConditionalExpression *ce)
{
    CONSTANT::Constant *ret = calculate(ce->logicalOR);

    if(ce->conditional != nullptr)
    {
        if(!ret->isZero())
            ret = calculate(ce->expression);
        else
            ret = calculate(ce->conditional);
    }

    return ret;
}

CONSTANT::Constant *calculate(const LogicalORExpression *lore)
{
    if(lore->vec.size() == 1)
        return calculate(lore->vec.front());
    else
    {
        bool isZero = false;
        for(auto &&lande : lore->vec)
        {
            CONSTANT::Constant *c = calculate(lande);

            if(!(isZero = c->isZero()))
                break;
        }

        CONSTANT::Constant *ret = new CONSTANT::Constant();
        ret->type = new Type();
        ret->type->set(new BaseType())->tag = BaseType::Tag::S_INT;
        ret->uni.si = isZero ? 0 : 1;

        return ret;
    }
}

CONSTANT::Constant *calculate(const Expression *e)
{
    return calculate(e->vec.back());
}

CONSTANT::Constant *calculate(const AssignmentExpression *ae)
{
    CONSTANT::Constant *ret = nullptr;

    switch(ae->tag)
    {
        case(AssignmentExpression::Tag::CONDITIONAL):
            ret = calculate(ae->uni.conditional);
            break;
        
        default:
            Simbol::unexpectTag("AssignmentExpression");
    }

    return ret;
}

CONSTANT::Constant *calculate(const LogicalANDExpression *lande)
{
    if(lande->vec.size() == 1)
        return calculate(lande->vec.front());
    else
    {
        bool isZero = false;
        for(auto &&iore : lande->vec)
        {
            CONSTANT::Constant *c = calculate(iore);

            if((isZero = c->isZero()))
                break;
        }

        CONSTANT::Constant *ret = new CONSTANT::Constant();
        ret->type = new Type();
        ret->type->set(new BaseType())->tag = BaseType::Tag::S_INT;
        ret->uni.si = isZero ? 0 : 1;

        return ret;
    }
}

CONSTANT::Constant *calculate(const InclusiveORExpression *iore)
{
    
}

}

}