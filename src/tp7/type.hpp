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
    
    Type() noexcept;
    Type(const Type&);
    ~Type() noexcept;
    
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

    BaseType();
    BaseType(const BaseType&) = default;
    ~BaseType() noexcept = default;

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



    StructOrUnionType();
    StructOrUnionType(const StructOrUnionType&);
    ~StructOrUnionType() noexcept;

    TypeTag typeTag;
    DeclarationTag declarationTag;
    std::array<bool, Type::QualifierTag::SIZE> qualifiers;
    std::string tag;
    std::vector<Type*> types;
};

class EnumerationType
{
public:
    EnumerationType();
    EnumerationType(const EnumerationType&) = default;
    ~EnumerationType() noexcept = default;

    std::string tag;
    std::array<bool, Type::QualifierTag::SIZE> qualifiers;
};

class TypedefType
{
public:
    TypedefType();
    TypedefType(const TypedefType&);
    ~TypedefType() noexcept;

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

    ArrayType() noexcept;
    ArrayType(const ArrayType&);
    ~ArrayType() noexcept;

    Tag tag;
    Type *type;
    std::size_t size;
};

class PointerType
{
public:
    PointerType();
    PointerType(const PointerType&);
    ~PointerType() noexcept;

    Type *type;
    std::array<bool, Type::QualifierTag::SIZE> qualifiers;
};

class FunctionType
{
public:
    FunctionType();
    FunctionType(const FunctionType&);
    ~FunctionType() noexcept;

    Type *returnType;
    std::vector<Type*> argumentsType;
    bool isVariable;
};

}

#endif