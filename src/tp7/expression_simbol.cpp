#include "expression_simbol.hpp"

namespace TP7
{

std::string Expression::string() const
{
    std::string retval = vec.front()->string();

    for(std::size_t i = 1; i < vec.size(); i++)
    {
        retval.push_back(',');
        retval += vec[i]->string();
    }

    return retval;
}

std::string ConstantExpression::string() const
{
    return conditional->string();
}

std::string AssignmentExpression::string() const
{
    std::string retval;

    switch(tag)
    {
        case(Tag::CONDITIONAL):
            retval += uni.conditional->string();
            break;
        case(Tag::ASSIGNMENT):
            retval += uni.sAssignment.unary->string();
            retval += uni.sAssignment.punctuator->string();
            retval += uni.sAssignment.assignment->string();
            break;
        
        default:
            Simbol::unexpectTag("AssignmentExpression");
            break;
    }

    return retval;
}

std::string ConditionalExpression::string() const
{
    std::string retval = logicalOR->string();

    if(expression != nullptr)
    {
        retval.push_back('\?');
        retval += expression->string();
        retval.push_back(':');
        retval += conditional->string();
    }

    return retval;
}

std::string LogicalORExpression::string() const
{
    std::string retval = vec.front()->string();

    for(std::size_t i = 1; i < vec.size(); i++)
    {
        retval += "||";
        retval += vec[i]->string();
    }

    return retval;
}

std::string LogicalANDExpression::string() const
{
    std::string retval = vec.front()->string();

    for(std::size_t i = 1; i < vec.size(); i++)
    {
        retval += "&&";
        retval += vec[i]->string();
    }

    return retval;
}

std::string InclusiveORExpression::string() const
{
    std::string retval = vec.front()->string();

    for(std::size_t i = 1; i < vec.size(); i++)
    {
        retval.push_back('|');
        retval += vec[i]->string();
    }

    return retval;
}

std::string ExclusiveORExpression::string() const
{
    std::string retval = vec.front()->string();

    for(std::size_t i = 1; i < vec.size(); i++)
    {
        retval.push_back('^');
        retval += vec[i]->string();
    }

    return retval;
}

std::string ANDExpression::string() const
{
    std::string retval = vec.front()->string();

    for(std::size_t i = 1; i < vec.size(); i++)
    {
        retval.push_back('&');
        retval += vec[i]->string();
    }

    return retval;
}

std::string EqualityExpression::string() const
{
    std::string retval = relvec.front()->string();

    for(std::size_t i = 1; i < relvec.size(); i++)
    {
        retval += tagvec[i - 1]->string();
        retval += relvec[i]->string();
    }

    return retval;
}

std::string RelationalExpression::string() const
{
    std::string retval = sftvec.front()->string();

    for(std::size_t i = 1; i < sftvec.size(); i++)
    {
        retval += tagvec[i - 1]->string();
        retval += sftvec[i]->string();
    }

    return retval;
}

std::string ShiftExpression::string() const
{
    std::string retval = addvec.front()->string();

    for(std::size_t i = 1; i < addvec.size(); i++)
    {
        retval += tagvec[i - 1]->string();
        retval += addvec[i]->string();
    }

    return retval;
}

std::string AdditiveExpression::string() const
{
    std::string retval = mulvec.front()->string();

    for(std::size_t i = 1; i < mulvec.size(); i++)
    {
        retval += tagvec[i - 1]->string();
        retval += mulvec[i]->string();
    }

    return retval;
}

std::string MultiplicativeExpression::string() const
{
    std::string retval = castvec.front()->string();

    for(std::size_t i = 1; i < castvec.size(); i++)
    {
        retval += tagvec[i - 1]->string();
        retval += castvec[i]->string();
    }

    return retval;
}

std::string CastExpression::string() const
{
    std::string retval;

    switch(tag)
    {
        case(Tag::UNARY):
            retval += uni.unary->string();
            break;
        case(Tag::CAST):
            retval += "(type-name)";
            retval += uni.sCast.cast->string();
            break;
        
        default:
            Simbol::unexpectTag("CastExpression");
            break;
    }

    return retval;
}

std::string UnaryExpression::string() const
{
    std::string retval;

    switch(tag)
    {
        case(Tag::POSTFIX):
            retval += uni.postfix->string();
            break;
        case(Tag::INC):
            retval += uni.inc->string();
            break;
        case(Tag::DEC):
            retval += uni.dec->string();
            break;
        case(Tag::UNARY):
            retval += uni.sUnary.tag->string();
            retval += uni.sUnary.cast->string();
            break;
        case(Tag::SIZEOF_UNARY):
            retval += "sizeof ";
            retval += uni.sizeofUnary->string();
            break;
        case(Tag::SIZEOF_TYPE):
            retval += "sizeof(type-name)";
            break;
        case(Tag::ALIGNOF):
            retval += "_Alignof(type-name)";
            break;
        
        default:
            Simbol::unexpectTag("UnaryExpression");
            break;
    }

    return retval;
}

std::string PostfixExpression::string() const
{
    std::string retval;

    switch(tag)
    {
        case(Tag::PRIMARY):
            retval += uni.primary->string();
            break;
        
        default:
            Simbol::unexpectTag("PostfixExpression");
            break;
    }

    return retval;
}

std::string PrimaryExpression::string() const
{
    std::string retval;

    switch(tag)
    {
        case(Tag::IDENTIFIER):
            retval += uni.identifier->string();
            break;
        case(Tag::CONSTANT):
            retval += uni.constant->string();
            break;
        case(Tag::STRING_LITERAL):
            retval += uni.stringLiteral->string();
            break;
        case(Tag::EXPRESSION):
            retval.push_back('(');
            retval += uni.expression->string();
            retval.push_back(')');
            break;
        
        default:
            Simbol::unexpectTag("PrimaryExpression");
            break;
    }

    return retval;
}

}