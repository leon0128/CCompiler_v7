#ifndef TP7_EXPRESSION_SIMBOL_HPP
#define TP7_EXPRESSION_SIMBOL_HPP

#include "../simbol.hpp"

namespace TP7
{

class TypeName;
class ArgumentExpressionList;
class InitializerList;

class Expression;
class ConstantExpression;
class AssignmentExpression;
class AssignmentOperator;
class ConditionalExpression;
class LogicalORExpression;
class LogicalANDExpression;
class InclusiveORExpression;
class ExclusiveORExpression;
class ANDExpression;
class EqualityExpression;
class RelationalExpression;
class ShiftExpression;
class AdditiveExpression;
class MultiplicativeExpression;
class CastExpression;
class UnaryExpression;
class UnaryOperator;
class PostfixExpression;
class PrimaryExpression;
class GenericSelection;
class GenericAssocList;
class GenericAssociation;

class Expression : public BaseSimbol
{
public:
    std::vector<AssignmentExpression*> vec;

    Expression():
        BaseSimbol(),
        vec(){}
    
    std::string string() const override;
};

class ConstantExpression : public BaseSimbol
{
public:
    ConditionalExpression *conditional;

    ConstantExpression():
        BaseSimbol(),
        conditional(nullptr){}

    std::string string() const override;
};

class AssignmentExpression : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        CONDITIONAL,
        ASSIGNMENT
    };
    union Uni
    {
        ConditionalExpression *conditional;
        struct SAssignment
        {
            UnaryExpression *unary;
            AssignmentOperator *assignmentOperator;
            AssignmentExpression *assignment;
        } sAssignment;

        constexpr Uni() noexcept:
            conditional(nullptr){}
    };

    AssignmentExpression():
        BaseSimbol(),
        tag(Tag::NONE),
        uni(){}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class AssignmentOperator : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        ASS,
        MUL,
        DIV,
        REM,
        PLUS,
        MINUS,
        LSFT,
        RSFT,
        AND,
        INOR,
        EXOR
    };

    AssignmentOperator()
        : BaseSimbol()
        , tag(Tag::NONE){}

    Tag tag;

    std::string string() const override;
};

class ConditionalExpression : public BaseSimbol
{
public:
    LogicalORExpression *logicalOR;
    Expression *expression;
    ConditionalExpression *conditional;

    ConditionalExpression():
        BaseSimbol(),
        logicalOR(nullptr),
        expression(nullptr),
        conditional(nullptr){}

    std::string string() const override;
};

class LogicalORExpression : public BaseSimbol
{
public:
    std::vector<LogicalANDExpression*> vec;

    LogicalORExpression():
        BaseSimbol(),
        vec(){}
    
    std::string string() const override;
};

class LogicalANDExpression : public BaseSimbol
{
public:
    std::vector<InclusiveORExpression*> vec;

    LogicalANDExpression():
        BaseSimbol(),
        vec(){}
    
    std::string string() const override;
};

class InclusiveORExpression : public BaseSimbol
{
public:
    std::vector<ExclusiveORExpression*> vec;

    InclusiveORExpression():
        BaseSimbol(),
        vec(){}
    
    std::string string() const override;
};

class ExclusiveORExpression : public BaseSimbol
{
public:
    std::vector<ANDExpression*> vec;

    ExclusiveORExpression():
        BaseSimbol(),
        vec(){}
    
    std::string string() const override;
};

class ANDExpression : public BaseSimbol
{
public:
    std::vector<EqualityExpression*> vec;

    ANDExpression():
        BaseSimbol(),
        vec(){}
    
    std::string string() const override;
};

class EqualityExpression : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE
        , EQUAL
        , NOT_EQUAL
    };
    struct SEqualityExpression
    {
        constexpr SEqualityExpression() noexcept
            : re(nullptr)
            , tag(Tag::NONE){}

        RelationalExpression *re;
        Tag tag;
    };

    EqualityExpression():
        BaseSimbol(),
        seevec(){}
    
    std::vector<SEqualityExpression> seevec;

    std::string string() const override;
};

class RelationalExpression : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE
        , LESS
        , GREATER
        , LESS_EQ
        , GREATER_EQ
    };
    struct SRelationalExpression
    {
        constexpr SRelationalExpression() noexcept
            : se(nullptr)
            , tag(Tag::NONE){}

        ShiftExpression *se;
        Tag tag;
    };

    RelationalExpression():
        BaseSimbol(),
        srevec(){}

    std::vector<SRelationalExpression> srevec;

    std::string string() const override;
};

class ShiftExpression : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE
        , L_SHIFT
        , R_SHIFT
    };
    struct SShiftExpression
    {
        constexpr SShiftExpression() noexcept
            : tag(Tag::NONE)
            , ae(nullptr){}

        Tag tag;
        AdditiveExpression *ae;
    };

    ShiftExpression():
        BaseSimbol(),
        ssevec(){}
    std::vector<SShiftExpression> ssevec;

    std::string string() const override;
};

class AdditiveExpression : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE
        , ADD
        , SUB
    };
    struct SAdditiveExpression
    {
        constexpr SAdditiveExpression() noexcept
            : tag(Tag::NONE)
            , me(nullptr){}

        Tag tag;
        MultiplicativeExpression *me;
    };

    AdditiveExpression():
        BaseSimbol(),
        saevec(){}

    std::vector<SAdditiveExpression> saevec;

    std::string string() const override;
};

class MultiplicativeExpression : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE
        , MUL
        , DIV
        , REM
    };
    struct SMultiplicativeExpression
    {
        constexpr SMultiplicativeExpression() noexcept
            : tag(Tag::NONE)
            , ce(nullptr){}

        Tag tag;
        CastExpression *ce;
    };

    MultiplicativeExpression():
        BaseSimbol(),
        smevec(){}
    
    std::vector<SMultiplicativeExpression> smevec;

    std::string string() const override;
};

class CastExpression : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        UNARY,
        CAST
    };
    union Uni
    {
        UnaryExpression *unary;
        struct SCast
        {
            TypeName *typeName;
            CastExpression *cast;
        } sCast;

        constexpr Uni() noexcept:
            unary(nullptr){}
    };

    CastExpression():
        BaseSimbol(),
        tag(Tag::NONE),
        uni(){}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class UnaryExpression : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        POSTFIX,
        INC,
        DEC,
        UNARY,
        SIZEOF_UNARY,
        SIZEOF_TYPE,
        ALIGNOF
    };
    union Uni
    {
        PostfixExpression *postfix;
        UnaryExpression *inc;
        UnaryExpression *dec;
        struct SUnary
        {
            UnaryOperator *unaryOperator;
            CastExpression *cast;
        } sUnary;
        UnaryExpression *sizeofUnary;
        TypeName *sizeofType;
        TypeName *alignofType;

        constexpr Uni() noexcept:
            postfix(nullptr){}
    };

    UnaryExpression():
        BaseSimbol(),
        tag(Tag::NONE),
        uni(){}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class UnaryOperator : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        AND,
        AST,
        PLUS,
        MINUS,
        TILDE,
        EXC
    };

    UnaryOperator()
        : BaseSimbol()
        , tag(Tag::NONE){}

    Tag tag;

    std::string string() const override;
};

class PostfixExpression : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        PRIMARY,
        EXPRESSION,
        ARGUMENT,
        DOT_IDENTIFIER,
        ARROW_IDENTIFIER,
        INC,
        DEC,
        TYPE_NAME
    };
    union Uni
    {
        PrimaryExpression *primary;
        Expression *expression;
        ArgumentExpressionList *argument;
        ::Identifier *dotIdentifier;
        ::Identifier *arrowIdentifier;
        struct STypeName
        {
            TypeName *typeName;
            InitializerList *initializer;
        } sTypeName;

        constexpr Uni() noexcept:
            primary(nullptr){}
    };
    struct SPostfixExpression
    {
        Tag tag;
        Uni uni;
    };

    PostfixExpression():
        BaseSimbol(),
        spevec(){}

    std::vector<SPostfixExpression> spevec;

    std::string string() const override;
};

class ArgumentExpressionList : public BaseSimbol
{
public:
    ArgumentExpressionList()
        : BaseSimbol()
        , aevec(){}

    std::vector<AssignmentExpression*> aevec;

    std::string string() const override;
};

class PrimaryExpression : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        IDENTIFIER,
        CONSTANT,
        STRING_LITERAL,
        EXPRESSION,
        GENERIC_SELECTION
    };
    union Uni
    {
        ::Identifier *identifier;
        ::Constant *constant;
        StringLiteral *stringLiteral;
        Expression *expression;
        GenericSelection *genericSelection;
    };

    PrimaryExpression():
        BaseSimbol(),
        tag(Tag::NONE),
        uni{nullptr}{}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class GenericSelection : public BaseSimbol
{
public:
    AssignmentExpression *assignment;
    GenericAssocList *generic;

    GenericSelection()
        : BaseSimbol()
        , assignment(nullptr)
        , generic(nullptr){}
    
    std::string string() const override;
};

class GenericAssocList : public BaseSimbol
{
public:
    GenericAssocList()
        : BaseSimbol()
        , gavec(){}

    std::vector<GenericAssociation*> gavec;

    std::string string() const override;
};

class GenericAssociation : public BaseSimbol
{
public:
    GenericAssociation()
        : BaseSimbol()
        , typeName(nullptr)
        , assignment(nullptr){}

    TypeName *typeName;
    AssignmentExpression *assignment;

    std::string string() const override;
};

}

#endif