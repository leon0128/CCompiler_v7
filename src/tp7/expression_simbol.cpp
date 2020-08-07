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

    switch(tag)
    {
        case(Tag::PRIMARY):
            retval += uni.primary->string();
            break;
        case(Tag::POSTFIX_EXPRESSION):
            retval += uni.sPostfixExpression.postfix->string();
            retval.push_back('[');
            retval += uni.sPostfixExpression.expression->string();
            retval.push_back(']');
            break;
        case(Tag::POSTFIX_ARGUMENT):
            retval += uni.sPostfixArgument.postfix->string();
            retval.push_back('(');
            if(uni.sPostfixArgument.argument != nullptr)
                retval += uni.sPostfixArgument.argument->string();
            retval.push_back(')');
            break;
        case(Tag::POSTFIX_DOT_IDENTIFIER):
            retval += uni.sPostfixDotIdentifier.postfix->string();
            retval.push_back('.');
            retval += uni.sPostfixDotIdentifier.identifier->string();
            break;
        case(Tag::POSTFIX_ARROW_IDENTIFIER):
            retval += uni.sPostfixArrowIdentifier.postfix->string();
            retval += "->";
            retval += uni.sPostfixArrowIdentifier.identifier->string();
            break;
        case(Tag::POSTFIX_INC):
            retval += uni.postfixInc->string();
            retval += "++";
            break;
        case(Tag::POSTFIX_DEC):
            retval += uni.postfixDec->string();
            retval += "--";
            break;
        case(Tag::TYPE_NAME):
            retval.push_back('(');
            retval += uni.sTypeName.typeName->string();
            retval += "){";
            retval += uni.sTypeName.initializer->string();
            retval.push_back('}');
            break;
        
        default:
            Simbol::unexpectTag("PostfixExpression");
            break;
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