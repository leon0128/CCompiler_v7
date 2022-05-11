#include <iostream>

#include "type.hpp"

namespace TP7
{

std::size_t Type::size() const
{
    std::size_t ret = 0;

    switch(tag)
    {
        case(Tag::BASE):
            ret = uni.baseT->size();
            break;
        case(Tag::FUNCTION):
            ret = uni.functionT->size();
            break;
        case(Tag::POINTER):
            ret = uni.pointerT->size();
            break;
        case(Tag::BITFIELD):
            ret = uni.bitfieldT->size();
            break;
        case(Tag::STRUCT):
            ret = uni.structT->size();
            break;
        case(Tag::UNION):
            ret = uni.unionT->size();
            break;
        case(Tag::ARRAY):
            ret = uni.arrayT->size();
            break;
        case(Tag::ENUMERATION):
            ret = uni.enumerationT->size();
            break;

        default:
            Simbol::unexpectTag("Type");
    }

    return ret;
}

std::size_t Type::alignment() const
{
    std::size_t ret = 0;

    switch(tag)
    {
        case(Tag::BASE):
            ret = uni.baseT->alignment();
            break;
        case(Tag::FUNCTION):
            ret = uni.functionT->alignment();
            break;
        case(Tag::POINTER):
            ret = uni.pointerT->alignment();
            break;
        case(Tag::BITFIELD):
            ret = uni.bitfieldT->alignment();
            break;
        case(Tag::STRUCT):
            ret = uni.structT->alignment();
            break;
        case(Tag::UNION):
            ret = uni.unionT->alignment();
            break;
        case(Tag::ARRAY):
            ret = uni.arrayT->alignment();
            break;
        case(Tag::ENUMERATION):
            ret = uni.enumerationT->alignment();
            break;

        default:
            Simbol::unexpectTag("Type");
    }

    return ret;
}

bool Type::isSigned() const
{
    bool ret = false;

    switch(tag)
    {
        case(Type::Tag::BASE):
        {
            switch(uni.baseT->tag)
            {
                case(BaseType::Tag::CHAR):
                case(BaseType::Tag::S_CHAR):
                case(BaseType::Tag::S_SHORT):
                case(BaseType::Tag::S_INT):
                case(BaseType::Tag::S_LONG):
                case(BaseType::Tag::S_LONG_LONG):
                    ret = true;
                    break;
                
                default:;
                    ret = false;
            }
            break;
        }
        case(Type::Tag::ENUMERATION):
            ret = true;
            break;
        
        default:
            ret = false;
    }

    return ret;
}

bool Type::isUnsigned() const
{
    
}

std::size_t BaseType::size() const
{
    std::size_t ret = 0;

    switch(tag)
    {
        case(Tag::VOID):
            ret = sizeof(void);
            break;
        case(Tag::CHAR):
            ret = sizeof(char);
            break;
        case(Tag::S_CHAR):
            ret = sizeof(signed char);
            break;
        case(Tag::U_CHAR):
            ret = sizeof(unsigned char);
            break;
        case(Tag::S_SHORT):
            ret = sizeof(signed short);
            break;
        case(Tag::U_SHORT):
            ret = sizeof(unsigned short);
            break;
        case(Tag::S_INT):
            ret = sizeof(signed int);
            break;
        case(Tag::U_INT):
            ret = sizeof(unsigned int);
            break;
        case(Tag::S_LONG):
            ret = sizeof(signed long);
            break;
        case(Tag::U_LONG):
            ret = sizeof(unsigned long);
            break;
        case(Tag::S_LONG_LONG):
            ret = sizeof(signed long long);
            break;
        case(Tag::U_LONG_LONG):
            ret = sizeof(unsigned long long);
            break;
        case(Tag::FLOAT):
            ret = sizeof(float);
            break;
        case(Tag::DOUBLE):
            ret = sizeof(double);
            break;
        case(Tag::LONG_DOUBLE):
            ret = sizeof(long double);
            break;
        case(Tag::BOOL):
            ret = sizeof(bool);
            break;
        
        default:
            Simbol::unexpectTag("BaseType");
    }

    return ret;
}

std::size_t BaseType::alignment() const
{
    std::size_t ret = 0;

    switch(tag)
    {
        case(Tag::VOID):
            ret = alignof(void);
            break;
        case(Tag::CHAR):
            ret = alignof(char);
            break;
        case(Tag::S_CHAR):
            ret = alignof(signed char);
            break;
        case(Tag::U_CHAR):
            ret = alignof(unsigned char);
            break;
        case(Tag::S_SHORT):
            ret = alignof(signed short);
            break;
        case(Tag::U_SHORT):
            ret = alignof(unsigned short);
            break;
        case(Tag::S_INT):
            ret = alignof(signed int);
            break;
        case(Tag::U_INT):
            ret = alignof(unsigned int);
            break;
        case(Tag::S_LONG):
            ret = alignof(signed long);
            break;
        case(Tag::U_LONG):
            ret = alignof(unsigned long);
            break;
        case(Tag::S_LONG_LONG):
            ret = alignof(signed long long);
            break;
        case(Tag::U_LONG_LONG):
            ret = alignof(unsigned long long);
            break;
        case(Tag::FLOAT):
            ret = alignof(float);
            break;
        case(Tag::DOUBLE):
            ret = alignof(double);
            break;
        case(Tag::LONG_DOUBLE):
            ret = alignof(long double);
            break;
        case(Tag::BOOL):
            ret = alignof(bool);
            break;
        
        default:
            Simbol::unexpectTag("BaseType");
    }

    return ret;
}

std::size_t FunctionType::size() const
{
    return sizeof(void());
}

std::size_t FunctionType::alignment() const
{
    return alignof(void());
}

std::size_t PointerType::size() const
{
    return sizeof(void*);
}

std::size_t PointerType::alignment() const
{
    return alignof(void*);
}

std::size_t BitfieldType::size() const
{
    std::cerr << "Type error:\n"
        "    what: invalid application of size to bit-field.\n"
        << std::flush;

    return 0;
}

std::size_t BitfieldType::alignment() const
{
    std::cerr << "Type error:\n"
        "    what: invalid application of size to bit-field.\n"
        << std::flush;

    return 0;
}

std::size_t StructType::size() const
{
    std::size_t ret = 0;

    std::cout << "=== not implements(StructType::size()) ===\n"
        << std::flush;
    
    return ret;
}

std::size_t StructType::alignment() const
{
    std::size_t ret = 0;

    std::cout << "=== not implements(StructType::alignment()) ===\n"
        << std::flush;
    
    return ret;
}

std::size_t UnionType::size() const
{
    std::size_t ret = 0;

    std::cout << "=== not implements(UnionType::size()) ===\n"
        << std::flush;
    
    return ret;
}

std::size_t UnionType::alignment() const
{
    std::size_t ret = 0;

    std::cout << "=== not implements(UnionType::alignment()) ===\n"
        << std::flush;
    
    return ret;
}

std::size_t ArrayType::size() const
{
    return elementT->size() * arraySize;
}

std::size_t ArrayType::alignment() const
{
    return elementT->alignment();
}

std::size_t EnumerationType::size() const
{
    return sizeof(int);
}

std::size_t EnumerationType::alignment() const
{
    return alignof(int);
}

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

const std::unordered_map<BaseType::Tag, std::ptrdiff_t> CONVERSION_RANK
    = {{BaseType::Tag::NONE, -1}
        , {BaseType::Tag::BOOL, 0}
        , {BaseType::Tag::CHAR, 1}
        , {BaseType::Tag::S_CHAR, 1}
        , {BaseType::Tag::U_CHAR, 1}
        , {BaseType::Tag::S_SHORT, 2}
        , {BaseType::Tag::U_SHORT, 2}
        , {BaseType::Tag::S_INT, 3}
        , {BaseType::Tag::U_INT, 3}
        , {BaseType::Tag::S_LONG, 4}
        , {BaseType::Tag::U_LONG, 4}
        , {BaseType::Tag::S_LONG_LONG, 5}
        , {BaseType::Tag::U_LONG_LONG, 5}
        , {BaseType::Tag::FLOAT, 6}
        , {BaseType::Tag::DOUBLE, 7}
        , {BaseType::Tag::LONG_DOUBLE, 8}};

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

Type *deduce(const TypeName *typeName)
{
    Type *ret = deduce(typeName->specifierQualifierList);

    return typeName->abstractDeclarator != nullptr
        ? deduce(typeName->abstractDeclarator, ret)
            : ret;
}

Type *deduce(const SpecifierQualifierList *specifierQualifierList)
{
    std::vector<const TypeSpecifier*> tsvec;

    for(const SpecifierQualifierList *sql = specifierQualifierList;
        sql != nullptr;
        sql = sql->specifierQualifierList)
    {
        switch(sql->tag)
        {
            case(SpecifierQualifierList::Tag::SPECIFIER):
                tsvec.push_back(sql->uni.specifier);
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
                Simbol::unexpectTag("DirectDeclarator");
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

Type *promote(const Type *lhs, const Type *rhs)
{
    Type *ret = nullptr;

    std::ptrdiff_t lRank = getConversionRank(lhs)
        , rRank = getConversionRank(rhs);
    
    const Type *priorityType = nullptr;
    if(lRank > rRank)
        priorityType = lhs;
    else if(lRank < rRank)
        priorityType = rhs;

    if(lRank != rRank)
        // return new
}

std::ptrdiff_t getConversionRank(const Type *type)
{
    std::ptrdiff_t ret = 0;

    switch(type->tag)
    {
        case(Type::Tag::BASE):
            ret = CONVERSION_RANK[type->uni.baseT->tag];
            break;
        case(Type::Tag::ENUMERATION):
            ret = CONVERSION_RANK[BaseType::Tag::S_INT];
            break;
        
        default:
            ret = -1;
    }

    return ret;
}

}

}