#ifndef TP7_TYPE_HPP
#define TP7_TYPE_HPP

#include <array>
#include <vector>
#include <string>

#include "../resource_controller.hpp"

namespace TP7
{

class Type;
class BaseType;
class StructOrUnionType;
class EnumerationType;
class TypedefType;
class ArrayType;
class PointerType;
class FunctionType;

class Type : RESOURCE_CONTROLLER::Base
{
public:
    class QualifierTag
    {
    public:
        QualifierTag() = delete;

        enum
        {
            CONST
            , RESTRICT
            , VOLATILE
            , ATOMIC
            , SIZE
        };
    };
    enum class TypeTag
    {
        NONE
        , BASE
        , STRUCT_OR_UNION
        , ENUMERATION
        , TYPEDEF
        , ARRAY
        , POINTER
        , FUNCTION
    };
    
    Type(TypeTag inTag)
        : Base()
        , tag(inTag){}
    
    TypeTag tag;
};

class BaseType : public Type
{
public:
    enum class Tag
    {
        NONE
        , VOID
        , CHAR
        , S_CHAR
        , U_CHAR
        , S_SHORT
        , U_SHORT
        , S_INT
        , U_INT
        , S_LONG
        , U_LONG
        , S_LONG_LONG
        , U_LONG_LONG
        , FLOAT
        , DOUBLE
        , LONG_DOUBLE
        , BOOL
        , FLOAT_COMPLEX
        , DOUBLE_COMPLEX
        , LONG_DOUBLE_COMPLEX
    };

    BaseType()
        : Type(Type::TypeTag::BASE)
        , tag(Tag::NONE)
        , qualifiers()
    {
        qualifiers.fill(false);
    }

    Tag tag;
    std::array<bool, Type::QualifierTag::SIZE> qualifiers;
};

class StructOrUnionType : public Type
{
public:
    enum class TypeTag
    {
        NONE
        , STRUCT
        , UNION
    };

    enum class DeclarationTag
    {
        NONE
        , TAG
        , DIRECT
    };

    union Uni
    {
        Uni()
            : tag(){}

        std::string tag;
        std::vector<Type*> types;
    };

    StructOrUnionType()
        : Type(Type::TypeTag::STRUCT_OR_UNION)
        , typeTag(TypeTag::NONE)
        , declarationTag(DeclarationTag::NONE)
        , qualifiers()
    {
        qualifiers.fill(false);
    }

    TypeTag typeTag;
    DeclarationTag declarationTag;
    std::array<bool, Type::QualifierTag::SIZE> qualifiers;
};

class EnumerationType : public Type
{
public:
    EnumerationType()
        : Type(Type::TypeTag::ENUMERATION)
        , tag()
        , qualifiers()
    {
        qualifiers.fill(false);
    }
    std::string tag;
    std::array<bool, Type::QualifierTag::SIZE> qualifiers;
};

class TypedefType : public Type
{
public:
    TypedefType()
        : Type(Type::TypeTag::TYPEDEF)
        , type(nullptr)
        , qualifiers()
    {
        qualifiers.fill(false);
    }

    Type *type;
    std::array<bool, Type::QualifierTag::SIZE> qualifiers;
};

class ArrayType : public Type
{
public:
    enum class Tag
    {
        NONE
        , FIXED
        , VARIABLE
    };

    ArrayType()
        : Type(Type::TypeTag::ARRAY)
        , tag(Tag::NONE)
        , type(nullptr)
        , size(0){}

    Tag tag;
    Type *type;
    std::size_t size;
};

class PointerType : public Type
{
public:
    PointerType()
        : Type(Type::TypeTag::POINTER)
        , type(nullptr)
        , qualifiers()
    {
        qualifiers.fill(false);
    }

    Type *type;
    std::array<bool, Type::QualifierTag::SIZE> qualifiers;
};

class FunctionType : public Type
{
public:
    FunctionType()
        : Type(Type::TypeTag::FUNCTION)
        , returnType(nullptr)
        , argumentsType()
        , isVariable(false){}

    Type *returnType;
    std::vector<Type*> argumentsType;
    bool isVariable;
};

}

#endif