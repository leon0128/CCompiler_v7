#ifndef TP7_TYPE_HPP
#define TP7_TYPE_HPP

#include <array>
#include <vector>
#include <string>

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

class Type
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
    enum class Tag
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
    union Uni
    {
        BaseType *bt;
        StructOrUnionType *sut;
        EnumerationType *et;
        TypedefType *tt;
        ArrayType *at;
        PointerType *pt;
        FunctionType *ft;
    };
    
    constexpr Type() noexcept
        : tag(Tag::NONE)
        , uni{nullptr}{}
    
    Tag tag;
    Uni uni;
};

class BaseType
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
        : tag(Tag::NONE)
        , qualifiers()
    {
        qualifiers.fill(false);
    }

    Tag tag;
    std::array<bool, Type::QualifierTag::SIZE> qualifiers;
};

class StructOrUnionType
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
        : typeTag(TypeTag::NONE)
        , declarationTag(DeclarationTag::NONE)
        , qualifiers()
    {
        qualifiers.fill(false);
    }

    TypeTag typeTag;
    DeclarationTag declarationTag;
    std::array<bool, Type::QualifierTag::SIZE> qualifiers;
};

class EnumerationType
{
public:
    EnumerationType()
        : tag()
        , qualifiers()
    {
        qualifiers.fill(false);
    }

    std::string tag;
    std::array<bool, Type::QualifierTag::SIZE> qualifiers;
};

class TypedefType
{
public:
    TypedefType()
        : type(nullptr)
        , qualifiers()
    {
        qualifiers.fill(false);
    }

    Type *type;
    std::array<bool, Type::QualifierTag::SIZE> qualifiers;
};

class ArrayType
{
public:
    enum class Tag
    {
        NONE
        , FIXED
        , VARIABLE
    };

    constexpr ArrayType() noexcept
        : tag(Tag::NONE)
        , type(nullptr)
        , size(0){}

    Tag tag;
    Type *type;
    std::size_t size;
};

class PointerType
{
public:
    PointerType()
        : type(nullptr)
        , qualifiers()
    {
        qualifiers.fill(false);
    }

    Type *type;
    std::array<bool, Type::QualifierTag::SIZE> qualifiers;
};

class FunctionType
{
public:
    FunctionType()
        : returnType(nullptr)
        , argumentsType()
        , isVariable(false){}

    Type *returnType;
    std::vector<Type*> argumentsType;
    bool isVariable;
};

}

#endif