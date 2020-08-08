#include "function_definition_simbol.hpp"

#include "declaration_simbol.hpp"
#include "expression_simbol.hpp"

namespace TP7
{

std::string TranslationUnit::string() const
{
    std::string ret;

    for(auto &&ed : edvec)
        ret += ed->string();

    return ret;
}

std::string ExternalDeclaration::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::FUNCTION_DEFINITION):
            ret += uni.functionDefinition->string();
            break;
        case(Tag::DECLARATION):
            ret += uni.declaration->string();
            break;
        
        default:
            Simbol::unexpectTag("ExternalDeclaration");
            break;
    }

    return ret;
}

std::string FunctionDefinition::string() const
{
    std::string ret;

    ret += declarationSpecifiers->string();
    ret += declarator->string();
    if(declarationList != nullptr)
        ret += declarationList->string();
    ret += compoundStatement->string();

    return ret;
}

std::string DeclarationList::string() const
{
    std::string ret;

    for(auto &&d : dvec)
        ret += d->string();
    
    return ret;
}

std::string CompoundStatement::string() const
{
    std::string ret(1, '{');

    if(blockItemList != nullptr)
        ret += blockItemList->string();
    
    ret.push_back('}');

    return ret;
}

std::string BlockItemList::string() const
{
    std::string ret;
    
    for(auto &&bi : bivec)
        ret += bi->string();

    return ret;
}

std::string BlockItem::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::DECLARATION):
            ret += uni.declaration->string();
            break;
        case(Tag::STATEMENT):
            ret += uni.statement->string();
            break;
        
        default:
            Simbol::unexpectTag("BlockItem");
            break;
    }

    return ret;
}

std::string Statement::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::LABELED):
            ret += uni.labeled->string();
            break;
        case(Tag::COMPOUND):
            ret += uni.compound->string();
            break;
        case(Tag::EXPRESSION):
            ret += uni.expression->string();
            break;
        case(Tag::SELECTION):
            ret += uni.selection->string();
            break;
        case(Tag::ITERATION):
            ret += uni.iteration->string();
            break;
        case(Tag::JUMP):
            ret += uni.jump->string();
            break;
        
        default:
            Simbol::unexpectTag("Statement");
            break;
    }

    return ret;
}

std::string LabeledStatement::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::IDENTIFIER):
            ret += uni.sIdentifier.identifier->string();
            ret.push_back(':');
            ret += uni.sIdentifier.statement->string();
            break;
        case(Tag::CASE):
            ret += "case ";
            ret += uni.sCase.constant->string();
            ret.push_back(':');
            ret += uni.sCase.statement->string();
            break;
        case(Tag::DEFAULT):
            ret += "default:";
            ret += uni.defaultStatement->string();
            break;
        
        default:
            Simbol::unexpectTag("LabeledStatement");
            break;
    }

    return ret;
}

std::string ExpressionStatement::string() const
{
    std::string ret;

    if(expression != nullptr)
        ret += expression->string();

    return ret;
}

std::string SelectionStatement::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::IF):
            ret += "if(";
            ret += uni.sIf.expression->string();
            ret.push_back(')');
            ret += uni.sIf.statement->string();
            break;
        case(Tag::IF_ELSE):
            ret += "if(";
            ret += uni.sIfElse.expression->string();
            ret.push_back(')');
            ret += uni.sIfElse.ifStatement->string();
            ret += "else";
            ret += uni.sIfElse.elseStatement->string();
            break;
        case(Tag::SWITCH):
            ret += "switch(";
            ret += uni.sSwitch.expression->string();
            ret.push_back(')');
            ret += uni.sSwitch.statement->string();
            break;
        
        default:
            Simbol::unexpectTag("SelectionStatement");
            break;
    }

    return ret;
}

std::string IterationStatement::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::WHILE):
            ret += "while(";
            ret += uni.sWhile.expression->string();
            ret.push_back(')');
            ret += uni.sWhile.statement->string();
            break;
        case(Tag::DO_WHILE):
            ret += "do";
            ret += uni.sDoWhile.statement->string();
            ret += "while(";
            ret += uni.sDoWhile.expression->string();
            ret += ");";
            break;
        case(Tag::FOR_EXPRESSION):
            ret += "for(";
            for(auto &&e : uni.sForExpression.expressionArr)
            {
                if(e != nullptr)
                    ret += e->string();
                ret.push_back(';');
            }
            ret.pop_back();
            ret.push_back(')');
            ret += uni.sForExpression.statement->string();
            break;
        case(Tag::FOR_DECLARATION):
            ret += "for(";
            ret += uni.sForDeclaration.declaration->string();
            for(auto &&e : uni.sForDeclaration.expressionArr)
            {
                if(e != nullptr)
                    ret += e->string();
                ret.push_back(';');
            }
            ret.pop_back();
            ret.push_back(';');
            ret += uni.sForDeclaration.statement->string();
            break;

        default:
            Simbol::unexpectTag("IterationStatement");
            break;
    }

    return ret;
}

std::string JumpStatement::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::GOTO):
            ret += "goto ";
            ret += uni.gotoIdentifier->string();
            ret.push_back(';');
            break;
        case(Tag::CONTINUE):
            ret += "continue;";
            break;
        case(Tag::BREAK):
            ret += "break;";
            break;
        case(Tag::RETURN):
            ret += "return";
            if(uni.returnExpression != nullptr)
            {
                ret.push_back(' ');
                ret += uni.returnExpression->string();
            }
            ret.push_back(';');
            break;
        
        default:
            Simbol::unexpectTag("JumpStatement");
            break;
    }

    return ret;
}

}