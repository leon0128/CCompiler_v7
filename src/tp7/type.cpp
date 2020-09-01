#include "type.hpp"

namespace TP7
{

Type::Type() noexcept
    : tag(Tag::NONE)
    , uni{nullptr}
{
}

Type::Type(const Type &src)
    : tag(src.tag)
    , uni{nullptr}
{
    switch(tag)
    {
        case(Tag::NONE):
            uni.bt = nullptr;
            break;
        case(Tag::BASE):
            uni.bt = new BaseType(*src.uni.bt);
            break;
        case(Tag::STRUCT_OR_UNION):
            uni.sut = new StructOrUnionType(*src.uni.sut);
            break;
        case(Tag::ENUMERATION):
            uni.et = new EnumerationType(*src.uni.et);
            break;
        case(Tag::TYPEDEF):
            uni.tt = new TypedefType(*src.uni.tt);
            break;
        case(Tag::ARRAY):
            uni.at = new ArrayType(*src.uni.at);
            break;
        case(Tag::POINTER):
            uni.pt = new PointerType(*src.uni.pt);
            break;
        case(Tag::FUNCTION):
            uni.ft = new FunctionType(*src.uni.ft);
            break;
    }
}

Type::~Type() noexcept
{
    switch(tag)
    {
        case(Tag::NONE):
            break;
        case(Tag::BASE):
            delete uni.bt;
            break;
        case(Tag::STRUCT_OR_UNION):
            delete uni.sut;
            break;
        case(Tag::ENUMERATION):
            delete uni.et;
            break;
        case(Tag::TYPEDEF):
            delete uni.tt;
            break;
        case(Tag::ARRAY):
            delete uni.at;
            break;
        case(Tag::POINTER):
            delete uni.pt;
            break;
        case(Tag::FUNCTION):
            delete uni.ft;
            break;
    }
}

bool Type::isIntegerType() const noexcept
{
    bool ret = false;
    
    switch(tag)
    {
        case(Tag::BASE):
            switch(uni.bt->tag)
            {
                case(BaseType::Tag::CHAR):
                case(BaseType::Tag::S_CHAR):
                case(BaseType::Tag::U_CHAR):
                case(BaseType::Tag::S_SHORT):
                case(BaseType::Tag::U_SHORT):
                case(BaseType::Tag::S_INT):
                case(BaseType::Tag::U_INT):
                case(BaseType::Tag::S_LONG):
                case(BaseType::Tag::U_LONG):
                case(BaseType::Tag::S_LONG_LONG):
                case(BaseType::Tag::U_LONG_LONG):
                case(BaseType::Tag::BOOL):
                    ret = true;
                    break;
                
                default:
                    break;
            }
            break;
        case(Tag::ENUMERATION):
            ret = true;
            break;
        case(Tag::TYPEDEF):
            ret = uni.tt->type->isIntegerType();
            break;
        
        default:
            break;
    }

    return ret;
}

bool Type::isSignedIntegerType() const noexcept
{
    bool ret = false;

    switch(tag)
    {
        case(Tag::BASE):
            switch(uni.bt->tag)
            {
                case(BaseType::Tag::CHAR):
                case(BaseType::Tag::S_CHAR):
                case(BaseType::Tag::S_SHORT):
                case(BaseType::Tag::S_INT):
                case(BaseType::Tag::S_LONG):
                case(BaseType::Tag::S_LONG_LONG):
                    ret = true;
                    break;
                
                default:
                    break;
            }
            break;
        case(Tag::ENUMERATION):
            ret = true;
            break;
        case(Tag::TYPEDEF):
            ret = uni.tt->type->isSignedIntegerType();
            break;
        
        default:
            break;
    }

    return ret;
}

bool Type::isUnsignedIntegerType() const noexcept
{
    bool ret = false;

    switch(tag)
    {
        case(Tag::BASE):
            switch(uni.bt->tag)
            {
                case(BaseType::Tag::U_CHAR):
                case(BaseType::Tag::U_SHORT):
                case(BaseType::Tag::U_INT):
                case(BaseType::Tag::U_LONG):
                case(BaseType::Tag::U_LONG_LONG):
                case(BaseType::Tag::BOOL):
                    ret = true;
                    break;

                default:
                    break;
            }
            break;
        case(Tag::TYPEDEF):
            ret = uni.tt->type->isUnsignedIntegerType();
            break;
        
        default:
            break;
    }

    return ret;
}

bool Type::isFloatingType() const noexcept
{
    bool ret = false;

    switch(tag)
    {
        case(Tag::BASE):
            switch(uni.bt->tag)
            {
                case(BaseType::Tag::FLOAT):
                case(BaseType::Tag::DOUBLE):
                case(BaseType::Tag::LONG_DOUBLE):
                case(BaseType::Tag::FLOAT_COMPLEX):
                case(BaseType::Tag::DOUBLE_COMPLEX):
                case(BaseType::Tag::LONG_DOUBLE_COMPLEX):
                    ret = true;
                    break;
                
                default:
                    break;
            }
            break;
        case(Tag::TYPEDEF):
            ret = uni.tt->type->isFloatingType();
            break;
        
        default:
            break;
    }

    return ret;
}

bool Type::isArithmeticType() const noexcept
{
    return isIntegerType() || isFloatingType();
}

bool Type::isComplexType() const noexcept
{
    bool ret = false;

    switch(tag)
    {
        case(Tag::BASE):
            switch(uni.bt->tag)
            {
                case(BaseType::Tag::FLOAT_COMPLEX):
                case(BaseType::Tag::DOUBLE_COMPLEX):
                case(BaseType::Tag::LONG_DOUBLE_COMPLEX):
                    ret = true;
                    break;
                
                default:
                    break;
            }
            break;
        case(Tag::TYPEDEF):
            ret = uni.tt->type->isComplexType();
            break;
        
        default:
            break;
    }

    return ret;
}

int Type::integerConversionRank() const noexcept
{
    int ret = 0;

    if(!isIntegerType())
        return 0;

    switch(tag)
    {
        case(Tag::BASE):
            switch(uni.bt->tag)
            {
                case(BaseType::Tag::CHAR):
                case(BaseType::Tag::S_CHAR):
                case(BaseType::Tag::U_CHAR):
                    ret = 2;
                    break;
                case(BaseType::Tag::S_SHORT):
                case(BaseType::Tag::U_SHORT):
                    ret = 3;
                    break;
                case(BaseType::Tag::S_INT):
                case(BaseType::Tag::U_INT):
                    ret = 4;
                    break;
                case(BaseType::Tag::S_LONG):
                case(BaseType::Tag::U_LONG):
                    ret = 5;
                    break;
                case(BaseType::Tag::S_LONG_LONG):
                case(BaseType::Tag::U_LONG_LONG):
                    ret = 6;
                    break;
                case(BaseType::Tag::BOOL):
                    ret = 1;
                    break;
                
                default:
                    break;
            }
            break;
        case(Tag::ENUMERATION):
            ret = 4;
            break;
        case(Tag::TYPEDEF):
            ret = uni.tt->type->integerConversionRank();
            break;
        
        default:
            break;
    }

    return ret;
}

int Type::floatingConversionRank() const noexcept
{
    int ret = 0;

    switch(tag)
    {
        case(Tag::BASE):
            switch(uni.bt->tag)
            {
                case(BaseType::Tag::FLOAT):
                case(BaseType::Tag::FLOAT_COMPLEX):
                    ret = 1;
                    break;
                case(BaseType::Tag::DOUBLE):
                case(BaseType::Tag::DOUBLE_COMPLEX):
                    ret = 2;
                    break;
                case(BaseType::Tag::LONG_DOUBLE):
                case(BaseType::Tag::LONG_DOUBLE_COMPLEX):
                    ret = 3;
                    break;
                
                default:
                    break;
            }
            break;
        case(Tag::TYPEDEF):
            ret = uni.tt->type->floatingConversionRank();
            break;
        
        default:
            break;
    }

    return ret;
}

Type *Type::usualArithmeticConversion(const Type *lhs, const Type *rhs) const noexcept
{
    Type *ret = nullptr;

    if(!lhs->isArithmeticType()
        || !rhs->isArithmeticType())
        return new Type();
    
    // not floating
    if(lhs->isIntegerType()
        && rhs->isIntegerType())
    {
        int lrank = lhs->integerConversionRank();
        int rrank = rhs->integerConversionRank();

        if(lrank < rrank)
            ret = new Type(*rhs);
        else if(lrank > rrank)
            ret = new Type(*lhs);
        else
        {
            if(lhs->isSignedIntegerType())
                ret = new Type(*rhs);
            else
                ret =  new Type(*lhs);
        }
    }
    // floating
    else
    {
        bool isComplex = lhs->isComplexType() || rhs->isComplexType();
        int lrank = lhs->floatingConversionRank();
        int rrank = rhs->floatingConversionRank();
        int rank = lrank >= rrank ? lrank : rrank;

        BaseType *bt = new BaseType();
        switch(rank)
        {
            case(1):
                bt->tag = isComplex ? BaseType::Tag::FLOAT_COMPLEX : BaseType::Tag::FLOAT;
                break;
            case(2):
                bt->tag = isComplex ? BaseType::Tag::DOUBLE_COMPLEX : BaseType::Tag::DOUBLE;
                break;
            case(3):
                bt->tag = isComplex ? BaseType::Tag::LONG_DOUBLE_COMPLEX : BaseType::Tag::LONG_DOUBLE;
                break;
            
            default:
                break;
        }

        ret = new Type();
        ret->tag = Tag::BASE;
        ret->uni.bt = bt;
    }

    return ret;
}

BaseType::BaseType()
    : tag(Tag::NONE)
    , qualifiers()
{
    qualifiers.fill(false);
}

StructOrUnionType::StructOrUnionType()
    : typeTag(TypeTag::NONE)
    , declarationTag(DeclarationTag::NONE)
    , qualifiers()
    , tag()
    , types()
{
    qualifiers.fill(false);
}

StructOrUnionType::StructOrUnionType(const StructOrUnionType &src)
    : typeTag(src.typeTag)
    , declarationTag(src.declarationTag)
    , qualifiers(src.qualifiers)
    , tag(src.tag)
    , types(src.types)
{
    switch(declarationTag)
    {
        case(DeclarationTag::DIRECT):
            for(auto &&t : types)
                t = new Type(*t);
            break;

        default:
            break;
    }
}

StructOrUnionType::~StructOrUnionType() noexcept
{
    switch(declarationTag)
    {
        case(DeclarationTag::DIRECT):
            for(auto &&t : types)
                delete t;
            break;
        
        default:
            break;
    }
}

EnumerationType::EnumerationType()
    : tag()
    , qualifiers()
{
    qualifiers.fill(false);
}

TypedefType::TypedefType()
    : type(nullptr)
    , qualifiers()
{
    qualifiers.fill(false);
}

TypedefType::TypedefType(const TypedefType &src)
    : type(new Type(*src.type))
    , qualifiers(src.qualifiers)
{
}

TypedefType::~TypedefType() noexcept
{
    delete type;
}

ArrayType::ArrayType() noexcept
    : tag(Tag::NONE)
    , type(nullptr)
    , size(0)
{
}

ArrayType::ArrayType(const ArrayType &src)
    : tag(src.tag)
    , type(new Type(*src.type))
    , size(src.size)
{
}

ArrayType::~ArrayType() noexcept
{
    delete type;
}

PointerType::PointerType()
    : type(nullptr)
    , qualifiers()
{
    qualifiers.fill(false);
}

PointerType::PointerType(const PointerType &src)
    : type(new Type(*src.type))
    , qualifiers(src.qualifiers)
{
}

PointerType::~PointerType() noexcept
{
    delete type;
}

FunctionType::FunctionType()
    : returnType(nullptr)
    , argumentsType()
    , isVariable(false)
{
}

FunctionType::FunctionType(const FunctionType &src)
    : returnType(new Type(*src.returnType))
    , argumentsType(src.argumentsType)
    , isVariable(src.isVariable)
{
    for(auto &&t : argumentsType)
        t = new Type(*t);
}

FunctionType::~FunctionType() noexcept
{
    delete returnType;
    for(auto &&t : argumentsType)
        delete t;
}

}