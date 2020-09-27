#include <iostream>

#include "type.hpp"

namespace TP7
{

namespace TYPE
{

const std::unordered_map<BaseType::Tag, std::vector<std::multiset<TypeSpecifier::Tag>>> SPECIFIER_MAP
    = {{BaseType::Tag::VOID
        , {{TypeSpecifier::Tag::VOID}}}
        , {BaseType::Tag::CHAR
            , {{TypeSpecifier::Tag::CHAR}}}
        , {BaseType::Tag::S_CHAR
            , {{TypeSpecifier::Tag::SIGNED
                , TypeSpecifier::Tag::CHAR}}}
        , {BaseType::Tag::U_CHAR
            , {{TypeSpecifier::Tag::UNSIGNED
                , TypeSpecifier::Tag::CHAR}}}
        , {BaseType::Tag::S_SHORT
            , {{TypeSpecifier::Tag::SHORT}
                , {TypeSpecifier::Tag::SIGNED
                    , TypeSpecifier::Tag::SHORT}
                , {TypeSpecifier::Tag::SHORT
                    , TypeSpecifier::Tag::INT}
                , {TypeSpecifier::Tag::SIGNED
                    , TypeSpecifier::Tag::SHORT
                    , TypeSpecifier::Tag::INT}}}
        , {BaseType::Tag::U_SHORT
            , {{TypeSpecifier::Tag::UNSIGNED
                , TypeSpecifier::Tag::SHORT}
                , {TypeSpecifier::Tag::UNSIGNED
                    , TypeSpecifier::Tag::SHORT
                    , TypeSpecifier::Tag::INT}}}
        , {BaseType::Tag::S_INT
            , {{TypeSpecifier::Tag::INT}
                , {TypeSpecifier::Tag::SIGNED}
                , {TypeSpecifier::Tag::SIGNED
                    , TypeSpecifier::Tag::INT}}}
        , {BaseType::Tag::U_INT
            , {{TypeSpecifier::Tag::UNSIGNED}
                , {TypeSpecifier::Tag::UNSIGNED
                    , TypeSpecifier::Tag::INT}}}
        , {BaseType::Tag::S_LONG
            , {{TypeSpecifier::Tag::LONG}
                , {TypeSpecifier::Tag::SIGNED
                    , TypeSpecifier::Tag::LONG}
                , {TypeSpecifier::Tag::LONG
                    , TypeSpecifier::Tag::INT}
                , {TypeSpecifier::Tag::SIGNED
                    , TypeSpecifier::Tag::LONG
                    , TypeSpecifier::Tag::INT}}}
        , {BaseType::Tag::U_LONG
            , {{TypeSpecifier::Tag::UNSIGNED
                , TypeSpecifier::Tag::LONG}
                , {TypeSpecifier::Tag::UNSIGNED
                    , TypeSpecifier::Tag::LONG
                    , TypeSpecifier::Tag::INT}}}
        , {BaseType::Tag::S_LONG_LONG
            , {{TypeSpecifier::Tag::LONG
                , TypeSpecifier::Tag::LONG}
                , {TypeSpecifier::Tag::SIGNED
                    , TypeSpecifier::Tag::LONG
                    , TypeSpecifier::Tag::LONG}
                , {TypeSpecifier::Tag::LONG
                    , TypeSpecifier::Tag::LONG
                    , TypeSpecifier::Tag::INT}
                , {TypeSpecifier::Tag::SIGNED
                    , TypeSpecifier::Tag::LONG
                    , TypeSpecifier::Tag::LONG
                    , TypeSpecifier::Tag::INT}}}
        , {BaseType::Tag::U_LONG_LONG
            , {{TypeSpecifier::Tag::UNSIGNED
                , TypeSpecifier::Tag::LONG
                , TypeSpecifier::Tag::LONG}
                , {TypeSpecifier::Tag::UNSIGNED
                    , TypeSpecifier::Tag::LONG
                    , TypeSpecifier::Tag::LONG
                    , TypeSpecifier::Tag::INT}}}
        , {BaseType::Tag::FLOAT
            , {{TypeSpecifier::Tag::FLOAT}}}
        , {BaseType::Tag::DOUBLE
            , {{TypeSpecifier::Tag::DOUBLE}}}
        , {BaseType::Tag::LONG_DOUBLE
            , {{TypeSpecifier::Tag::LONG
                , TypeSpecifier::Tag::DOUBLE}}}
        , {BaseType::Tag::BOOL
            , {{TypeSpecifier::Tag::BOOL}}}};

Type *deduce(const DeclarationSpecifiers *declarationSpecifiers)
{
    std::vector<const TypeSpecifier*> tsvec;
    for(const DeclarationSpecifiers *ds = declarationSpecifiers;
        ds != nullptr;
        ds = ds->declarationSpecifiers)
    {
        switch(ds->tag)
        {
            case(DeclarationSpecifiers::Tag::TYPE_SPECIFIER):
                tsvec.push_back(ds->uni.typeSpecifier);
                break;
            
            default:;
        }
    }

    return deduce(tsvec);
}

Type *deduce(const Declarator *declarator, Type *derivedType)
{
    Type *ret = derivedType;

    for(Pointer *p = declarator->pointer;
        p != nullptr;
        p = p->pointer)
    {
        Type *tmp = ret;
        ret = new Type();
        ret->set(new PointerType())->referencedT = tmp;
    }

    for(auto &&iter = declarator->directDeclarator->sddvec.rbegin();
        iter != declarator->directDeclarator->sddvec.rend();
        iter++)
    {
        switch(iter->tag)
        {
            case(DirectDeclarator::Tag::IDENTIFIER):
                break;
            case(DirectDeclarator::Tag::DECLARATOR):
                ret = deduce(iter->uni.declarator, ret);
                break;
            case(DirectDeclarator::Tag::QUALIFIER_ASSIGNMENT):
            case(DirectDeclarator::Tag::STATIC_QUALIFIER_ASSIGNMENT):
            case(DirectDeclarator::Tag::QUALIFIER_STATIC_ASSIGNMENT):
            case(DirectDeclarator::Tag::QUALIFIER):
            {
                Type *tmp = ret;
                ret = new Type();
                ret->set(new ArrayType())->elementT = tmp;
                break;
            }
            case(DirectDeclarator::Tag::PARAMETER):
            {
                Type *tmp = ret;
                ret = new Type();
                ret->set(new FunctionType())->returnT = tmp;
                ret->uni.functionT->isVariable = iter->uni.parameter->tag == ParameterTypeList::Tag::VALRIABLE_PARALETER_LIST;
                for(auto &&pd : iter->uni.parameter->parameterList->pdvec)
                    ret->uni.functionT->argTypes.push_back(deduce(pd));
                break;
            }
            case(DirectDeclarator::Tag::IDENTIFIER_LIST):
            {
                Type *tmp = ret;
                ret = new Type();
                ret->set(new FunctionType())->returnT = tmp;
                if(iter->uni.identifierList != nullptr)
                {
                    std::cout << "=== not implements (IdentifierList) ===\n"
                        << std::flush;
                }
                break;
            }

            default:
                Simbol::unexpectTag("DrectDeclarator");
        }
    }

    return ret;
}

Type *deduce(const ParameterDeclaration *parameterDeclaration)
{
    Type *ret = deduce(parameterDeclaration->declarationSpecifiers);

    switch(parameterDeclaration->tag)
    {
        case(ParameterDeclaration::Tag::DECLARATOR):
            ret = deduce(parameterDeclaration->uni.declarator, ret);
            break;
        case(ParameterDeclaration::Tag::ABSTRACT_DECLARATOR):
            if(parameterDeclaration->uni.abstractDeclarator != nullptr)
                ret = deduce(parameterDeclaration->uni.abstractDeclarator, ret);
            break;
        
        default:
            Simbol::unexpectTag("ParameterDeclaration");
    }

    return ret;
}

Type *deduce(const AbstractDeclarator *abstractDeclarator, Type *derivedType)
{
    Type *ret = derivedType;

    for(Pointer *p = abstractDeclarator->pointer;
        p != nullptr;
        p = p->pointer)
    {
        Type *tmp = ret;
        ret = new Type();
        ret->set(new PointerType())->referencedT = tmp;
    }

    for(auto &&iter = abstractDeclarator->declarator->sdadvec.rbegin();
        iter != abstractDeclarator->declarator->sdadvec.rend();
        iter++)
    {
        switch(iter->tag)
        {
            case(DirectAbstractDeclarator::Tag::ABSTRACT):
                ret = deduce(iter->uni.abstract, ret);
                break;
            case(DirectAbstractDeclarator::Tag::QUALIFIER_ASSIGNMENT):
            case(DirectAbstractDeclarator::Tag::STATIC_QUALIFIER_ASSIGNMENT):
            case(DirectAbstractDeclarator::Tag::QUALIFIER_STATIC_ASSIGNMENT):
            case(DirectAbstractDeclarator::Tag::ASTERISK):
            {
                Type *tmp = ret;
                ret = new Type();
                ret->set(new ArrayType())->elementT = tmp;
                break;
            }
            case(DirectAbstractDeclarator::Tag::PARAMETER):
            {
                Type *tmp = ret;
                ret = new Type();
                ret->set(new FunctionType())->returnT = tmp;
                if(iter->uni.parameter != nullptr)
                {
                    ret->uni.functionT->isVariable = iter->uni.parameter->tag == ParameterTypeList::Tag::VALRIABLE_PARALETER_LIST;
                    for(auto &&pd : iter->uni.parameter->parameterList->pdvec)
                        ret->uni.functionT->argTypes.push_back(deduce(pd));
                }
                break;
            }

            default:
                Simbol::unexpectTag("DrectDeclarator");
        }
    }    

    return ret;
}

Type *deduce(const std::vector<const TypeSpecifier*> &tsvec)
{
    Type ret;

    std::multiset<TypeSpecifier::Tag> tagSet;
    for(auto &&ts : tsvec)
        tagSet.insert(ts->tag);
    
    BaseType::Tag baseTag = BaseType::Tag::NONE;
    for(auto &&mapIter = SPECIFIER_MAP.begin(); mapIter != SPECIFIER_MAP.end(); mapIter++)
    {
        for(auto &&set : mapIter->second)
        {
            if(tagSet == set)
            {
                baseTag = mapIter->first;
                break;
            }
        }

        if(baseTag != BaseType::Tag::NONE)
            break;
    }

    if(baseTag != BaseType::Tag::NONE)
    {
        ret.tag = Type::Tag::BASE;
        ret.uni.baseT = new BaseType();
        ret.uni.baseT->tag = baseTag;
    }

    if(ret.tag != Type::Tag::NONE)
        return new Type(ret);
    else
        return nullptr;
}

}

}