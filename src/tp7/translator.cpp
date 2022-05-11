#include <iostream>
#include <utility>

#include "identifier.hpp"
#include "generator.hpp"
#include "scope.hpp"
#include "translator.hpp"

namespace TP7
{

bool Translator::execute(const std::vector<Token*> &tvec
    , std::string &dst)
{
    Translator translator(tvec);

    TranslationUnit *tu = translator.tokTranslationUnit();
    if(tu != nullptr)
    {
        Generator generator;
        dst = generator.generate(tu);
    }

    if(translator.mIdx != tvec.size())
    {
        std::cerr << "TP7 Translation error:\n"
            "    what: source code could not be evalueated to the end.\n"
            << std::flush;
    }

    return translator.mIsValid;
}

Translator::Translator(const std::vector<Token*> &tvec)
    : mTvec(tvec)
    , mIdx(0)
    , mIsValid(true)
{
}

TranslationUnit *Translator::tokTranslationUnit()
{
    std::vector<ExternalDeclaration*> edvec;

    for(ExternalDeclaration *ed = tokExternalDeclaration();
        ed != nullptr;
        ed = tokExternalDeclaration())
        edvec.push_back(ed);
    
    if(!edvec.empty())
    {
        TranslationUnit *ret = new TranslationUnit();
        ret->edvec = std::move(edvec);
        return ret;
    }
    else
        return nullptr;
}

ExternalDeclaration *Translator::tokExternalDeclaration()
{
    ExternalDeclaration ret;
    std::size_t preidx = mIdx;

    // tokenize
    if((ret.uni.functionDefinition = tokFunctionDefinition()) != nullptr)
        ret.tag = ExternalDeclaration::Tag::FUNCTION_DEFINITION;
    else if((ret.uni.declaration = tokDeclaration()) != nullptr)
        ret.tag = ExternalDeclaration::Tag::DECLARATION;

    if(ret.tag != ExternalDeclaration::Tag::NONE)
        ;
    else
        return nullptr;

    // analyze
    DeclarationSpecifiers *ds = nullptr;
    switch(ret.tag)
    {
        case(ExternalDeclaration::Tag::FUNCTION_DEFINITION):
            ds = ret.uni.functionDefinition->declarationSpecifiers;
            break;
        case(ExternalDeclaration::Tag::DECLARATION):
            switch(ret.uni.declaration->tag)
            {
                case(Declaration::Tag::DECLARATION):
                    ds = ret.uni.declaration->uni.sDeclaration.declarationSpecifiers;
                    break;
                
                default:;
            }
        
        default:;
    }

    bool isValid = true;
    while(ds != nullptr)
    {
        if(ds->tag == DeclarationSpecifiers::Tag::STORAGE_CLASS_SPECIFIER)
        {
            if(ds->uni.storageClassSpecifier->tag == StorageClassSpecifier::Tag::AUTO
                || ds->uni.storageClassSpecifier->tag == StorageClassSpecifier::Tag::REGISTER)
            {
                std::cout << "TP7 Translator error:\n"
                    "    what: The storage-class specifiers auto and register shall not appear in the declaration specifiers in an external declaration.\n"
                    "    contents: " << ret.string()
                    << std::endl;
                isValid = false;
                break;
            }
        }

        ds = ds->declarationSpecifiers;
    }

    if(isValid)
        return new ExternalDeclaration(ret);
    else
        return mIdx = preidx, nullptr;
}

FunctionDefinition *Translator::tokFunctionDefinition()
{
    FunctionDefinition ret;
    std::size_t preidx = mIdx;

    if((ret.declarationSpecifiers = tokDeclarationSpecifiers()) != nullptr
        && (ret.declarator = tokDeclarator()) != nullptr
        && ((ret.declarationList = tokDeclarationList()), true)
        && (ret.compoundStatement = tokCompoundStatement()) != nullptr)
        return new FunctionDefinition(ret);
    else
        return mIdx = preidx, nullptr;
}

Declaration *Translator::tokDeclaration()
{
    Declaration ret;
    std::size_t preidx = mIdx;

    if((ret.uni.sDeclaration.declarationSpecifiers = tokDeclarationSpecifiers()) != nullptr
        && ((ret.uni.sDeclaration.initDeclaratorList = tokInitDeclaratorList()), true)
        && isMatch(Punctuator::Tag::SEMI_COL))
        ret.tag = Declaration::Tag::DECLARATION;
    else if(((mIdx = preidx), true)
        && (ret.uni.staticAssert = tokStaticAssertDeclaration()) != nullptr)
        ret.tag = Declaration::Tag::STATIC_ASSERT;
    
    if(ret.tag != Declaration::Tag::NONE)
        return new Declaration(ret);
    else
        return mIdx = preidx, nullptr;
}

DeclarationSpecifiers *Translator::tokDeclarationSpecifiers()
{
    DeclarationSpecifiers ret;

    if((ret.uni.storageClassSpecifier = tokStorageClassSpecifier()) != nullptr)
        ret.tag = DeclarationSpecifiers::Tag::STORAGE_CLASS_SPECIFIER;
    else if((ret.uni.typeSpecifier = tokTypeSpecifier()) != nullptr)
        ret.tag = DeclarationSpecifiers::Tag::TYPE_SPECIFIER;
    else if((ret.uni.typeQualifier = tokTypeQualifier()) != nullptr)
        ret.tag = DeclarationSpecifiers::Tag::TYPE_QUALIFIER;
    else if((ret.uni.functionSpecifier = tokFunctionSpecifier()) != nullptr)
        ret.tag = DeclarationSpecifiers::Tag::FUNCTION_SPECIFIER;
    else if((ret.uni.alignmentSpecifier = tokAlignmentSpecifier()) != nullptr)
        ret.tag = DeclarationSpecifiers::Tag::ALIGNMENT_SPECIFIER;
    
    if(ret.tag != DeclarationSpecifiers::Tag::NONE)
    {
        ret.declarationSpecifiers = tokDeclarationSpecifiers();
        return new DeclarationSpecifiers(ret);
    }
    else
        return nullptr;
}

Declarator *Translator::tokDeclarator()
{
    Declarator ret;
    std::size_t preidx = mIdx;

    if(((ret.pointer = tokPointer()), true)
        && (ret.directDeclarator = tokDirectDeclarator()) != nullptr)
        return new Declarator(ret);
    else
        return mIdx = preidx, nullptr;
}

DeclarationList *Translator::tokDeclarationList()
{
    std::vector<Declaration*> dvec;

    for(Declaration *d = tokDeclaration();
        d != nullptr;
        d = tokDeclaration())
        dvec.push_back(d);
    
    if(!dvec.empty())
    {
        DeclarationList *ret = new DeclarationList();
        ret->dvec = std::move(dvec);
        return ret;
    }
    else
        return nullptr;
}

CompoundStatement *Translator::tokCompoundStatement()
{
    CompoundStatement ret;
    std::size_t preidx = mIdx;

    if(isMatch(Punctuator::Tag::BL_PAREN)
        && ((ret.blockItemList = tokBlockItemList()), true)
        && isMatch(Punctuator::Tag::BR_PAREN))
        return new CompoundStatement(ret);
    else
        return mIdx = preidx, nullptr;
}

InitDeclaratorList *Translator::tokInitDeclaratorList()
{
    std::vector<InitDeclarator*> idvec;
    InitDeclarator *id = tokInitDeclarator();

    if(id != nullptr)
    {
        idvec.push_back(id);

        while(isMatch(Punctuator::Tag::COMMA))
        {
            if((id = tokInitDeclarator()) != nullptr)
                idvec.push_back(id);
            else
            {
                mIdx--;
                break;
            }
        }
    }

    if(!idvec.empty())
    {
        InitDeclaratorList *ret = new InitDeclaratorList();
        ret->idvec = std::move(idvec);
        return ret;
    }
    else
        return nullptr;
}

StaticAssertDeclaration *Translator::tokStaticAssertDeclaration()
{
    StaticAssertDeclaration ret;
    std::size_t preidx = mIdx;

    if(isMatch(Keyword::Tag::STATIC_ASSERT)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (ret.constant = tokConstantExpression()) != nullptr
        && isMatch(Punctuator::Tag::COMMA)
        && (ret.str = tokStringLiteral()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN)
        && isMatch(Punctuator::Tag::SEMI_COL))
        return new StaticAssertDeclaration(ret);
    else
        return mIdx = preidx, nullptr;
}

StorageClassSpecifier *Translator::tokStorageClassSpecifier()
{
    StorageClassSpecifier ret;

    if(isMatch(Keyword::Tag::TYPEDEF))
        ret.tag = StorageClassSpecifier::Tag::TYPEDEF;
    else if(isMatch(Keyword::Tag::EXTERN))
        ret.tag = StorageClassSpecifier::Tag::EXTERN;
    else if(isMatch(Keyword::Tag::STATIC))
        ret.tag = StorageClassSpecifier::Tag::STATIC;
    else if(isMatch(Keyword::Tag::THREAD_LOCAL))
        ret.tag = StorageClassSpecifier::Tag::THREAD_LOCAL;
    else if(isMatch(Keyword::Tag::AUTO))
        ret.tag = StorageClassSpecifier::Tag::AUTO;
    else if(isMatch(Keyword::Tag::REGISTER))
        ret.tag = StorageClassSpecifier::Tag::REGISTER;

    if(ret.tag != StorageClassSpecifier::Tag::NONE)
        return new StorageClassSpecifier(ret);
    else
        return nullptr;
}

TypeSpecifier *Translator::tokTypeSpecifier()
{
    TypeSpecifier ret;

    if(isMatch(Keyword::Tag::VOID))
        ret.tag = TypeSpecifier::Tag::VOID;
    else if(isMatch(Keyword::Tag::CHAR))
        ret.tag = TypeSpecifier::Tag::CHAR;
    else if(isMatch(Keyword::Tag::SHORT))
        ret.tag = TypeSpecifier::Tag::SHORT;
    else if(isMatch(Keyword::Tag::INT))
        ret.tag = TypeSpecifier::Tag::INT;
    else if(isMatch(Keyword::Tag::LONG))
        ret.tag = TypeSpecifier::Tag::LONG;
    else if(isMatch(Keyword::Tag::FLOAT))
        ret.tag = TypeSpecifier::Tag::FLOAT;
    else if(isMatch(Keyword::Tag::DOUBLE))
        ret.tag = TypeSpecifier::Tag::DOUBLE;
    else if(isMatch(Keyword::Tag::SIGNED))
        ret.tag = TypeSpecifier::Tag::SIGNED;
    else if(isMatch(Keyword::Tag::UNSIGNED))
        ret.tag = TypeSpecifier::Tag::UNSIGNED;
    else if(isMatch(Keyword::Tag::BOOL))
        ret.tag = TypeSpecifier::Tag::BOOL;
    else if(isMatch(Keyword::Tag::COMPLEX))
        ret.tag = TypeSpecifier::Tag::COMPLEX;
    else if((ret.uni.atomicTypeSpecifier = tokAtomicTypeSpecifier()) != nullptr)
        ret.tag = TypeSpecifier::Tag::ATOMIC_TYPE_SPECIFIER;
    else if((ret.uni.structOrUnionSpecifier = tokStructOrUnionSpecifier()) != nullptr)
        ret.tag = TypeSpecifier::Tag::STRUCT_OR_UNION_SPECIFIER;
    else if((ret.uni.enumSpecifier = tokEnumSpecifier()) != nullptr)
        ret.tag = TypeSpecifier::Tag::ENUM_SPECIFIER;
    else if((ret.uni.typedefName = tokTypedefName()) != nullptr)
        ret.tag = TypeSpecifier::Tag::TYPEDEF_NAME;

    if(ret.tag != TypeSpecifier::Tag::NONE)
        return new TypeSpecifier(ret);
    else
        return nullptr;
}

TypeQualifier *Translator::tokTypeQualifier()
{
    TypeQualifier ret;

    if(isMatch(Keyword::Tag::CONST))
        ret.tag = TypeQualifier::Tag::CONST;
    else if(isMatch(Keyword::Tag::RESTRICT))
        ret.tag = TypeQualifier::Tag::RESTRICT;
    else if(isMatch(Keyword::Tag::VOLATILE))
        ret.tag = TypeQualifier::Tag::VOLATILE;
    else if(isMatch(Keyword::Tag::ATOMIC))
        ret.tag = TypeQualifier::Tag::ATOMIC;
    
    if(ret.tag != TypeQualifier::Tag::NONE)
        return new TypeQualifier(ret);
    else
        return nullptr;
}

FunctionSpecifier *Translator::tokFunctionSpecifier()
{
    FunctionSpecifier ret;

    if(isMatch(Keyword::Tag::INLINE))
        ret.tag = FunctionSpecifier::Tag::INLINE;
    else if(isMatch(Keyword::Tag::NORETURN))
        ret.tag = FunctionSpecifier::Tag::NORETURN;

    if(ret.tag != FunctionSpecifier::Tag::NONE)
        return new FunctionSpecifier(ret);
    else
        return nullptr;
}

AlignmentSpecifier *Translator::tokAlignmentSpecifier()
{
    AlignmentSpecifier ret;
    std::size_t preidx = mIdx;

    if(isMatch(Keyword::Tag::ALIGNAS)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (ret.uni.typeName = tokTypeName()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN))
        ret.tag = AlignmentSpecifier::Tag::TYPE_NAME;
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::ALIGNAS)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (ret.uni.constantExpression = tokConstantExpression()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN))
        ret.tag = AlignmentSpecifier::Tag::CONSTANT_EXPRESSION;
    
    if(ret.tag != AlignmentSpecifier::Tag::NONE)
        return new AlignmentSpecifier(ret);
    else
        return mIdx = preidx, nullptr;
}

Pointer *Translator::tokPointer()
{
    Pointer ret;
    std::size_t preidx = mIdx;

    if(isMatch(Punctuator::Tag::AST)
        && (ret.typeQualifierList = tokTypeQualifierList(), true)
        && (ret.pointer = tokPointer()) != nullptr)
        return new Pointer(ret);
    else if((mIdx = preidx, true)
        && isMatch(Punctuator::Tag::AST)
        && (ret.typeQualifierList = tokTypeQualifierList(), true)
        && (ret.pointer = nullptr, true))
        return new Pointer(ret);
    else
        return mIdx = preidx, nullptr;
}

DirectDeclarator *Translator::tokDirectDeclarator()
{
    std::vector<DirectDeclarator::SDirectDeclarator> sddvec;
    DirectDeclarator::SDirectDeclarator sdd;
    std::size_t preidx = mIdx;

    if((sdd.uni.identifier = tokIdentifier()) != nullptr)
        sdd.tag = DirectDeclarator::Tag::IDENTIFIER;
    else if(isMatch(Punctuator::Tag::L_PAREN)
        && (sdd.uni.declarator = tokDeclarator()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN))
        sdd.tag = DirectDeclarator::Tag::DECLARATOR;

    while(sdd.tag != DirectDeclarator::Tag::NONE)
    {
        sddvec.push_back(sdd);
        preidx = mIdx;
        
        if(isMatch(Punctuator::Tag::SL_PAREN)
            && (sdd.uni.sQualifierAssignment.qualifier = tokTypeQualifierList(), true)
            && (sdd.uni.sQualifierAssignment.assignment = tokAssignmentExpression(), true)
            && isMatch(Punctuator::Tag::SR_PAREN))
            sdd.tag = DirectDeclarator::Tag::QUALIFIER_ASSIGNMENT;
        else if((mIdx = preidx, true)
            && isMatch(Punctuator::Tag::SL_PAREN)
            && isMatch(Keyword::Tag::STATIC)
            && (sdd.uni.sStaticQualifierAssignment.qualifier = tokTypeQualifierList(), true)
            && (sdd.uni.sStaticQualifierAssignment.assignment = tokAssignmentExpression()) != nullptr
            && isMatch(Punctuator::Tag::SR_PAREN))
            sdd.tag = DirectDeclarator::Tag::STATIC_QUALIFIER_ASSIGNMENT;
        else if((mIdx = preidx, true)
            && isMatch(Punctuator::Tag::SL_PAREN)
            && (sdd.uni.sQualifierStaticAssignment.qualifier = tokTypeQualifierList()) != nullptr
            && isMatch(Keyword::Tag::STATIC)
            && (sdd.uni.sQualifierStaticAssignment.assginment = tokAssignmentExpression()) != nullptr
            && isMatch(Punctuator::Tag::SR_PAREN))
            sdd.tag = DirectDeclarator::Tag::QUALIFIER_STATIC_ASSIGNMENT;
        else if((mIdx = preidx, true)
            && isMatch(Punctuator::Tag::SL_PAREN)
            && (sdd.uni.qualifier = tokTypeQualifierList(), true)
            && isMatch(Punctuator::Tag::AST)
            && isMatch(Punctuator::Tag::SR_PAREN))
            sdd.tag = DirectDeclarator::Tag::QUALIFIER;
        else if((mIdx = preidx, true)
            && isMatch(Punctuator::Tag::L_PAREN)
            && (sdd.uni.parameter = tokParameterTypeList()) != nullptr
            && isMatch(Punctuator::Tag::R_PAREN))
            sdd.tag = DirectDeclarator::Tag::PARAMETER;
        else if((mIdx = preidx, true)
            && isMatch(Punctuator::Tag::L_PAREN)
            && (sdd.uni.identifierList = tokIdentifierList(), true)
            && isMatch(Punctuator::Tag::R_PAREN))
            sdd.tag = DirectDeclarator::Tag::IDENTIFIER_LIST;
        else
            sdd.tag = DirectDeclarator::Tag::NONE;
    }

    mIdx = preidx;
    
    if(!sddvec.empty())
    {
        DirectDeclarator *ret = new DirectDeclarator();
        ret->sddvec = std::move(sddvec);
        return ret;
    }
    else
        return nullptr;
}

BlockItemList *Translator::tokBlockItemList()
{
    std::vector<BlockItem*> bivec;

    for(BlockItem *bi = tokBlockItem();
        bi != nullptr;
        bi = tokBlockItem())
        bivec.push_back(bi);
    
    if(!bivec.empty())
    {
        BlockItemList *ret = new BlockItemList();
        ret->bivec = std::move(bivec);
        return ret;
    }
    else
        return nullptr;
}

InitDeclarator *Translator::tokInitDeclarator()
{
    InitDeclarator ret;
    std::size_t preidx = mIdx;

    if((ret.declarator = tokDeclarator()) != nullptr
        && isMatch(Punctuator::Tag::ASS)
        && (ret.initializer = tokInitializer()) != nullptr)
        return new InitDeclarator(ret);
    else if((mIdx = preidx, true)
        && (ret.declarator = tokDeclarator()) != nullptr
        && (ret.initializer = nullptr, true))
        return new InitDeclarator(ret);
    else
        return mIdx = preidx, nullptr;
}

ConstantExpression *Translator::tokConstantExpression()
{
    ConstantExpression ret;

    if((ret.conditional = tokConditionalExpression()) != nullptr)
        return new ConstantExpression(ret);
    else
        return nullptr;
}

StringLiteral *Translator::tokStringLiteral()
{
    if(isMatch(Token::Tag::STRING_LITERAL))
        return mTvec[mIdx - 1]->uni.stringLiteral;
    else
        return nullptr;
}

AtomicTypeSpecifier *Translator::tokAtomicTypeSpecifier()
{
    AtomicTypeSpecifier ret;
    std::size_t preidx = mIdx;

    if(isMatch(Keyword::Tag::ATOMIC)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (ret.typeName = tokTypeName()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN))
        return new AtomicTypeSpecifier(ret);
    else
        return mIdx = preidx, nullptr;
}

StructOrUnionSpecifier *Translator::tokStructOrUnionSpecifier()
{
    StructOrUnionSpecifier ret;
    std::size_t preidx = mIdx;

    if((ret.structOrUnion = tokStructOrUnion()) != nullptr
        && (ret.identifier = tokIdentifier(), true)
        && isMatch(Punctuator::Tag::BL_PAREN)
        && (ret.structDeclarationList = tokStructDeclarationList()) != nullptr
        && isMatch(Punctuator::Tag::BR_PAREN))
        return new StructOrUnionSpecifier(ret);
    else if((mIdx = preidx, true)
        && (ret.structOrUnion = tokStructOrUnion()) != nullptr
        && (ret.identifier = tokIdentifier()) != nullptr
        && (ret.structDeclarationList = nullptr, true))
        return new StructOrUnionSpecifier(ret);
    else
        return mIdx = preidx, nullptr;
}

EnumSpecifier *Translator::tokEnumSpecifier()
{
    EnumSpecifier ret;
    std::size_t preidx = mIdx;

    if(isMatch(Keyword::Tag::ENUM)
        && (ret.identifier = tokIdentifier(), true)
        && isMatch(Punctuator::Tag::BL_PAREN)
        && (ret.enumeratorList = tokEnumeratorList()) != nullptr
        && isMatch(Punctuator::Tag::BR_PAREN))
        return new EnumSpecifier(ret);
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::ENUM)
        && (ret.identifier = tokIdentifier(), true)
        && isMatch(Punctuator::Tag::BL_PAREN)
        && (ret.enumeratorList = tokEnumeratorList()) != nullptr
        && isMatch(Punctuator::Tag::COMMA)
        && isMatch(Punctuator::Tag::BR_PAREN))
        return new EnumSpecifier(ret);
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::ENUM)
        && (ret.identifier = tokIdentifier()) != nullptr
        && (ret.enumeratorList = nullptr, true))
        return new EnumSpecifier(ret);
    else
        return mIdx = preidx, nullptr;
}

TypedefName *Translator::tokTypedefName()
{
    TypedefName ret;
    std::size_t preidx = mIdx;

    // tokenize
    if((ret.identifier = tokIdentifier()) != nullptr)
        ;
    else
        return nullptr;

    // analyze
    IDENTIFIER::Identifier *ident = Scope::find(Scope::NameSpaceTag::OTHER, ret.identifier->str);
    if(ident != nullptr)
        return new TypedefName(ret);
    else
        return mIdx = preidx, nullptr;
}

TypeName *Translator::tokTypeName()
{
    TypeName ret;
    std::size_t preidx = mIdx;

    if((ret.specifierQualifierList = tokSpecifierQualifierList()) != nullptr
        && (ret.abstractDeclarator = tokAbstractDeclarator(), true))
        return new TypeName();
    else
        return mIdx = preidx, nullptr;
}

TypeQualifierList *Translator::tokTypeQualifierList()
{
    std::vector<TypeQualifier*> tqvec;

    for(TypeQualifier *tq = tokTypeQualifier();
        tq != nullptr;
        tq = tokTypeQualifier())
        tqvec.push_back(tq);
    
    if(!tqvec.empty())
    {
        TypeQualifierList *ret = new TypeQualifierList();
        ret->tqvec = std::move(tqvec);
        return ret;
    }
    else
        return nullptr;
}

::Identifier *Translator::tokIdentifier()
{
    if(isMatch(Token::Tag::IDENTIFIER))
        return mTvec[mIdx - 1]->uni.identifier;
    else
        return nullptr;
}

AssignmentExpression *Translator::tokAssignmentExpression()
{
    AssignmentExpression ret;
    std::size_t preidx = mIdx;

    if((ret.uni.sAssignment.unary = tokUnaryExpression()) != nullptr
        && (ret.uni.sAssignment.assignmentOperator = tokAssignmentOperator()) != nullptr
        && (ret.uni.sAssignment.assignment = tokAssignmentExpression()) != nullptr)
        ret.tag = AssignmentExpression::Tag::ASSIGNMENT;
    else if((mIdx = preidx, true)
        && (ret.uni.conditional = tokConditionalExpression()) != nullptr)
        ret.tag = AssignmentExpression::Tag::CONDITIONAL;
    
    if(ret.tag != AssignmentExpression::Tag::NONE)
        return new AssignmentExpression(ret);
    else
        return mIdx = preidx, nullptr;
}

ParameterTypeList *Translator::tokParameterTypeList()
{
    ParameterTypeList ret;
    std::size_t preidx = mIdx;

    if((ret.parameterList = tokParameterList()) != nullptr
        && isMatch(Punctuator::Tag::COMMA)
        && isMatch(Punctuator::Tag::DDD))
        ret.tag = ParameterTypeList::Tag::VALRIABLE_PARALETER_LIST;
    else if((mIdx = preidx, true)
        && (ret.parameterList = tokParameterList()) != nullptr)
        ret.tag = ParameterTypeList::Tag::PARAMETER_LIST;
    
    if(ret.tag != ParameterTypeList::Tag::NONE)
        return new ParameterTypeList(ret);
    else
        return mIdx = preidx, nullptr;
}

IdentifierList *Translator::tokIdentifierList()
{
    std::vector<::Identifier*> ivec;
    std::size_t preidx = mIdx;
    ::Identifier *i = tokIdentifier();

    while(i != nullptr)
    {
        ivec.push_back(i);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::COMMA))
            i = tokIdentifier();
        else
            i = nullptr;
    }

    mIdx = preidx;

    if(!ivec.empty())
    {
        IdentifierList *ret = new IdentifierList();
        ret->ivec = std::move(ivec);
        return ret;
    }
    else
        return nullptr;
}

BlockItem *Translator::tokBlockItem()
{
    BlockItem ret;
    
    if((ret.uni.declaration = tokDeclaration()) != nullptr)
        ret.tag = BlockItem::Tag::DECLARATION;
    else if((ret.uni.statement = tokStatement()) != nullptr)
        ret.tag = BlockItem::Tag::STATEMENT;
    
    if(ret.tag != BlockItem::Tag::NONE)
        return new BlockItem(ret);
    else
        return nullptr;
}

Initializer *Translator::tokInitializer()
{
    Initializer ret;
    std::size_t preidx = mIdx;

    if((ret.uni.assignment = tokAssignmentExpression()) != nullptr)
        ret.tag = Initializer::Tag::ASSIGNMENT;
    else if((mIdx = preidx)
        && isMatch(Punctuator::Tag::BL_PAREN)
        && (ret.uni.initializer = tokInitializerList()) != nullptr
        && isMatch(Punctuator::Tag::BR_PAREN))
        ret.tag = Initializer::Tag::INITIALIZER;
    else if((mIdx = preidx, true)
        && isMatch(Punctuator::Tag::BL_PAREN)
        && (ret.uni.initializer = tokInitializerList()) != nullptr
        && isMatch(Punctuator::Tag::COMMA)
        && isMatch(Punctuator::Tag::BR_PAREN))
        ret.tag = Initializer::Tag::INITIALIZER;
    
    if(ret.tag != Initializer::Tag::NONE)
        return new Initializer(ret);
    else
        return mIdx = preidx, nullptr;
}

ConditionalExpression *Translator::tokConditionalExpression()
{
    ConditionalExpression ret;
    std::size_t preidx = mIdx;

    if((ret.logicalOR = tokLogicalORExpression()) != nullptr
        && isMatch(Punctuator::Tag::QUE)
        && (ret.expression = tokExpression()) != nullptr
        && isMatch(Punctuator::Tag::COLON)
        && (ret.conditional = tokConditionalExpression()) != nullptr)
        return new ConditionalExpression(ret);
    else if((mIdx = preidx, true)
        && (ret.logicalOR = tokLogicalORExpression()) != nullptr
        && (ret.expression = nullptr, true)
        && (ret.conditional = nullptr, true))
        return new ConditionalExpression(ret);
    else
        return mIdx = preidx, nullptr;
}

StructOrUnion *Translator::tokStructOrUnion()
{
    StructOrUnion ret;

    if(isMatch(Keyword::Tag::STRUCT))
        ret.tag = StructOrUnion::Tag::STRUCT;
    else if(isMatch(Keyword::Tag::UNION))
        ret.tag = StructOrUnion::Tag::UNION;
    
    if(ret.tag != StructOrUnion::Tag::NONE)
        return new StructOrUnion(ret);
    else
        return nullptr;
}

StructDeclarationList *Translator::tokStructDeclarationList()
{
    std::vector<StructDeclaration*> sdvec;

    for(StructDeclaration *sd = tokStructDeclaration();
        sd != nullptr;
        sd = tokStructDeclaration())
        sdvec.push_back(sd);
    
    if(!sdvec.empty())
    {
        StructDeclarationList *ret = new StructDeclarationList();
        ret->sdvec = std::move(sdvec);
        return ret;
    }
    else
        return nullptr;
}

EnumeratorList *Translator::tokEnumeratorList()
{
    std::vector<Enumerator*> evec;
    std::size_t preidx = mIdx;
    Enumerator *e = tokEnumerator();

    while(e != nullptr)
    {
        evec.push_back(e);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::COMMA))
            e = tokEnumerator();
        else
            e = nullptr;
    }

    mIdx = preidx;

    if(!evec.empty())
    {
        EnumeratorList *ret = new EnumeratorList();
        ret->evec = std::move(evec);
        return ret;
    }
    else
        return nullptr;
}

SpecifierQualifierList * Translator::tokSpecifierQualifierList()
{
    SpecifierQualifierList ret;

    if((ret.uni.specifier = tokTypeSpecifier()) != nullptr)
        ret.tag = SpecifierQualifierList::Tag::SPECIFIER;
    else if((ret.uni.qualifier = tokTypeQualifier()) != nullptr)
        ret.tag = SpecifierQualifierList::Tag::QUALIFIER;
    
    if(ret.tag != SpecifierQualifierList::Tag::NONE)
    {
        ret.specifierQualifierList = tokSpecifierQualifierList();
        return new SpecifierQualifierList(ret);
    }
    else
        return nullptr;
}

AbstractDeclarator *Translator::tokAbstractDeclarator()
{
    AbstractDeclarator ret;
    std::size_t preidx = mIdx;

    if((ret.pointer = tokPointer(), true)
        && (ret.declarator = tokDirectAbstractDeclarator()) != nullptr)
        return new AbstractDeclarator(ret);
    else if((mIdx = preidx, true)
        && (ret.pointer = tokPointer()) != nullptr
        && (ret.declarator = nullptr, true))
        return new AbstractDeclarator(ret);
    else
        return mIdx = preidx, nullptr;
}

UnaryExpression *Translator::tokUnaryExpression()
{
    UnaryExpression ret;
    std::size_t preidx = mIdx;

    if((ret.uni.postfix = tokPostfixExpression()) != nullptr)
        ret.tag = UnaryExpression::Tag::POSTFIX;
    else if((mIdx = preidx, true)
        && isMatch(Punctuator::Tag::INC)
        && (ret.uni.inc = tokUnaryExpression()) != nullptr)
        ret.tag = UnaryExpression::Tag::INC;
    else if((mIdx = preidx, true)
        && isMatch(Punctuator::Tag::DEC)
        && (ret.uni.dec = tokUnaryExpression()) != nullptr)
        ret.tag = UnaryExpression::Tag::DEC;
    else if((mIdx = preidx, true)
        && (ret.uni.sUnary.unaryOperator = tokUnaryOperator()) != nullptr
        && (ret.uni.sUnary.cast = tokCastExpression()) != nullptr)
        ret.tag = UnaryExpression::Tag::UNARY;
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::SIZEOF)
        && (ret.uni.sizeofUnary = tokUnaryExpression()) != nullptr)
        ret.tag = UnaryExpression::Tag::SIZEOF_UNARY;
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::SIZEOF)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (ret.uni.sizeofType = tokTypeName()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN))
        ret.tag = UnaryExpression::Tag::SIZEOF_TYPE;
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::ALIGNOF)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (ret.uni.alignofType = tokTypeName()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN))
        ret.tag = UnaryExpression::Tag::ALIGNOF;
    
    if(ret.tag != UnaryExpression::Tag::NONE)
        return new UnaryExpression(ret);
    else
        return mIdx = preidx, nullptr;
}

AssignmentOperator *Translator::tokAssignmentOperator()
{
    AssignmentOperator ret;

    if(isMatch(Punctuator::Tag::ASS))
        ret.tag = AssignmentOperator::Tag::ASS;
    else if(isMatch(Punctuator::Tag::MUL_ASS))
        ret.tag = AssignmentOperator::Tag::MUL;
    else if(isMatch(Punctuator::Tag::DIV_ASS))
        ret.tag = AssignmentOperator::Tag::DIV;
    else if(isMatch(Punctuator::Tag::REM_ASS))
        ret.tag = AssignmentOperator::Tag::REM;
    else if(isMatch(Punctuator::Tag::ADD_ASS))
        ret.tag = AssignmentOperator::Tag::PLUS;
    else if(isMatch(Punctuator::Tag::SUB_ASS))
        ret.tag = AssignmentOperator::Tag::MINUS;
    else if(isMatch(Punctuator::Tag::LSFT_ASS))
        ret.tag = AssignmentOperator::Tag::LSFT;
    else if(isMatch(Punctuator::Tag::RSFT_ASS))
        ret.tag = AssignmentOperator::Tag::RSFT;
    else if(isMatch(Punctuator::Tag::BW_AND_ASS))
        ret.tag = AssignmentOperator::Tag::AND;
    else if(isMatch(Punctuator::Tag::BW_EXOR_ASS))
        ret.tag = AssignmentOperator::Tag::EXOR;
    else if(isMatch(Punctuator::Tag::BW_INOR_ASS))
        ret.tag = AssignmentOperator::Tag::INOR;
    
    if(ret.tag != AssignmentOperator::Tag::NONE)
        return new AssignmentOperator(ret);
    else
        return nullptr;
}

ParameterList *Translator::tokParameterList()
{
    std::vector<ParameterDeclaration*> pdvec;
    std::size_t preidx = mIdx;
    ParameterDeclaration *pd = tokParameterDeclaration();

    while(pd != nullptr)
    {
        pdvec.push_back(pd);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::COMMA))
            pd = tokParameterDeclaration();
        else
            pd = nullptr;
    }

    mIdx = preidx;

    if(!pdvec.empty())
    {
        ParameterList *ret = new ParameterList();
        ret->pdvec = std::move(pdvec);
        return ret;
    }
    else
        return nullptr;
}

Statement *Translator::tokStatement()
{
    Statement ret;

    if((ret.uni.labeled = tokLabeledStatement()) != nullptr)
        ret.tag = Statement::Tag::LABELED;
    else if((ret.uni.compound = tokCompoundStatement()) != nullptr)
        ret.tag = Statement::Tag::COMPOUND;
    else if((ret.uni.expression = tokExpressionStatement()) != nullptr)
        ret.tag = Statement::Tag::EXPRESSION;
    else if((ret.uni.selection = tokSelectionStatement()) != nullptr)
        ret.tag = Statement::Tag::SELECTION;
    else if((ret.uni.iteration = tokIterationStatement()) != nullptr)
        ret.tag = Statement::Tag::ITERATION;
    else if((ret.uni.jump = tokJumpStatement()) != nullptr)
        ret.tag = Statement::Tag::JUMP;
    
    if(ret.tag != Statement::Tag::NONE)
        return new Statement(ret);
    else
        return nullptr;
}

InitializerList *Translator::tokInitializerList()
{
    std::vector<InitializerList::SInitializerList> silvec;
    std::size_t preidx = mIdx;
    InitializerList::SInitializerList sil{nullptr, nullptr};
    sil.designation = tokDesignation();
    sil.initializer = tokInitializer();

    while(sil.initializer != nullptr)
    {
        silvec.push_back(sil);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::COMMA))
        {
            sil.designation = tokDesignation();
            sil.initializer = tokInitializer();
        }
        else
            sil = {nullptr, nullptr};
    }

    mIdx = preidx;

    if(!silvec.empty())
    {
        InitializerList *ret = new InitializerList();
        ret->ilvec = std::move(silvec);
        return ret;
    }
    else
        return nullptr;
}

LogicalORExpression *Translator::tokLogicalORExpression()
{
    std::vector<LogicalANDExpression*> laevec;
    std::size_t preidx = mIdx;
    LogicalANDExpression *lae = tokLogicalANDExpression();

    while(lae != nullptr)
    {
        laevec.push_back(lae);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::OR))
            lae = tokLogicalANDExpression();
        else
            lae = nullptr;
    }

    mIdx = preidx;
    
    if(!laevec.empty())
    {
        LogicalORExpression *ret = new LogicalORExpression();
        ret->vec = std::move(laevec);
        return ret;
    }
    else
        return nullptr;
}

Expression *Translator::tokExpression()
{
    std::vector<AssignmentExpression*> aevec;
    std::size_t preidx = mIdx;
    AssignmentExpression *ae = tokAssignmentExpression();

    while(ae != nullptr)
    {
        aevec.push_back(ae);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::COMMA))
            ae = tokAssignmentExpression();
        else
            ae = nullptr;
    }

    mIdx = preidx;

    if(!aevec.empty())
    {
        Expression *ret = new Expression();
        ret->vec = std::move(aevec);
        return ret;
    }
    else
        return nullptr;
}

StructDeclaration *Translator::tokStructDeclaration()
{
    StructDeclaration ret;
    std::size_t preidx = mIdx;

    if((ret.uni.sDeclarator.specifierQualifierList = tokSpecifierQualifierList()) != nullptr
        && (ret.uni.sDeclarator.structDeclaratorList = tokStructDeclaratorList(), true)
        && isMatch(Punctuator::Tag::SEMI_COL))
        ret.tag = StructDeclaration::Tag::DECLARATOR;
    else if((mIdx = preidx, true)
        && (ret.uni.staticAssert = tokStaticAssertDeclaration()) != nullptr)
        ret.tag = StructDeclaration::Tag::STATIC_ASSERT;
    
    if(ret.tag != StructDeclaration::Tag::NONE)
        return new StructDeclaration(ret);
    else
        return mIdx = preidx, nullptr;
}

Enumerator *Translator::tokEnumerator()
{
    Enumerator ret;
    std::size_t preidx = mIdx;

    if((ret.enumerationConstant = tokEnumerationConstant()) != nullptr
        && isMatch(Punctuator::Tag::ASS)
        && (ret.constantExpression = tokConstantExpression()) != nullptr)
        return new Enumerator(ret);
    else if((mIdx = preidx, true)
        && (ret.enumerationConstant = tokEnumerationConstant()) != nullptr)
        return new Enumerator(ret);
    else
        return mIdx = preidx, nullptr;
}

DirectAbstractDeclarator *Translator::tokDirectAbstractDeclarator()
{
    std::vector<DirectAbstractDeclarator::SDirectAbstractDeclarator> sdadvec;
    std::size_t preidx = mIdx;
    DirectAbstractDeclarator::SDirectAbstractDeclarator sdad;

    if(isMatch(Punctuator::Tag::L_PAREN)
        && (sdad.uni.abstract = tokAbstractDeclarator()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN))
    {
        sdad.tag = DirectAbstractDeclarator::Tag::ABSTRACT;
        sdadvec.push_back(sdad);
    }
    else
        mIdx = preidx;
    
    do
    {
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::SL_PAREN)
            && (sdad.uni.sQualifierAssignment.qualifier = tokTypeQualifierList(), true)
            && (sdad.uni.sQualifierAssignment.assignment = tokAssignmentExpression(), true)
            && isMatch(Punctuator::Tag::SR_PAREN))
            sdad.tag = DirectAbstractDeclarator::Tag::QUALIFIER_ASSIGNMENT;
        else if((mIdx = preidx, true)
            && isMatch(Punctuator::Tag::SL_PAREN)
            && isMatch(Keyword::Tag::STATIC)
            && (sdad.uni.sStaticQualifierAssignment.qualifier = tokTypeQualifierList(), true)
            && (sdad.uni.sStaticQualifierAssignment.assignment = tokAssignmentExpression()) != nullptr
            && isMatch(Punctuator::Tag::SR_PAREN))
            sdad.tag = DirectAbstractDeclarator::Tag::STATIC_QUALIFIER_ASSIGNMENT;
        else if((mIdx = preidx, true)
            && isMatch(Punctuator::Tag::SL_PAREN)
            && (sdad.uni.sQualifierStaticAssignment.qualifier = tokTypeQualifierList()) != nullptr
            && isMatch(Keyword::Tag::STATIC)
            && (sdad.uni.sQualifierStaticAssignment.assignment = tokAssignmentExpression()) != nullptr
            && isMatch(Punctuator::Tag::SR_PAREN))
            sdad.tag = DirectAbstractDeclarator::Tag::QUALIFIER_STATIC_ASSIGNMENT;
        else if((mIdx = preidx, true)
            && isMatch(Punctuator::Tag::SL_PAREN)
            && isMatch(Punctuator::Tag::AST)
            && isMatch(Punctuator::Tag::SR_PAREN))
            sdad.tag = DirectAbstractDeclarator::Tag::ASTERISK;
        else if((mIdx = preidx, true)
            && isMatch(Punctuator::Tag::L_PAREN)
            && (sdad.uni.parameter = tokParameterTypeList(), true)
            && isMatch(Punctuator::Tag::R_PAREN))
            sdad.tag = DirectAbstractDeclarator::Tag::PARAMETER;
        else
            sdad.tag = DirectAbstractDeclarator::Tag::NONE;

        if(sdad.tag != DirectAbstractDeclarator::Tag::NONE)
            sdadvec.push_back(sdad);
    }
    while(sdad.tag != DirectAbstractDeclarator::Tag::NONE);

    mIdx = preidx;

    if(!sdadvec.empty())
    {
        DirectAbstractDeclarator *ret = new DirectAbstractDeclarator();
        ret->sdadvec = std::move(sdadvec);
        return ret;
    }
    else
        return nullptr;
}

PostfixExpression *Translator::tokPostfixExpression()
{
    std::vector<PostfixExpression::SPostfixExpression> spevec;
    std::size_t preidx = mIdx;
    PostfixExpression::SPostfixExpression spe = {PostfixExpression::Tag::NONE, PostfixExpression::Uni()};

    if((spe.uni.primary = tokPrimaryExpression()) != nullptr)
        spe.tag = PostfixExpression::Tag::PRIMARY;
    else if(isMatch(Punctuator::Tag::L_PAREN)
        && (spe.uni.sTypeName.typeName = tokTypeName()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN)
        && isMatch(Punctuator::Tag::BL_PAREN)
        && (spe.uni.sTypeName.initializer = tokInitializerList()) != nullptr
        && isMatch(Punctuator::Tag::BR_PAREN))
        spe.tag = PostfixExpression::Tag::TYPE_NAME;
    else if((mIdx = preidx, true)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (spe.uni.sTypeName.typeName = tokTypeName()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN)
        && isMatch(Punctuator::Tag::BL_PAREN)
        && (spe.uni.sTypeName.initializer = tokInitializerList()) != nullptr
        && isMatch(Punctuator::Tag::COMMA)
        && isMatch(Punctuator::Tag::BR_PAREN))
        spe.tag = PostfixExpression::Tag::TYPE_NAME;
    
    while(spe.tag != PostfixExpression::Tag::NONE)
    {
        spevec.push_back(spe);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::SL_PAREN)
            && (spe.uni.expression = tokExpression()) != nullptr
            && isMatch(Punctuator::Tag::SR_PAREN))
            spe.tag = PostfixExpression::Tag::EXPRESSION;
        else if((mIdx = preidx, true)
            && isMatch(Punctuator::Tag::L_PAREN)
            && (spe.uni.argument = tokArgumentExpressionList(), true)
            && (isMatch(Punctuator::Tag::R_PAREN)))
            spe.tag = PostfixExpression::Tag::ARGUMENT;
        else if((mIdx = preidx, true)
            && isMatch(Punctuator::Tag::DOT)
            && (spe.uni.dotIdentifier = tokIdentifier()) != nullptr)
            spe.tag = PostfixExpression::Tag::DOT_IDENTIFIER;
        else if((mIdx = preidx, true)
            && isMatch(Punctuator::Tag::ARROW)
            && (spe.uni.arrowIdentifier = tokIdentifier()) != nullptr)
            spe.tag = PostfixExpression::Tag::ARROW_IDENTIFIER;
        else if((mIdx = preidx, true)
            && isMatch(Punctuator::Tag::INC))
            spe.tag = PostfixExpression::Tag::INC;
        else if((mIdx = preidx, true)
            && isMatch(Punctuator::Tag::DEC))
            spe.tag = PostfixExpression::Tag::DEC;
        else
            spe.tag = PostfixExpression::Tag::NONE;
    }

    mIdx = preidx;

    if(!spevec.empty())
    {
        PostfixExpression *ret = new PostfixExpression();
        ret->spevec = std::move(spevec);
        return ret;
    }
    else
        return nullptr;
}

CastExpression *Translator::tokCastExpression()
{
    CastExpression ret;
    std::size_t preidx = mIdx;

    if(isMatch(Punctuator::Tag::L_PAREN)
        && (ret.uni.sCast.typeName = tokTypeName()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN)
        && (ret.uni.sCast.cast = tokCastExpression()) != nullptr)
        ret.tag = CastExpression::Tag::CAST;
    else if((mIdx = preidx, true)
        && (ret.uni.unary = tokUnaryExpression()) != nullptr)
        ret.tag = CastExpression::Tag::UNARY;
    
    if(ret.tag != CastExpression::Tag::NONE)
        return new CastExpression(ret);
    else
        return mIdx = preidx, nullptr;
}

UnaryOperator *Translator::tokUnaryOperator()
{
    UnaryOperator ret;

    if(isMatch(Punctuator::Tag::AMP))
        ret.tag = UnaryOperator::Tag::AND;
    else if(isMatch(Punctuator::Tag::AST))
        ret.tag = UnaryOperator::Tag::AST;
    else if(isMatch(Punctuator::Tag::PLUS))
        ret.tag = UnaryOperator::Tag::PLUS;
    else if(isMatch(Punctuator::Tag::MINUS))
        ret.tag = UnaryOperator::Tag::MINUS;
    else if(isMatch(Punctuator::Tag::TILDE))
        ret.tag = UnaryOperator::Tag::TILDE;
    else if(isMatch(Punctuator::Tag::EXC))
        ret.tag = UnaryOperator::Tag::EXC;
    
    if(ret.tag != UnaryOperator::Tag::NONE)
        return new UnaryOperator(ret);
    else
        return nullptr;
}

ParameterDeclaration *Translator::tokParameterDeclaration()
{
    ParameterDeclaration ret;
    std::size_t preidx = mIdx;
    
    if((ret.declarationSpecifiers = tokDeclarationSpecifiers()) != nullptr
        && (ret.uni.declarator = tokDeclarator()) != nullptr)
        ret.tag = ParameterDeclaration::Tag::DECLARATOR;
    else if((mIdx = preidx, true)
        && (ret.declarationSpecifiers = tokDeclarationSpecifiers()) != nullptr
        && (ret.uni.abstractDeclarator = tokAbstractDeclarator(), true))
        ret.tag = ParameterDeclaration::Tag::ABSTRACT_DECLARATOR;
    
    if(ret.tag != ParameterDeclaration::Tag::NONE)
        return new ParameterDeclaration(ret);
    else
        return mIdx = preidx, nullptr;
}

LabeledStatement *Translator::tokLabeledStatement()
{
    LabeledStatement ret;
    std::size_t preidx = mIdx;

    if((ret.uni.sIdentifier.identifier = tokIdentifier()) != nullptr
        && isMatch(Punctuator::Tag::COLON)
        && (ret.uni.sIdentifier.statement = tokStatement()) != nullptr)
        ret.tag = LabeledStatement::Tag::IDENTIFIER;
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::CASE)
        && (ret.uni.sCase.constant = tokConstantExpression()) != nullptr
        && isMatch(Punctuator::Tag::COLON)
        && (ret.uni.sCase.statement = tokStatement()) != nullptr)
        ret.tag = LabeledStatement::Tag::CASE;
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::DEFAULT)
        && isMatch(Punctuator::Tag::COLON)
        && (ret.uni.defaultStatement = tokStatement()) != nullptr)
        ret.tag = LabeledStatement::Tag::DEFAULT;
    
    if(ret.tag != LabeledStatement::Tag::NONE)
        return new LabeledStatement(ret);
    else
        return mIdx = preidx, nullptr;
}

ExpressionStatement *Translator::tokExpressionStatement()
{
    ExpressionStatement ret;
    std::size_t preidx = mIdx;

    if((ret.expression = tokExpression(), true)
        && isMatch(Punctuator::Tag::SEMI_COL))
        return new ExpressionStatement(ret);
    else
        return mIdx = preidx, nullptr;
}

SelectionStatement *Translator::tokSelectionStatement()
{
    SelectionStatement ret;
    std::size_t preidx = mIdx;


    if(isMatch(Keyword::Tag::IF)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (ret.uni.sIfElse.expression = tokExpression()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN)
        && (ret.uni.sIfElse.ifStatement = tokStatement()) != nullptr
        && isMatch(Keyword::Tag::ELSE)
        && (ret.uni.sIfElse.elseStatement = tokStatement()) != nullptr)
        ret.tag = SelectionStatement::Tag::IF_ELSE;
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::IF)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (ret.uni.sIf.expression = tokExpression()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN)
        && (ret.uni.sIf.statement = tokStatement()) != nullptr)
        ret.tag = SelectionStatement::Tag::IF;        
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::SWITCH)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (ret.uni.sSwitch.expression = tokExpression()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN)
        && (ret.uni.sSwitch.statement = tokStatement()) != nullptr)
        ret.tag = SelectionStatement::Tag::SWITCH;
    
    if(ret.tag != SelectionStatement::Tag::NONE)
        return new SelectionStatement(ret);
    else
        return mIdx = preidx, nullptr;
}

IterationStatement *Translator::tokIterationStatement()
{
    IterationStatement ret;
    std::size_t preidx = mIdx;

    if(isMatch(Keyword::Tag::WHILE)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (ret.uni.sWhile.expression = tokExpression()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN)
        && (ret.uni.sWhile.statement = tokStatement()) != nullptr)
        ret.tag = IterationStatement::Tag::WHILE;
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::DO)
        && (ret.uni.sDoWhile.statement = tokStatement()) != nullptr
        && isMatch(Keyword::Tag::WHILE)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (ret.uni.sDoWhile.expression = tokExpression()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN)
        && isMatch(Punctuator::Tag::SEMI_COL))
        ret.tag = IterationStatement::Tag::DO_WHILE;
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::FOR)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (ret.uni.sForExpression.expressionArr[0] = tokExpression(), true)
        && isMatch(Punctuator::Tag::SEMI_COL)
        && (ret.uni.sForExpression.expressionArr[1] = tokExpression(), true)
        && isMatch(Punctuator::Tag::SEMI_COL)
        && (ret.uni.sForExpression.expressionArr[2] = tokExpression(), true)
        && isMatch(Punctuator::Tag::R_PAREN)
        && (ret.uni.sForExpression.statement = tokStatement()) != nullptr)
        ret.tag = IterationStatement::Tag::FOR_EXPRESSION;
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::FOR)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (ret.uni.sForDeclaration.declaration = tokDeclaration()) != nullptr
        && (ret.uni.sForDeclaration.expressionArr[0] = tokExpression(), true)
        && isMatch(Punctuator::Tag::SEMI_COL)
        && (ret.uni.sForDeclaration.expressionArr[1] = tokExpression(), true)
        && isMatch(Punctuator::Tag::R_PAREN)
        && (ret.uni.sForDeclaration.statement = tokStatement()) != nullptr)
        ret.tag = IterationStatement::Tag::FOR_DECLARATION;
    
    if(ret.tag != IterationStatement::Tag::NONE)
        return new IterationStatement(ret);
    else
        return mIdx = preidx, nullptr;
}

JumpStatement *Translator::tokJumpStatement()
{
    JumpStatement ret;
    std::size_t preidx = mIdx;

    if(isMatch(Keyword::Tag::GOTO)
        && (ret.uni.gotoIdentifier = tokIdentifier()) != nullptr
        && isMatch(Punctuator::Tag::SEMI_COL))
        ret.tag = JumpStatement::Tag::GOTO;
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::CONTINUE)
        && isMatch(Punctuator::Tag::SEMI_COL))
        ret.tag = JumpStatement::Tag::CONTINUE;
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::BREAK)
        && isMatch(Punctuator::Tag::SEMI_COL))
        ret.tag = JumpStatement::Tag::BREAK;
    else if((mIdx = preidx, true)
        && isMatch(Keyword::Tag::RETURN)
        && (ret.uni.returnExpression = tokExpression(), true)
        && isMatch(Punctuator::Tag::SEMI_COL))
        ret.tag = JumpStatement::Tag::RETURN;
    
    if(ret.tag != JumpStatement::Tag::NONE)
        return new JumpStatement(ret);
    else
        return mIdx = preidx, nullptr;
}

Designation *Translator::tokDesignation()
{
    Designation ret;
    std::size_t preidx = mIdx;

    if((ret.designatorList = tokDesignatorList()) != nullptr
        && isMatch(Punctuator::Tag::ASS))
        return new Designation(ret);
    else
        return mIdx = preidx, nullptr;
}

LogicalANDExpression *Translator::tokLogicalANDExpression()
{
    std::vector<InclusiveORExpression*> ioevec;
    std::size_t preidx = mIdx;
    InclusiveORExpression *ioe = tokInclusiveORExpression();

    while(ioe != nullptr)
    {
        ioevec.push_back(ioe);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::AND))
            ioe = tokInclusiveORExpression();
        else
            ioe = nullptr;
    }

    mIdx = preidx;

    if(!ioevec.empty())
    {
        LogicalANDExpression *ret = new LogicalANDExpression();
        ret->vec = std::move(ioevec);
        return ret;
    }
    else
        return nullptr;
}

StructDeclaratorList *Translator::tokStructDeclaratorList()
{
    std::vector<StructDeclarator*> sdvec;
    std::size_t preidx = mIdx;
    StructDeclarator *sd = tokStructDeclarator();

    while(sd != nullptr)
    {
        sdvec.push_back(sd);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::COMMA))
            sd = tokStructDeclarator();
        else
            sd = nullptr;
    }

    mIdx = preidx;

    if(!sdvec.empty())
    {
        StructDeclaratorList *ret = new StructDeclaratorList();
        ret->sdvec = std::move(sdvec);
        return ret;
    }
    else
        return nullptr;
}

EnumerationConstant *Translator::tokEnumerationConstant()
{
    EnumerationConstant ret;

    if((ret.identifier = tokIdentifier()) != nullptr)
        return new EnumerationConstant(ret);
    else
        return nullptr;
}

ArgumentExpressionList *Translator::tokArgumentExpressionList()
{
    std::vector<AssignmentExpression*> aevec;
    std::size_t preidx = mIdx;
    AssignmentExpression *ae = tokAssignmentExpression();

    while(ae != nullptr)
    {
        aevec.push_back(ae);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::COMMA))
            ae = tokAssignmentExpression();
        else
            ae = nullptr;
    }

    mIdx = preidx;

    if(!aevec.empty())
    {
        ArgumentExpressionList *ret = new ArgumentExpressionList();
        ret->aevec = std::move(aevec);
        return ret;
    }
    else
        return nullptr;
}

PrimaryExpression *Translator::tokPrimaryExpression()
{
    PrimaryExpression ret;
    std::size_t preidx = mIdx;

    if((ret.uni.identifier = tokIdentifier()) != nullptr)
        ret.tag = PrimaryExpression::Tag::IDENTIFIER;
    else if((ret.uni.constant = tokConstant()) != nullptr)
        ret.tag = PrimaryExpression::Tag::CONSTANT;
    else if((ret.uni.stringLiteral = tokStringLiteral()) != nullptr)
        ret.tag = PrimaryExpression::Tag::STRING_LITERAL;
    else if(isMatch(Punctuator::Tag::L_PAREN)
        && (ret.uni.expression = tokExpression()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN))
        ret.tag = PrimaryExpression::Tag::EXPRESSION;
    else if((mIdx = preidx, true)
        && (ret.uni.genericSelection = tokGenericSelection()) != nullptr)
        ret.tag = PrimaryExpression::Tag::GENERIC_SELECTION;
    
    if(ret.tag != PrimaryExpression::Tag::NONE)
        return new PrimaryExpression(ret);
    else
        return nullptr;
}

DesignatorList *Translator::tokDesignatorList()
{
    std::vector<Designator*> dvec;

    for(Designator *d = tokDesignator();
        d != nullptr;
        d = tokDesignator())
        dvec.push_back(d);
    
    if(!dvec.empty())
    {
        DesignatorList *ret = new DesignatorList();
        ret->dvec = std::move(dvec);
        return ret;
    }
    else
        return nullptr;
}

InclusiveORExpression *Translator::tokInclusiveORExpression()
{
    std::vector<ExclusiveORExpression*> eoevec;
    std::size_t preidx = mIdx;
    ExclusiveORExpression *eoe = tokExclusiveORExpression();

    while(eoe != nullptr)
    {
        eoevec.push_back(eoe);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::IN_OR))
            eoe = tokExclusiveORExpression();
        else
            eoe = nullptr;
    }

    mIdx = preidx;

    if(!eoevec.empty())
    {
        InclusiveORExpression *ret = new InclusiveORExpression();
        ret->vec = std::move(eoevec);
        return ret;
    }
    else
        return nullptr;
}

StructDeclarator *Translator::tokStructDeclarator()
{
    StructDeclarator ret;
    std::size_t preidx = mIdx;

    if((ret.declarator = tokDeclarator(), true)
        && isMatch(Punctuator::Tag::COLON)
        && (ret.constantExpression = tokConstantExpression()) != nullptr)
        return new StructDeclarator(ret);
    else if((mIdx = preidx, true)
        && (ret.declarator = tokDeclarator()) != nullptr
        && (ret.constantExpression = nullptr, true))
        return new StructDeclarator(ret);
    else
        return mIdx = preidx, nullptr;
}

Constant *Translator::tokConstant()
{
    if(isMatch(Token::Tag::CONSTANT))
        return mTvec[mIdx - 1]->uni.constant;
    else
        return nullptr;
}

GenericSelection *Translator::tokGenericSelection()
{
    GenericSelection ret;
    std::size_t preidx = mIdx;

    if(isMatch(Keyword::Tag::GENERIC)
        && isMatch(Punctuator::Tag::L_PAREN)
        && (ret.assignment = tokAssignmentExpression()) != nullptr
        && isMatch(Punctuator::Tag::COMMA)
        && (ret.generic = tokGenericAssocList()) != nullptr
        && isMatch(Punctuator::Tag::R_PAREN))
        return new GenericSelection(ret);
    else
        return mIdx = preidx, nullptr;
}

Designator *Translator::tokDesignator()
{
    Designator ret;
    std::size_t preidx = mIdx;

    if(isMatch(Punctuator::Tag::SL_PAREN)
        && (ret.uni.constant = tokConstantExpression()) != nullptr
        && isMatch(Punctuator::Tag::SR_PAREN))
        ret.tag = Designator::Tag::CONSTANT;
    else if((mIdx = preidx, true)
        && isMatch(Punctuator::Tag::DOT)
        && (ret.uni.identifier = tokIdentifier()) != nullptr)
        ret.tag = Designator::Tag::IDENTIFIER;
    
    if(ret.tag != Designator::Tag::NONE)
        return new Designator(ret);
    else
        return mIdx = preidx, nullptr;
}

ExclusiveORExpression *Translator::tokExclusiveORExpression()
{
    std::vector<ANDExpression*> aevec;
    std::size_t preidx = mIdx;
    ANDExpression *ae = tokANDExpression();

    while(ae != nullptr)
    {
        aevec.push_back(ae);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::EX_OR))
            ae = tokANDExpression();
        else
            ae = nullptr;
    }

    mIdx = preidx;

    if(!aevec.empty())
    {
        ExclusiveORExpression *ret = new ExclusiveORExpression();
        ret->vec = std::move(aevec);
        return ret;
    }
    else
        return nullptr;
}

GenericAssocList *Translator::tokGenericAssocList()
{
    std::vector<GenericAssociation*> gavec;
    std::size_t preidx = mIdx;
    GenericAssociation *ga = tokGenericAssociation();

    while(ga != nullptr)
    {
        gavec.push_back(ga);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::COMMA))
            ga = tokGenericAssociation();
        else
            ga = nullptr;
    }

    mIdx = preidx;

    if(!gavec.empty())
    {
        GenericAssocList *ret = new GenericAssocList();
        ret->gavec = std::move(gavec);
        return ret;
    }
    else
        return nullptr;
}

ANDExpression *Translator::tokANDExpression()
{
    std::vector<EqualityExpression*> eevec;
    std::size_t preidx = mIdx;
    EqualityExpression *ee = tokEqualityExpression();

    while(ee != nullptr)
    {
        eevec.push_back(ee);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::AMP))
            ee = tokEqualityExpression();
        else
            ee = nullptr;
    }

    mIdx = preidx;

    if(!eevec.empty())
    {
        ANDExpression *ret = new ANDExpression();
        ret->vec = std::move(eevec);
        return ret;
    }
    else
        return nullptr;
}

GenericAssociation *Translator::tokGenericAssociation()
{
    GenericAssociation ret;
    std::size_t preidx = mIdx;

    if(isMatch(Keyword::Tag::DEFAULT)
        && isMatch(Punctuator::Tag::COLON)
        && (ret.assignment = tokAssignmentExpression()) != nullptr)
        return new GenericAssociation(ret);
    else if((mIdx = preidx, true)
        && (ret.typeName = tokTypeName()) != nullptr
        && isMatch(Punctuator::Tag::COLON)
        && (ret.assignment = tokAssignmentExpression()) != nullptr)
        return new GenericAssociation(ret);
    else
        return mIdx = preidx, nullptr;
}

EqualityExpression *Translator::tokEqualityExpression()
{
    std::vector<EqualityExpression::SEqualityExpression> seevec;
    std::size_t preidx = mIdx;
    EqualityExpression::SEqualityExpression see;
    see.re = tokRelationalExpression();

    while(see.re != nullptr)
    {
        seevec.push_back(see);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::EQUAL))
            see.tag = EqualityExpression::Tag::EQUAL;
        else if(isMatch(Punctuator::Tag::NOT_EQ))
            see.tag = EqualityExpression::Tag::NOT_EQUAL;
        else
            see.tag = EqualityExpression::Tag::NONE;

        if(see.tag != EqualityExpression::Tag::NONE)
            see.re = tokRelationalExpression();
        else
            see.re = nullptr;
    }

    mIdx = preidx;

    if(!seevec.empty())
    {
        EqualityExpression *ret = new EqualityExpression();
        ret->seevec = std::move(seevec);
        return ret;
    }
    else
        return nullptr;
}

RelationalExpression *Translator::tokRelationalExpression()
{
    std::vector<RelationalExpression::SRelationalExpression> srevec;
    std::size_t preidx = mIdx;
    RelationalExpression::SRelationalExpression sre;
    sre.se = tokShiftExpression();

    while(sre.se != nullptr)
    {
        srevec.push_back(sre);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::LESS))
            sre.tag = RelationalExpression::Tag::LESS;
        else if(isMatch(Punctuator::Tag::GREATER))
            sre.tag = RelationalExpression::Tag::GREATER;
        else if(isMatch(Punctuator::Tag::LESS_EQ))
            sre.tag = RelationalExpression::Tag::LESS_EQ;
        else if(isMatch(Punctuator::Tag::GREATER_EQ))
            sre.tag = RelationalExpression::Tag::GREATER_EQ;
        else
            sre.tag = RelationalExpression::Tag::NONE;
        
        if(sre.tag != RelationalExpression::Tag::NONE)
            sre.se = tokShiftExpression();
        else
            sre.se = nullptr;
    }

    mIdx = preidx;

    if(!srevec.empty())
    {
        RelationalExpression *ret = new RelationalExpression();
        ret->srevec = std::move(srevec);
        return ret;
    }
    else
        return nullptr;
}

ShiftExpression *Translator::tokShiftExpression()
{
    std::vector<ShiftExpression::SShiftExpression> ssevec;
    std::size_t preidx = mIdx;
    ShiftExpression::SShiftExpression sse;
    sse.ae = tokAdditiveExpression();

    while(sse.ae != nullptr)
    {
        ssevec.push_back(sse);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::L_SHIFT))
            sse.tag = ShiftExpression::Tag::L_SHIFT;
        else if(isMatch(Punctuator::Tag::R_SHIFT))
            sse.tag = ShiftExpression::Tag::R_SHIFT;
        else
            sse.tag = ShiftExpression::Tag::NONE;
        
        if(sse.tag != ShiftExpression::Tag::NONE)
            sse.ae = tokAdditiveExpression();
        else
            sse.ae = nullptr;
    }

    mIdx = preidx;

    if(!ssevec.empty())
    {
        ShiftExpression *ret = new ShiftExpression();
        ret->ssevec = std::move(ssevec);
        return ret;
    }
    else
        return nullptr;
}

AdditiveExpression *Translator::tokAdditiveExpression()
{
    std::vector<AdditiveExpression::SAdditiveExpression> saevec;
    std::size_t preidx = mIdx;
    AdditiveExpression::SAdditiveExpression sae;
    sae.me = tokMultiplicativeExpression();

    while(sae.me != nullptr)
    {
        saevec.push_back(sae);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::PLUS))
            sae.tag = AdditiveExpression::Tag::ADD;
        else if(isMatch(Punctuator::Tag::MINUS))
            sae.tag = AdditiveExpression::Tag::SUB;
        else
            sae.tag = AdditiveExpression::Tag::NONE;
        
        if(sae.tag != AdditiveExpression::Tag::NONE)
            sae.me = tokMultiplicativeExpression();
        else
            sae.me = nullptr;
    }

    mIdx = preidx;

    if(!saevec.empty())
    {
        AdditiveExpression *ret = new AdditiveExpression();
        ret->saevec = std::move(saevec);
        return ret;
    }
    else
        return nullptr;
}

MultiplicativeExpression *Translator::tokMultiplicativeExpression()
{
    std::vector<MultiplicativeExpression::SMultiplicativeExpression> smevec;
    std::size_t preidx = mIdx;
    MultiplicativeExpression::SMultiplicativeExpression sme;
    sme.ce = tokCastExpression();

    while(sme.ce != nullptr)
    {
        smevec.push_back(sme);
        preidx = mIdx;

        if(isMatch(Punctuator::Tag::AST))
            sme.tag = MultiplicativeExpression::Tag::MUL;
        else if(isMatch(Punctuator::Tag::SLASH))
            sme.tag = MultiplicativeExpression::Tag::DIV;
        else if(isMatch(Punctuator::Tag::PER))
            sme.tag = MultiplicativeExpression::Tag::REM;
        else
            sme.tag = MultiplicativeExpression::Tag::NONE;
        
        if(sme.tag != MultiplicativeExpression::Tag::NONE)
            sme.ce = tokCastExpression();
        else
            sme.ce = nullptr;
    }

    mIdx = preidx;

    if(!smevec.empty())
    {
        MultiplicativeExpression *ret = new MultiplicativeExpression();
        ret->smevec = std::move(smevec);
        return ret;
    }
    else
        return nullptr;
}

bool Translator::isMatch(Punctuator::Tag tag, bool isConsumed)
{
    if(mIdx < mTvec.size()
        && mTvec[mIdx]->tag == Token::Tag::PUNCTUATOR
        && mTvec[mIdx]->uni.punctuator->tag == tag)
    {
        if(isConsumed)
            mIdx++;
        return true;
    }
    else
        return false;
}

bool Translator::isMatch(Keyword::Tag tag, bool isConsumed)
{
    if(mIdx < mTvec.size()
        && mTvec[mIdx]->tag == Token::Tag::KEYWORD
        && mTvec[mIdx]->uni.keyword->tag == tag)
    {
        if(isConsumed)
            mIdx++;
        return true;
    }
    else
        return false;
}

bool Translator::isMatch(Token::Tag tag, bool isConsumed)
{
    if(mIdx < mTvec.size()
        && mTvec[mIdx]->tag == tag)
    {
        if(isConsumed)
            mIdx++;
        return true;
    }
    else
        return false;
}

}