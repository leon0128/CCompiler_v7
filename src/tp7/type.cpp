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