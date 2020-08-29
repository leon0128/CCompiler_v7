#ifndef TP7_TRANSLATOR_HPP
#define TP7_TRANSLATOR_HPP

#include <vector>

#include "expression_simbol.hpp"
#include "static_assert_simbol.hpp"
#include "declaration_simbol.hpp" 
#include "function_definition_simbol.hpp"

namespace TP7
{

class Translator
{
public:
    static bool execute(const std::vector<Token*>&
        , std::string &dst);

private:
    Translator(const std::vector<Token*>&);

    TranslationUnit *tokTranslationUnit();
    ExternalDeclaration *tokExternalDeclaration();
    FunctionDefinition *tokFunctionDefinition();
    Declaration *tokDeclaration();
    DeclarationSpecifiers *tokDeclarationSpecifiers();
    Declarator *tokDeclarator();
    DeclarationList *tokDeclarationList();
    CompoundStatement *tokCompoundStatement();
    InitDeclaratorList *tokInitDeclaratorList();
    StaticAssertDeclaration *tokStaticAssertDeclaration();
    StorageClassSpecifier *tokStorageClassSpecifier();
    TypeSpecifier *tokTypeSpecifier();
    TypeQualifier *tokTypeQualifier();
    FunctionSpecifier *tokFunctionSpecifier();
    AlignmentSpecifier *tokAlignmentSpecifier();
    Pointer *tokPointer();
    DirectDeclarator *tokDirectDeclarator();
    BlockItemList *tokBlockItemList();
    InitDeclarator *tokInitDeclarator();
    ConstantExpression *tokConstantExpression();
    StringLiteral *tokStringLiteral();
    AtomicTypeSpecifier *tokAtomicTypeSpecifier();
    StructOrUnionSpecifier *tokStructOrUnionSpecifier();
    EnumSpecifier *tokEnumSpecifier();
    TypedefName *tokTypedefName();
    TypeName *tokTypeName();
    TypeQualifierList *tokTypeQualifierList();
    ::Identifier* tokIdentifier();
    AssignmentExpression *tokAssignmentExpression();
    ParameterTypeList *tokParameterTypeList();
    IdentifierList *tokIdentifierList();
    BlockItem *tokBlockItem();
    Initializer *tokInitializer();
    ConditionalExpression *tokConditionalExpression();
    StructOrUnion *tokStructOrUnion();
    StructDeclarationList *tokStructDeclarationList();
    EnumeratorList *tokEnumeratorList();
    SpecifierQualifierList *tokSpecifierQualifierList();
    AbstractDeclarator *tokAbstractDeclarator();
    UnaryExpression *tokUnaryExpression();
    AssignmentOperator *tokAssignmentOperator();
    ParameterList *tokParameterList();
    Statement *tokStatement();
    InitializerList *tokInitializerList();
    LogicalORExpression *tokLogicalORExpression();
    Expression *tokExpression();
    StructDeclaration *tokStructDeclaration();
    Enumerator *tokEnumerator();
    DirectAbstractDeclarator *tokDirectAbstractDeclarator();
    PostfixExpression *tokPostfixExpression();
    CastExpression *tokCastExpression();
    UnaryOperator *tokUnaryOperator();
    ParameterDeclaration *tokParameterDeclaration();
    LabeledStatement *tokLabeledStatement();
    ExpressionStatement *tokExpressionStatement();
    SelectionStatement *tokSelectionStatement();
    IterationStatement *tokIterationStatement();
    JumpStatement *tokJumpStatement();
    Designation *tokDesignation();
    LogicalANDExpression *tokLogicalANDExpression();
    StructDeclaratorList *tokStructDeclaratorList();
    EnumerationConstant *tokEnumerationConstant();
    ArgumentExpressionList *tokArgumentExpressionList();
    PrimaryExpression *tokPrimaryExpression();
    DesignatorList *tokDesignatorList();
    InclusiveORExpression *tokInclusiveORExpression();
    StructDeclarator *tokStructDeclarator();
    Constant *tokConstant();
    GenericSelection *tokGenericSelection();
    Designator *tokDesignator();
    ExclusiveORExpression *tokExclusiveORExpression();
    GenericAssocList *tokGenericAssocList();
    ANDExpression *tokANDExpression();
    GenericAssociation *tokGenericAssociation();
    EqualityExpression *tokEqualityExpression();
    RelationalExpression *tokRelationalExpression();
    ShiftExpression *tokShiftExpression();
    AdditiveExpression *tokAdditiveExpression();
    MultiplicativeExpression *tokMultiplicativeExpression();

    bool isMatch(Punctuator::Tag, bool isConsumed = true);
    bool isMatch(Keyword::Tag, bool isConsumed = true);
    bool isMatch(Token::Tag, bool isConsumed = true);

    const std::vector<Token*> &mTvec;
    std::size_t mIdx;
    bool mIsValid;
};

}

#endif