#include "expression_simbol.hpp"
#include "static_assert_simbol.hpp"
#include "declaration_simbol.hpp"

namespace TP7
{

std::string Declaration::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::DECLARATION):
            ret += uni.sDeclaration.declarationSpecifiers->string();
            ret.push_back(' ');
            if(uni.sDeclaration.initDeclaratorList != nullptr)
                ret += uni.sDeclaration.initDeclaratorList->string();
            break;
        case(Tag::STATIC_ASSERT):
            ret += uni.staticAssert->string();
            break;
        
        default:
            Simbol::unexpectTag("Declaration");
            break;
    }

    return ret;
}

std::string DeclarationSpecifiers::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::STORAGE_CLASS_SPECIFIER):
            ret += uni.storageClassSpecifier->string();
            break;
        case(Tag::TYPE_SPECIFIER):
            ret += uni.typeSpecifier->string();
            break;
        case(Tag::TYPE_QUALIFIER):
            ret += uni.typeQualifier->string();
            break;
        case(Tag::FUNCTION_SPECIFIER):
            ret += uni.functionSpecifier->string();
            break;
        case(Tag::ALIGNMENT_SPECIFIER):
            ret += uni.alignmentSpecifier->string();
            break;
        
        default:
            Simbol::unexpectTag("DeclarationSpecifiers");
            break;
    }

    if(declarationSpecifiers != nullptr)
        ret += declarationSpecifiers->string();
    
    return ret;
}

std::string InitDeclaratorList::string() const
{
    std::string ret;

    for(auto &&id : idvec)
    {
        ret += id->string();
        ret.push_back(' ');
    }

    ret.pop_back();
    return ret;
}

std::string InitDeclarator::string() const
{
    std::string ret;

    ret += declarator->string();

    if(initializer != nullptr)
    {
        ret.push_back('=');
        ret += initializer->string();
    }

    return ret;
}

std::string StorageClassSpecifier::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::TYPEDEF):
            ret += "typedef";
            break;
        case(Tag::EXTERN):
            ret += "extern";
            break;
        case(Tag::STATIC):
            ret += "static";
            break;
        case(Tag::THREAD_LOCAL):
            ret += "_Thread_local";
            break;
        case(Tag::AUTO):
            ret += "auto";
            break;
        case(Tag::REGISTER):
            ret += "register";
            break;
        
        default:
            Simbol::unexpectTag("StorageClassSpecifier");
            break;
    }

    return ret;
}

std::string TypeSpecifier::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::VOID):
            ret += "void";
            break;
        case(Tag::CHAR):
            ret += "char";
            break;
        case(Tag::SHORT):
            ret += "short";
            break;
        case(Tag::INT):
            ret += "int";
            break;
        case(Tag::LONG):
            ret += "long";
            break;
        case(Tag::FLOAT):
            ret += "float";
            break;
        case(Tag::DOUBLE):
            ret += "double";
            break;
        case(Tag::SIGNED):
            ret += "signed";
            break;
        case(Tag::UNSIGNED):
            ret += "unsigned";
            break;
        case(Tag::BOOL):
            ret += "_Bool";
            break;
        case(Tag::COMPLEX):
            ret += "_Complex";
            break;
        case(Tag::ATOMIC_TYPE_SPECIFIER):
            ret += uni.atomicTypeSpecifier->string();
            break;
        case(Tag::STRUCT_OR_UNION_SPECIFIER):
            ret += uni.structOrUnionSpecifier->string();
            break;
        case(Tag::ENUM_SPECIFIER):
            ret += uni.enumSpecifier->string();
            break;
        case(Tag::TYPEDEF_NAME):
            ret += uni.typedefName->string();
            break;
        
        default:
            Simbol::unexpectTag("TypeSpecifier");
            break;
    }

    return ret;
}

std::string TypeQualifier::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::CONST):
            ret += "const";
            break;
        case(Tag::RESTRICT):
            ret += "restrict";
            break;
        case(Tag::VOLATILE):
            ret += "volatile";
            break;
        case(Tag::ATOMIC):
            ret += "_Atomic";
            break;

        default:
            Simbol::unexpectTag("TypeQualifier");
            break;
    }

    return ret;
}

std::string FunctionSpecifier::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::INLINE):
            ret += "inline";
            break;
        case(Tag::NORETURN):
            ret += "_Noreturn";
            break;

        default:
            Simbol::unexpectTag("FunctionSpecifier");
            break;
    }

    return ret;
}

std::string AlignmentSpecifier::string() const
{
    std::string ret = "_Alignas(";

    switch(tag)
    {
        case(Tag::TYPE_NAME):
            ret += uni.typeName->string();
            break;
        case(Tag::CONSTANT_EXPRESSION):
            ret += uni.constantExpression->string();
            break;
        
        default:
            Simbol::unexpectTag("AlignmentSpecifier");
            break;
    }

    ret.push_back(')');
    return ret;
}

std::string AtomicTypeSpecifier::string() const
{
    std::string ret = "_Atomic(";
    ret += typeName->string();
    ret.push_back(')');

    return ret;
}

std::string StructOrUnionSpecifier::string() const
{
    std::string ret;

    ret += structOrUnion->string();
    ret.push_back(' ');

    switch(tag)
    {
        case(Tag::IDENTIFIER_STRUCT_DECLARATION_LIST):
            if(identifier != nullptr)
                ret += identifier->string();
            ret.push_back('{');
            ret += structDeclarationList->string();
            ret.push_back('}');
            break;
        case(Tag::IDENTIFIER):
            ret += identifier->string();
            break;
        
        default:
            Simbol::unexpectTag("StructOrUnionSpecifier");
            break;
    }

    return ret;
}

std::string StructOrUnion::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::STRUCT):
            ret += "struct";
            break;
        case(Tag::UNION):
            ret += "union";
            break;
        
        default:
            Simbol::unexpectTag("StructOrUnion");
            break;
    }

    return ret;
}

std::string StructDeclarationList::string() const
{
    std::string ret;
    for(auto &&sd : sdvec)
        ret += sd->string();

    return ret;
}

std::string StructDeclaration::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::DECLARATOR):
            ret += uni.sDeclarator.specifierQualifierList->string();
            if(uni.sDeclarator.structDeclaratorList != nullptr)
                ret += uni.sDeclarator.structDeclaratorList->string();
            ret.push_back(';');
            break;
        case(Tag::STATIC_ASSERT):
            ret += uni.staticAssert->string();
            break;
        
        default:
            Simbol::unexpectTag("StructDeclaration");
            break;
    }

    return ret;
}

std::string SpecifierQualifierList::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::SPECIFIER):
            ret += uni.specifier->string();
            break;
        case(Tag::QUALIFIER):
            ret += uni.qualifier->string();
            break;
        
        default:
            Simbol::unexpectTag("SpecifierQualifierList");
            break;
    }

    if(specifierQualifierList != nullptr)
    {
        ret.push_back(' ');
        ret += specifierQualifierList->string();
    }

    return ret;
}

std::string StructDeclaratorList::string() const
{
    std::string ret;

    for(auto &&sd : sdvec)
    {
        ret += sd->string();
        ret.push_back(',');
    }

    ret.pop_back();

    return ret;
}

std::string StructDeclarator::string() const
{
    std::string ret;

    if(constantExpression == nullptr)
        ret += declarator->string();
    else
    {
        if(declarator != nullptr)
            ret += declarator->string();
        ret.push_back(':');
        ret += constantExpression->string();
    }

    return ret;
}

std::string TypedefName::string() const
{
    return identifier->string();
}

std::string EnumSpecifier::string() const
{
    std::string ret = "enum";

    if(identifier != nullptr)
    {
        ret.push_back(' ');
        ret += identifier->string();
    }

    if(enumeratorList != nullptr)
    {
        ret.push_back('{');
        ret += enumeratorList->string();
        ret.push_back('}');
    }

    return ret;
}

std::string EnumeratorList::string() const
{
    std::string ret;

    for(auto &&e : evec)
    {
        ret += e->string();
        ret.push_back(',');
    }

    ret.pop_back();

    return ret;
}

std::string Enumerator::string() const
{
    std::string ret;

    ret += enumerationConstant->string();

    if(constantExpression != nullptr)
    {
        ret.push_back('=');
        ret += constantExpression->string();
    }

    return ret;
}

std::string EnumerationConstant::string() const
{
    return identifier->string();
}

std::string Initializer::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::ASSIGNMENT):
            ret += uni.assignment->string();
            break;
        case(Tag::INITIALIZER):
            ret.push_back('{');
            ret += uni.initializer->string();
            ret.push_back('}');
            break;
        
        default:
            Simbol::unexpectTag("Initializer");
            break;
    }

    return ret;
}

std::string InitializerList::string() const
{
    std::string ret;

    for(auto &&il : ilvec)
    {
        if(il.designation != nullptr)
        {
            ret += il.designation->string();
            ret.push_back(' ');
        }
        ret += il.initializer->string();
        ret.push_back(',');
    }

    ret.pop_back();

    return ret;
}

std::string Designation::string() const
{
    std::string ret = designatorList->string();
    ret.push_back('=');
    return ret;
}

std::string DesignatorList::string() const
{
    std::string ret;
    for(auto &&d : dvec)
        ret += d->string();

    return ret;
}

std::string Designator::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::CONSTANT):
            ret.push_back('[');
            ret += uni.constant->string();
            ret.push_back(']');
            break;
        case(Tag::IDENTIFIER):
            ret.push_back('.');
            ret += uni.identifier->string();
            break;
        
        default:
            Simbol::unexpectTag("Designator");
            break;
    }

    return ret;
}

std::string Declarator::string() const
{
    std::string ret;
    if(pointer != nullptr)
        ret += pointer->string();

    ret += directDeclarator->string();

    return ret;
}

std::string DirectDeclarator::string() const
{
    std::string ret;

    for(auto &&sdd : sddvec)
    {
        switch(sdd.tag)
        {
            case(Tag::IDENTIFIER):
                ret += sdd.uni.identifier->string();
                break;
            case(Tag::DECLARATOR):
                ret.push_back('(');
                ret += sdd.uni.declarator->string();
                ret.push_back(')');
                break;
            case(Tag::QUALIFIER_ASSIGNMENT):
                ret.push_back('[');
                if(sdd.uni.sQualifierAssignment.qualifier != nullptr)
                {
                    ret += sdd.uni.sQualifierAssignment.qualifier->string();
                    ret.push_back(' ');
                }
                if(sdd.uni.sQualifierAssignment.assignment != nullptr)
                    ret += sdd.uni.sQualifierAssignment.assignment->string();
                ret.push_back(']');
                break;
            case(Tag::STATIC_QUALIFIER_ASSIGNMENT):
                ret += "[static ";
                if(sdd.uni.sStaticQualifierAssignment.qualifier != nullptr)
                {
                    ret += sdd.uni.sStaticQualifierAssignment.qualifier->string();
                    ret.push_back(' ');
                }
                ret += sdd.uni.sStaticQualifierAssignment.assignment->string();
                ret.push_back(']');
                break;
            case(Tag::QUALIFIER_STATIC_ASSIGNMENT):
                ret.push_back('[');
                ret += sdd.uni.sQualifierStaticAssignment.qualifier->string();
                ret += " static ";
                ret += sdd.uni.sQualifierStaticAssignment.assginment->string();
                ret.push_back(']');
                break;
            case(Tag::QUALIFIER):
                ret.push_back('[');
                if(sdd.uni.qualifier != nullptr)
                    ret += sdd.uni.qualifier->string();
                ret += "*]";
                break;
            case(Tag::PARAMETER):
                ret.push_back('(');
                ret += sdd.uni.parameter->string();
                ret.push_back(')');
                break;
            case(Tag::IDENTIFIER_LIST):
                ret.push_back('(');
                if(sdd.uni.identifier != nullptr)
                    ret += sdd.uni.identifierList->string();
                ret.push_back(')');
                break;
            
            default:
                Simbol::unexpectTag("DirectDeclarator");
                break;
        }
    }

    return ret;
}

std::string Pointer::string() const
{
    std::string ret(1, '*');
    
    if(typeQualifierList != nullptr)
        ret += typeQualifierList->string();

    if(pointer != nullptr)
        ret += pointer->string();

    return ret;
}

std::string TypeQualifierList::string() const
{
    std::string ret;
    for(auto &&tq : tqvec)
    {
        ret += tq->string();
        ret.push_back(' ');
    }

    ret.pop_back();
    return ret;
}

std::string ParameterTypeList::string() const
{
    std::string ret;

    switch(tag)
    {
        case(Tag::PARAMETER_LIST):
            ret += parameterList->string();
            break;
        case(Tag::VALRIABLE_PARALETER_LIST):
            ret += parameterList->string();
            ret += ",...";
            break;
        
        default:
            Simbol::unexpectTag("ParameterTypeList");
            break;
    }

    return ret;
}

std::string ParameterList::string() const
{
    std::string ret;

    for(auto &&pd : pdvec)
    {
        ret += pd->string();
        ret.push_back(',');
    }

    ret.pop_back();
    return ret;
}

std::string ParameterDeclaration::string() const
{
    std::string ret = declarationSpecifiers->string();
    
    switch(tag)
    {
        case(Tag::DECLARATOR):
            ret.push_back(' ');
            ret += uni.declarator->string();
            break;
        case(Tag::ABSTRACT_DECLARATOR):
            if(uni.abstractDeclarator != nullptr)
            {
                ret.push_back(' ');
                ret += uni.abstractDeclarator->string();
            }
            break;
        
        default:
            Simbol::unexpectTag("ParameterDeclaration");
            break;
    }

    return ret;
}

std::string TypeName::string() const
{
    std::string ret = specifierQualifierList->string();

    if(abstractDeclarator != nullptr)
        ret += abstractDeclarator->string();

    return ret;
}

std::string AbstractDeclarator::string() const
{
    std::string ret;

    if(pointer != nullptr)
        ret += pointer->string();
    if(declarator != nullptr)
        ret += declarator->string();
    
    return ret;
}

std::string DirectAbstractDeclarator::string() const
{
    std::string ret;

    for(auto &&sdad : sdadvec)
    {
        switch(sdad.tag)
        {
            case(Tag::ABSTRACT):
                ret.push_back('(');
                ret += sdad.uni.abstract->string();
                ret.push_back(')');
                break;
            case(Tag::QUALIFIER_ASSIGNMENT):
                ret.push_back('[');
                if(sdad.uni.sQualifierAssignment.qualifier != nullptr)
                    ret += sdad.uni.sQualifierAssignment.qualifier->string();
                if(sdad.uni.sQualifierAssignment.assignment != nullptr)
                    ret += sdad.uni.sQualifierAssignment.assignment->string();
                ret.push_back(']');
                break;
            case(Tag::STATIC_QUALIFIER_ASSIGNMENT):
                ret += "[static ";
                if(sdad.uni.sStaticQualifierAssignment.qualifier != nullptr)
                {
                    ret += sdad.uni.sStaticQualifierAssignment.qualifier->string();
                    ret.push_back(' ');
                }
                ret += sdad.uni.sStaticQualifierAssignment.assignment->string();
                ret.push_back(']');
                break;
            case(Tag::QUALIFIER_STATIC_ASSIGNMENT):
                ret.push_back('[');
                ret += sdad.uni.sQualifierStaticAssignment.qualifier->string();
                ret += " static ";
                ret += sdad.uni.sQualifierStaticAssignment.assignment->string();
                ret.push_back(']');
                break;
            case(Tag::ASTERISK):
                ret += "[*]";
                break;
            case(Tag::PARAMETER):
                ret.push_back('(');
                if(sdad.uni.parameter != nullptr)
                    ret += sdad.uni.parameter->string();
                ret.push_back(')');
                break;
            
            default:
                Simbol::unexpectTag("DirectAbstractDeclarator");
                break;
        }
    }

    return ret;
}

}