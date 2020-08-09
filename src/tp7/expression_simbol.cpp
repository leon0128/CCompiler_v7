#include "declaration_simbol.hpp"
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
            retval += uni.sAssignment.assignmentOperator->string();
            retval += uni.sAssignment.assignment->string();
            break;
        
        default:
            Simbol::unexpectTag("AssignmentExpression");
            break;
    }

    return retval;
}

std::string AssignmentOperator::string() const
{
    std::string ret;
    
    switch(tag)
    {
        case(Tag::ASS):
            ret.push_back('=');
            break;
        case(Tag::MUL):
            ret += "*=";
            break;
        case(Tag::DIV):
            ret += "/=";
            break;
        case(Tag::REM):
            ret += "%=";
            break;
        case(Tag::PLUS):
            ret += "+=";
            break;
        case(Tag::MINUS):
            ret += "-=";
            break;
        case(Tag::LSFT):
            ret += "<<=";
            break;
        case(Tag::RSFT):
            ret += ">>=";
            break;
        case(Tag::AND):
            ret += "&=";
            break;
        case(Tag::INOR):
            ret += "|=";
            break;
        case(Tag::EXOR):
            ret += "^=";
            break;
        
        default:
            Simbol::unexpectTag("AssignmentOperator");
            break;
    }

    return ret;
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
    std::string ret = seevec.front().re->string();

    for(std::size_t i = 1; i < seevec.size(); i++)
    {
        switch(seevec[i].tag)
        {
            case(Tag::EQUAL):
                ret += "==";
                break;
            case(Tag::NOT_EQUAL):
                ret += "!=";
                break;
            
            default:
                Simbol::unexpectTag("EqualityExpression");
                break;
        }

        ret += seevec[i].re->string();
    }

    return ret;
}

std::string RelationalExpression::string() const
{
    std::string ret = srevec.front().se->string();

    for(std::size_t i = 1; i < srevec.size(); i++)
    {
        switch(srevec[i].tag)
        {
            case(Tag::LESS):
                ret.push_back('<');
                break;
            case(Tag::GREATER):
                ret.push_back('>');
                break;
            case(Tag::LESS_EQ):
                ret += "<=";
                break;
            case(Tag::GREATER_EQ):
                ret += ">=";
                break;
            
            default:
                Simbol::unexpectTag("RelationalExpression");
                break;
        }

        ret += srevec[i].se->string();
    }

    return ret;
}

std::string ShiftExpression::string() const
{
    std::string ret = ssevec.front().ae->string();

    for(std::size_t i = 1; i < ssevec.size(); i++)
    {
        switch(ssevec[i].tag)
        {
            case(Tag::L_SHIFT):
                ret += "<<";
                break;
            case(Tag::R_SHIFT):
                ret += ">>";
                break;
            
            default:
                Simbol::unexpectTag("ShiftExpression");
                break;
        }

        ret += ssevec[i].ae->string();
    }

    return ret;
}

std::string AdditiveExpression::string() const
{
    std::string ret = saevec.front().me->string();

    for(std::size_t i = 1; i < saevec.size(); i++)
    {
        switch(saevec[i].tag)
        {
            case(Tag::ADD):
                ret.push_back('+');
                break;
            case(Tag::SUB):
                ret.push_back('-');
                break;
            
            default:
                Simbol::unexpectTag("AdditiveExpression");
                break;
        }

        ret += saevec[i].me->string();
    }

    return ret;
}

std::string MultiplicativeExpression::string() const
{
    std::string ret = smevec.front().ce->string();

    for(std::size_t i = 1; i < smevec.size(); i++)
    {
        switch(smevec[i].tag)
        {
            case(Tag::MUL):
                ret.push_back('*');
                break;
            case(Tag::DIV):
                ret.push_back('/');
                break;
            case(Tag::REM):
                ret.push_back('%');
                break;

            default:
                Simbol::unexpectTag("MultiplicativeExpression");
                break;
        }

        ret += smevec[i].ce->string();
    }

    return ret;
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
            retval.push_back('(');
            retval += uni.sCast.typeName->string();
            retval.push_back(')');
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
            retval += uni.sUnary.unaryOperator->string();
            retval += uni.sUnary.cast->string();
            break;
        case(Tag::SIZEOF_UNARY):
            retval += "sizeof ";
            retval += uni.sizeofUnary->string();
            break;
        case(Tag::SIZEOF_TYPE):
            retval += "sizeof(";
            retval += uni.sizeofType->string();
            retval.push_back(')');
            break;
        case(Tag::ALIGNOF):
            retval += "_Alignof(";
            retval += uni.alignofType->string();
            retval.push_back(')');
            break;
        
        default:
            Simbol::unexpectTag("UnaryExpression");
            break;
    }

    return retval;
}

std::string UnaryOperator::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::AND):
            ret.push_back('&');
            break;
        case(Tag::AST):
            ret.push_back('*');
            break;
        case(Tag::PLUS):
            ret.push_back('+');
            break;
        case(Tag::MINUS):
            ret.push_back('-');
            break;
        case(Tag::TILDE):
            ret.push_back('~');
            break;
        case(Tag::EXC):
            ret.push_back('!');
            break;
        
        default:
            Simbol::unexpectTag("UnaryOperator");
            break;
    }

    return ret;
}

std::string PostfixExpression::string() const
{
    std::string retval;

    for(auto &&spe : spevec)
    {
        switch(spe.tag)
        {
            case(Tag::PRIMARY):
                retval += spe.uni.primary->string();
                break;
            case(Tag::EXPRESSION):
                retval.push_back('[');
                retval += spe.uni.expression->string();
                retval.push_back(']');
                break;
            case(Tag::ARGUMENT):
                retval.push_back('(');
                if(spe.uni.argument != nullptr)
                    retval += spe.uni.argument->string();
                retval.push_back(')');
                break;
            case(Tag::DOT_IDENTIFIER):
                retval.push_back('.');
                retval += spe.uni.dotIdentifier->string();
                break;
            case(Tag::ARROW_IDENTIFIER):
                retval += "->";
                retval += spe.uni.arrowIdentifier->string();
                break;
            case(Tag::INC):
                retval += "++";
                break;
            case(Tag::DEC):
                retval += "--";
                break;
            case(Tag::TYPE_NAME):
                retval.push_back('(');
                retval += spe.uni.sTypeName.typeName->string();
                retval += "){";
                retval += spe.uni.sTypeName.initializer->string();
                retval.push_back('}');
                break;
            
            default:
                Simbol::unexpectTag("PostfixExpression");
                break;
        }
    }

    return retval;
}

std::string ArgumentExpressionList::string() const
{
    std::string ret;

    for(auto &&ae : aevec)
    {
        ret += ae->string();
        ret.push_back(',');
    }

    ret.pop_back();
    return ret;
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
        case(Tag::GENERIC_SELECTION):
            retval += uni.genericSelection->string();
            break;

        default:
            Simbol::unexpectTag("PrimaryExpression");
            break;
    }

    return retval;
}

std::string GenericSelection::string() const
{
    std::string ret("_Generic(");
    ret += assignment->string();
    ret.push_back(',');
    ret += generic->string();
    ret.push_back(')');
    
    return ret;
}

std::string GenericAssocList::string() const
{
    std::string ret;

    for(auto &&ga : gavec)
    {
        ret += ga->string();
        ret.push_back(',');
    }

    ret.pop_back();
    return ret;
}

std::string GenericAssociation::string() const
{
    std::string ret;

    if(typeName != nullptr)
        ret += typeName->string();
    else
        ret += "default";
    
    ret.push_back(':');
    ret += assignment->string();

    return ret;
}

}