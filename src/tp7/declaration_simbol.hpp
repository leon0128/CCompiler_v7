#ifndef TP7_DECLARATION_SIMBOL_HPP
#define TP7_DECLARATION_SIMBOL_HPP

#include "../simbol.hpp"

namespace TP7
{

class ConstantExpression;
class AssignmentExpression;

class Declaration;
class DeclarationSpecifiers;
class InitDeclaratorList;
class StorageClassSpecifier;
class TypeSpecifier;
class TypeQualifier;
class FunctionSpecifier;
class AlignmentSpecifier;
class InitDeclarator;
class Declarator;
class Initializer;
class StaticAssertDeclaration;
class AtomicTypeSpecifier;
class StructOrUnionSpecifier;
class EnumSpecifier;
class TypedefName;
class TypeName;
class StructOrUnion;
class StructDeclarationList;
class StructDeclaration;
class SpecifierQualifierList;
class StructDeclaratorList;
class StructDeclarator;
class EnumeratorList;
class Enumerator;
class EnumerationConstant;
class InitializerList;
class Designation;
class DesignatorList;
class Designator;
class Pointer;
class DirectDeclarator;
class TypeQualifierList;
class ParameterTypeList;
class ParameterList;
class ParameterDeclaration;
class AbstractDeclarator;
class DirectAbstractDeclarator;

class Declaration : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        DECLARATION,
        STATIC_ASSERT
    };
    union Uni
    {
        struct SDeclaration
        {
            DeclarationSpecifiers *declarationSpecifiers;
            InitDeclaratorList *initDeclaratorList;
        } sDeclaration;
        
        StaticAssertDeclaration *staticAssert;

        constexpr Uni() noexcept
            : sDeclaration{nullptr, nullptr}{}
    };

    Declaration()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni(){}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class DeclarationSpecifiers : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        STORAGE_CLASS_SPECIFIER,
        TYPE_SPECIFIER,
        TYPE_QUALIFIER,
        FUNCTION_SPECIFIER,
        ALIGNMENT_SPECIFIER
    };
    union Uni
    {
        StorageClassSpecifier *storageClassSpecifier;
        TypeSpecifier *typeSpecifier;
        TypeQualifier *typeQualifier;
        FunctionSpecifier *functionSpecifier;
        AlignmentSpecifier *alignmentSpecifier;
    };

    DeclarationSpecifiers()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni{nullptr}
        , declarationSpecifiers(nullptr){}

    Tag tag;
    Uni uni;
    DeclarationSpecifiers *declarationSpecifiers;

    std::string string() const override;
};

class InitDeclaratorList : public BaseSimbol
{
public:
    std::vector<InitDeclarator*> idvec;

    InitDeclaratorList()
        : BaseSimbol()
        , idvec(){}
    
    std::string string() const override;
};

class InitDeclarator : public BaseSimbol
{
public:
    Declarator *declarator;
    Initializer *initializer;

    InitDeclarator()
        : BaseSimbol()
        , declarator(nullptr)
        , initializer(nullptr){}

    std::string string() const override;
};

class StorageClassSpecifier : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        TYPEDEF,
        EXTERN,
        STATIC,
        THREAD_LOCAL,
        AUTO,
        REGISTER
    };

    StorageClassSpecifier()
        : BaseSimbol()
        , tag(Tag::NONE){}

    Tag tag;

    std::string string() const override;
};

class TypeSpecifier : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        VOID,
        CHAR,
        SHORT,
        INT,
        LONG,
        FLOAT,
        DOUBLE,
        SIGNED,
        UNSIGNED,
        BOOL,
        COMPLEX,
        ATOMIC_TYPE_SPECIFIER,
        STRUCT_OR_UNION_SPECIFIER,
        ENUM_SPECIFIER,
        TYPEDEF_NAME
    };
    union Uni
    {
        AtomicTypeSpecifier *atomicTypeSpecifier;
        StructOrUnionSpecifier *structOrUnionSpecifier;
        EnumSpecifier *enumSpecifier;
        TypedefName *typedefName;
    };

    TypeSpecifier()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni{nullptr}{}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class TypeQualifier : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        CONST,
        RESTRICT,
        VOLATILE,
        ATOMIC
    };

    TypeQualifier()
        : BaseSimbol()
        , tag(Tag::NONE){}

    Tag tag;

    std::string string() const override;
};

class FunctionSpecifier : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        INLINE,
        NORETURN
    };

    FunctionSpecifier()
        : BaseSimbol()
        , tag(Tag::NONE){}

    Tag tag;

    std::string string() const override;
};

class AlignmentSpecifier : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        TYPE_NAME,
        CONSTANT_EXPRESSION
    };

    union Uni
    {
        TypeName *typeName;
        ConstantExpression *constantExpression;
    };

    AlignmentSpecifier()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni{nullptr}{}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class AtomicTypeSpecifier : public BaseSimbol
{
public:
    TypeName *typeName;

    AtomicTypeSpecifier()
        : BaseSimbol()
        , typeName(){}
    
    std::string string() const override;
};

class StructOrUnionSpecifier : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        IDENTIFIER_STRUCT_DECLARATION_LIST,
        IDENTIFIER
    };

    StructOrUnionSpecifier()
        : BaseSimbol()
        , tag(Tag::NONE)
        , structOrUnion(nullptr)
        , identifier(nullptr)
        , structDeclarationList(nullptr){}

    Tag tag;

    StructOrUnion *structOrUnion;
    ::Identifier *identifier;
    StructDeclarationList *structDeclarationList;
    
    std::string string() const override;
};

class StructOrUnion : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        STRUCT,
        UNION
    };

    StructOrUnion()
        : BaseSimbol()
        , tag(Tag::NONE){}

    Tag tag;

    std::string string() const override;
};

class StructDeclarationList : public BaseSimbol
{
public:
    StructDeclarationList()
        : BaseSimbol()
        , sdvec(){}

    std::vector<StructDeclaration*> sdvec;

    std::string string() const override;
};

class StructDeclaration : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        DECLARATOR,
        STATIC_ASSERT
    };
    union Uni
    {
        struct SDeclarator
        {
            SpecifierQualifierList *specifierQualifierList;
            StructDeclaratorList *structDeclaratorList;
        } sDeclarator;
        StaticAssertDeclaration *staticAssert;

        constexpr Uni() noexcept
            : sDeclarator{nullptr, nullptr}{}
    };

    StructDeclaration()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni(){}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class SpecifierQualifierList : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        SPECIFIER,
        QUALIFIER
    };
    union Uni
    {
        TypeSpecifier *specifier;
        TypeQualifier *qualifier;
    };

    SpecifierQualifierList()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni{nullptr}
        , specifierQualifierList(nullptr){}

    Tag tag;
    Uni uni;
    SpecifierQualifierList *specifierQualifierList;

    std::string string() const override;
};

class StructDeclaratorList : public BaseSimbol
{
public:
    StructDeclaratorList()
        : BaseSimbol()
        , sdvec(){}

    std::vector<StructDeclarator*> sdvec;

    std::string string() const override;
};

class StructDeclarator : public BaseSimbol
{
public:
    StructDeclarator()
        : BaseSimbol()
        , declarator(nullptr)
        , constantExpression(nullptr){}

    Declarator *declarator;
    ConstantExpression *constantExpression;

    std::string string() const override;
};

class TypedefName : public BaseSimbol
{
public:
    TypedefName()
        : BaseSimbol()
        , identifier(nullptr){}

    ::Identifier *identifier;

    std::string string() const override;
};

class EnumSpecifier : public BaseSimbol
{
public:
    EnumSpecifier()
        : BaseSimbol()
        , identifier(nullptr)
        , enumeratorList(nullptr){}

    ::Identifier *identifier;
    EnumeratorList *enumeratorList;

    std::string string() const override;
};

class EnumeratorList : public BaseSimbol
{
public:
    EnumeratorList()
        : BaseSimbol()
        , evec(){}

    std::vector<Enumerator*> evec;

    std::string string() const override;
};

class Enumerator : public BaseSimbol
{
public:
    Enumerator()
        : BaseSimbol()
        , enumerationConstant(nullptr)
        , constantExpression(nullptr){}

    EnumerationConstant *enumerationConstant;
    ConstantExpression *constantExpression;

    std::string string() const override;
};

class EnumerationConstant : public BaseSimbol
{
public:
    EnumerationConstant()
        : BaseSimbol()
        , identifier(nullptr){}

    ::Identifier *identifier;

    std::string string() const override;
};

class Initializer : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        ASSIGNMENT,
        INITIALIZER
    };
    union Uni
    {
        AssignmentExpression *assignment;
        InitializerList *initializer;
    };

    Initializer()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni{nullptr}{}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class InitializerList : public BaseSimbol
{
public:
    struct SInitializerList
    {
        Designation *designation;
        Initializer *initializer;
    };

    InitializerList()
        : BaseSimbol()
        , ilvec(){}

    std::vector<SInitializerList> ilvec;

    std::string string() const override;
};

class Designation : public BaseSimbol
{
public:
    Designation()
        : BaseSimbol()
        , designatorList(nullptr){}

    DesignatorList *designatorList;

    std::string string() const override;
};

class DesignatorList : public BaseSimbol
{
public:
    DesignatorList()
        : BaseSimbol()
        , dvec(){}

    std::vector<Designator*> dvec;

    std::string string() const override;
};

class Designator : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        CONSTANT,
        IDENTIFIER
    };
    union Uni
    {
        ConstantExpression *constant;
        ::Identifier *identifier;
    };

    Designator()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni{nullptr}{}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class Declarator : public BaseSimbol
{
public:
    Declarator()
        : BaseSimbol()
        , pointer(nullptr)
        , directDeclarator(nullptr){}

    Pointer *pointer;
    DirectDeclarator *directDeclarator;

    std::string string() const override;
};

class DirectDeclarator : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        IDENTIFIER,
        DECLARATOR,
        QUALIFIER_ASSIGNMENT,
        STATIC_QUALIFIER_ASSIGNMENT,
        QUALIFIER_STATIC_ASSIGNMENT,
        QUALIFIER,
        PARAMETER,
        IDENTIFIER_LIST
    };
    union Uni
    {
        ::Identifier *identifier;
        Declarator *declarator;
        struct SQualifierAssignment
        {
            TypeQualifierList *qualifier;
            AssignmentExpression *assignment;
        } sQualifierAssignment;
        struct SStaticQualifierAssignment
        {
            TypeQualifierList *qualifier;
            AssignmentExpression *assignment;
        } sStaticQualifierAssignment;
        struct SQualifierStaticAssignment
        {
            TypeQualifierList *qualifier;
            AssignmentExpression *assginment;
        } sQualifierStaticAssignment;
        TypeQualifierList *qualifier;
        ParameterTypeList *parameter;
        IdentifierList *identifierList;

        constexpr Uni() noexcept
            : identifier(nullptr){}
    };
    struct SDirectDeclarator
    {
        constexpr SDirectDeclarator() noexcept
            : tag(Tag::NONE)
            , uni(){}

        Tag tag;
        Uni uni;
    };

    DirectDeclarator()
        : BaseSimbol()
        , sddvec(){}

    std::vector<SDirectDeclarator> sddvec;
    
    std::string string() const override;
};

class Pointer : public BaseSimbol
{
public:
    Pointer()
        : BaseSimbol()
        , typeQualifierList(nullptr)
        , pointer(nullptr){}
    
    TypeQualifierList *typeQualifierList;
    Pointer *pointer;

    std::string string() const override;
};

class TypeQualifierList : public BaseSimbol
{
public:
    TypeQualifierList()
        : BaseSimbol()
        , tqvec(){}

    std::vector<TypeQualifier*> tqvec;

    std::string string() const override;
};

class ParameterTypeList : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        PARAMETER_LIST,
        VALRIABLE_PARALETER_LIST
    };

    ParameterTypeList()
        : BaseSimbol()
        , tag(Tag::NONE)
        , parameterList(nullptr){}

    Tag tag;
    ParameterList *parameterList;

    std::string string() const override;
};

class ParameterList : public BaseSimbol
{
public:
    ParameterList()
        : BaseSimbol()
        , pdvec(){}

    std::vector<ParameterDeclaration*> pdvec;

    std::string string() const override;
};

class ParameterDeclaration : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        DECLARATOR,
        ABSTRACT_DECLARATOR
    };
    union Uni
    {
        Declarator *declarator;
        AbstractDeclarator *abstractDeclarator;
    };

    ParameterDeclaration()
        : BaseSimbol()
        , tag(Tag::NONE)
        , declarationSpecifiers(nullptr)
        , uni{nullptr}{}

    Tag tag;
    DeclarationSpecifiers *declarationSpecifiers;
    Uni uni;

    std::string string() const override;
};

class TypeName : public BaseSimbol
{
public:
    TypeName()
        : BaseSimbol()
        , specifierQualifierList(nullptr)
        , abstractDeclarator(nullptr){}

    SpecifierQualifierList *specifierQualifierList;
    AbstractDeclarator *abstractDeclarator;

    std::string string() const override;
};

class AbstractDeclarator : public BaseSimbol
{
public:
    AbstractDeclarator()
        : BaseSimbol()
        , pointer(nullptr)
        , declarator(nullptr){}

    Pointer *pointer;
    DirectAbstractDeclarator *declarator;

    std::string string() const override;
};

class DirectAbstractDeclarator : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        ABSTRACT,
        QUALIFIER_ASSIGNMENT,
        STATIC_QUALIFIER_ASSIGNMENT,
        QUALIFIER_STATIC_ASSIGNMENT,
        ASTERISK,
        PARAMETER
    };
    union Uni
    {
        AbstractDeclarator *abstract;
        struct SQualifierAssignment
        {
            TypeQualifierList *qualifier;
            AssignmentExpression *assignment;
        } sQualifierAssignment;
        struct SStaticQualifierAssignment
        {
            TypeQualifierList *qualifier;
            AssignmentExpression *assignment;
        } sStaticQualifierAssignment;
        struct SQualifierStaticAssignment
        {
            TypeQualifierList *qualifier;
            AssignmentExpression *assignment;
        } sQualifierStaticAssignment;
        ParameterTypeList *parameter;

        constexpr Uni() noexcept
            : abstract(nullptr){}
    };
    struct SDirectAbstractDeclarator
    {
        constexpr SDirectAbstractDeclarator() noexcept
            : tag(Tag::NONE)
            , uni(){}

        Tag tag;
        Uni uni;
    };

    DirectAbstractDeclarator()
        : BaseSimbol()
        , sdadvec(){}

    std::vector<SDirectAbstractDeclarator> sdadvec;

    std::string string() const override;
};

}

#endif