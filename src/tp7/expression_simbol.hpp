#ifndef TP7_EXPRESSION_SIMBOL_HPP
#define TP7_EXPRESSION_SIMBOL_HPP

#include "type_specifier.hpp"
#include "../simbol.hpp"

namespace TP7
{

class Expression;
class ConstantExpression;
class AssignmentExpression;
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
class PostfixExpression;
class PrimaryExpression;

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
            Punctuator *punctuator;
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
    std::vector<RelationalExpression*> relvec;
    std::vector<Punctuator*> tagvec;

    EqualityExpression():
        BaseSimbol(),
        relvec(),
        tagvec(){}
    
    std::string string() const override;
};

class RelationalExpression : public BaseSimbol
{
public:
    std::vector<ShiftExpression*> sftvec;
    std::vector<Punctuator*> tagvec;

    RelationalExpression():
        BaseSimbol(),
        sftvec(),
        tagvec(){}

    std::string string() const override;
};

class ShiftExpression : public BaseSimbol
{
public:
    std::vector<AdditiveExpression*> addvec;
    std::vector<Punctuator*> tagvec;

    ShiftExpression():
        BaseSimbol(),
        addvec(),
        tagvec(){}

    std::string string() const override;
};

class AdditiveExpression : public BaseSimbol
{
public:
    std::vector<MultiplicativeExpression*> mulvec;
    std::vector<Punctuator*> tagvec;

    AdditiveExpression():
        BaseSimbol(),
        mulvec(),
        tagvec(){}

    std::string string() const override;
};

class MultiplicativeExpression : public BaseSimbol
{
public:
    std::vector<CastExpression*> castvec;
    std::vector<Punctuator*> tagvec;

    MultiplicativeExpression():
        BaseSimbol(),
        castvec(),
        tagvec(){}
    
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
            TypeSpecifier::Tag tag;
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
            Punctuator *tag;
            CastExpression *cast;
        } sUnary;
        UnaryExpression *sizeofUnary;
        TypeSpecifier::Tag sizeofType;
        TypeSpecifier::Tag alignofType;

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

class PostfixExpression : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        PRIMARY
    };
    union Uni
    {
        PrimaryExpression *primary;

        constexpr Uni() noexcept:
            primary(nullptr){}
    };

    PostfixExpression():
        BaseSimbol(),
        tag(Tag::NONE),
        uni(){}

    Tag tag;
    Uni uni;

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
        Identifier *identifier;
        Constant *constant;
        StringLiteral *stringLiteral;
        Expression *expression;
        // generic-selection
    };

    PrimaryExpression():
        BaseSimbol(),
        tag(Tag::NONE),
        uni{nullptr}{}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

}

#endif