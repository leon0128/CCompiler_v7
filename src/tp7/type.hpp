#ifndef TP7_TYPE_HPP
#define TP7_TYPE_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <set>

#include "declaration_simbol.hpp"

namespace TP7
{

class Type;
class BaseType;
class FunctionType;
class PointerType;
class BitfieldType;
class StructType;
class UnionType;
class ArrayType;
class EnumerationType;

class Declaration;

class Type
{
public:
    enum class Tag
    {
        NONE
        , BASE
        , FUNCTION
        , POINTER
        , BITFIELD
        , STRUCT
        , UNION
        , ARRAY
        , ENUMERATION
    };
    union Uni
    {
        BaseType *baseT;
        FunctionType *functionT;
        PointerType *pointerT;
        BitfieldType *bitfieldT;
        StructType *structT;
        UnionType *unionT;
        ArrayType *arrayT;
        EnumerationType *enumerationT;
    };

    constexpr Type() noexcept
        : tag(Tag::NONE)
        , uni{nullptr}{}
    constexpr BaseType *set(BaseType *t) noexcept
        {tag = Tag::BASE; return uni.baseT = t;}
    constexpr FunctionType *set(FunctionType *t) noexcept
        {tag = Tag::FUNCTION; return uni.functionT = t;}
    constexpr PointerType *set(PointerType *t) noexcept
        {tag = Tag::POINTER; return uni.pointerT = t;}
    constexpr BitfieldType *set(BitfieldType *t) noexcept
        {tag = Tag::BITFIELD; return uni.bitfieldT = t;}
    constexpr StructType *set(StructType *t) noexcept
        {tag = Tag::STRUCT; return uni.structT = t;}
    constexpr UnionType *set(UnionType *t) noexcept
        {tag = Tag::UNION; return uni.unionT = t;}
    constexpr ArrayType *set(ArrayType *t) noexcept
        {tag = Tag::ARRAY; return uni.arrayT = t;}
    constexpr EnumerationType *set(EnumerationType *t) noexcept
        {tag = Tag::ENUMERATION; return uni.enumerationT = t;}
    
    Type(const Type&);

    std::size_t size() const;
    std::size_t alignment() const;

    bool isSigned() const;
    bool isUnsigned() const;
    bool isInteger() const;
    bool isFloating() const;

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
    };

    constexpr BaseType() noexcept
        : tag(Tag::NONE){}

    BaseType(const BaseType&);

    std::size_t size() const;
    std::size_t alignment() const;

    Tag tag;
};

class FunctionType
{
public:
    FunctionType() noexcept(noexcept(std::vector<Type*>()))
        : returnT(nullptr)
        , argTypes()
        , isVariable(false){}

    FunctionType(const FunctionType&);

    std::size_t size() const;
    std::size_t alignment() const;

    Type *returnT;
    std::vector<Type*> argTypes;
    bool isVariable;
};

class PointerType
{
public:
    constexpr PointerType() noexcept
        : referencedT(nullptr){}

    PointerType(const PointerType&);

    std::size_t size() const;
    std::size_t alignment() const;

    Type *referencedT;
};

class BitfieldType
{
public:
    constexpr BitfieldType() noexcept
        : referencedT(nullptr)
        , fieldSize(0){}

    BitfieldType(const BitfieldType&);

    std::size_t size() const;
    std::size_t alignment() const;

    Type *referencedT;
    std::size_t fieldSize;
};

class StructType
{
public:
    StructType() noexcept(noexcept(std::vector<Type*>())
        && noexcept(std::string()))
        : tag()
        , types(){}

    StructType(const StructType&);

    std::size_t size() const;
    std::size_t alignment() const;

    std::string tag;
    std::vector<Type*> types;
};

class UnionType
{
public:
    UnionType() noexcept(noexcept(std::vector<Type*>())
        && noexcept(std::string()))
        : tag()
        , types(){}

    UnionType(const UnionType&);

    std::size_t size() const;
    std::size_t alignment() const;

    std::string tag;
    std::vector<Type*> types;
};

class ArrayType
{
public:
    constexpr ArrayType() noexcept
        : elementT(nullptr)
        , arraySize(0){}

    std::size_t size() const;
    std::size_t alignment() const;

    Type *elementT;
    std::size_t arraySize;
};

class EnumerationType
{
public:
    EnumerationType() noexcept(noexcept(std::string()))
        : tag(){}

    std::size_t size() const;
    std::size_t alignment() const;

    EnumerationType(const EnumerationType&);

    std::string tag;
};

namespace TYPE
{
    extern const std::unordered_map<BaseType::Tag, std::vector<std::multiset<::TP7::TypeSpecifier::Tag>>> SPECIFIER_MAP;
    extern const std::unordered_map<BaseType::Tag, std::ptrdiff_t> CONVERSION_RANK;

    extern Type *deduce(const DeclarationSpecifiers*);
    extern Type *deduce(const TypeName*);
    extern Type *deduce(const SpecifierQualifierList*);
    extern Type *deduce(const Declarator*, Type*);
    extern Type *deduce(const ParameterDeclaration*);
    extern Type *deduce(const AbstractDeclarator*, Type*);
    extern Type *deduce(const std::vector<const TypeSpecifier*>&);

    extern Type *promote(const Type*, const Type*);
    extern std::ptrdiff_t getConversionRank(const Type*);
}


}

#endif