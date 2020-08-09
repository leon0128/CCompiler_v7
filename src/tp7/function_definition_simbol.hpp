#ifndef TP7_FUNCTION_DEFINITION_SIMBOL_HPP
#define TP7_FUNCTION_DEFINITION_SIMBOL_HPP

#include <vector>
#include <array>
#include <string>

#include "../simbol.hpp"

namespace TP7
{

class TranslationUnit;
class ExternalDeclaration;
class FunctionDefinition;
class Declaration;
class DeclarationSpecifiers;
class Declarator;
class DeclarationList;
class Declaration;
class CompoundStatement;
class BlockItemList;
class BlockItem;
class Statement;
class LabeledStatement;
class ExpressionStatement;
class SelectionStatement;
class IterationStatement;
class JumpStatement;
class ConstantExpression;
class Expression;

class TranslationUnit : public BaseSimbol
{
public:
    TranslationUnit()
        : BaseSimbol()
        , edvec(){}

    std::vector<ExternalDeclaration*> edvec;

    std::string string() const override;
};

class ExternalDeclaration : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE
        , FUNCTION_DEFINITION
        , DECLARATION
    };
    union Uni
    {
        FunctionDefinition *functionDefinition;
        Declaration *declaration;
    };

    ExternalDeclaration()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni{nullptr}{}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class FunctionDefinition : public BaseSimbol
{
public:
    FunctionDefinition()
        : BaseSimbol()
        , declarationSpecifiers(nullptr)
        , declarator(nullptr)
        , declarationList(nullptr)
        , compoundStatement(nullptr){}

    DeclarationSpecifiers *declarationSpecifiers;
    Declarator *declarator;
    DeclarationList *declarationList;
    CompoundStatement *compoundStatement;

    std::string string() const override;
};

class DeclarationList : public BaseSimbol
{
public:
    DeclarationList()
        : BaseSimbol()
        , dvec(){}

    std::vector<Declaration*> dvec;

    std::string string() const override;
};

class CompoundStatement : public BaseSimbol
{
public:
    CompoundStatement()
        : BaseSimbol()
        , blockItemList(nullptr){}

    BlockItemList *blockItemList;

    std::string string() const override;
};

class BlockItemList : public BaseSimbol
{
public:
    BlockItemList()
        : BaseSimbol()
        , bivec(){}

    std::vector<BlockItem*> bivec;

    std::string string() const override;
};

class BlockItem : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE
        , DECLARATION
        , STATEMENT
    };
    union Uni
    {
        Declaration *declaration;
        Statement *statement;
    };

    BlockItem()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni{nullptr}{}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class Statement : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE
        , LABELED
        , COMPOUND
        , EXPRESSION
        , SELECTION
        , ITERATION
        , JUMP
    };
    union Uni
    {
        LabeledStatement *labeled;
        CompoundStatement *compound;
        ExpressionStatement *expression;
        SelectionStatement *selection;
        IterationStatement *iteration;
        JumpStatement *jump;
    };

    Statement()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni{nullptr}{}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class LabeledStatement : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE
        , IDENTIFIER
        , CASE
        , DEFAULT
    };
    union Uni
    {
        constexpr Uni() noexcept
            : sIdentifier{nullptr, nullptr}{}

        struct SIdentifier
        {
            ::Identifier *identifier;
            Statement *statement;
        } sIdentifier;
        struct SCase
        {
            ConstantExpression *constant;
            Statement *statement;
        } sCase;
        Statement *defaultStatement;
    };

    LabeledStatement()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni(){}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class ExpressionStatement : public BaseSimbol
{
public:
    ExpressionStatement()
        : BaseSimbol()
        , expression(nullptr){}

    Expression *expression;
    
    std::string string() const override;
};

class SelectionStatement : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE
        , IF
        , IF_ELSE
        , SWITCH
    };
    union Uni
    {
        constexpr Uni() noexcept
            : sIf{nullptr, nullptr}{}

        struct SIf
        {
            Expression *expression;
            Statement *statement;
        } sIf;
        struct SIfElse
        {
            Expression *expression;
            Statement *ifStatement;
            Statement *elseStatement;
        } sIfElse;
        struct SSwitch
        {
            Expression *expression;
            Statement *statement;
        } sSwitch;
    };

    SelectionStatement()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni(){}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class IterationStatement : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE
        , WHILE
        , DO_WHILE
        , FOR_EXPRESSION
        , FOR_DECLARATION
    };
    union Uni
    {
        constexpr Uni() noexcept
            : sWhile{nullptr, nullptr}{}

        struct SWhile
        {
            Expression *expression;
            Statement *statement;
        } sWhile;
        struct SDoWhile
        {
            Statement *statement;
            Expression *expression;
        } sDoWhile;
        struct SForExpression
        {
            std::array<Expression*, 3> expressionArr;
            Statement *statement;
        } sForExpression;
        struct SForDeclaration
        {
            Declaration *declaration;
            std::array<Expression*, 2> expressionArr;
            Statement *statement;
        } sForDeclaration;
    };

    IterationStatement()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni(){}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

class JumpStatement : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE
        , GOTO
        , CONTINUE
        , BREAK
        , RETURN
    };
    union Uni
    {
        ::Identifier *gotoIdentifier;
        Expression *returnExpression;
    };

    JumpStatement()
        : BaseSimbol()
        , tag(Tag::NONE)
        , uni{nullptr}{}

    Tag tag;
    Uni uni;

    std::string string() const override;
};

}

#endif