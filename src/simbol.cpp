#include <string>
#include <iostream>

#include "simbol.hpp"

std::vector<BaseSimbol*> BaseSimbol::ALLOCATED_SIMBOLS;

const std::unordered_map<std::string, Keyword::Tag> Keyword::KEYWORD_MAP
    = {{"auto", Tag::AUTO},
       {"break", Tag::BREAK},
       {"case", Tag::CASE},
       {"char", Tag::CHAR},
       {"const", Tag::CONST},
       {"continue", Tag::CONTINUE},
       {"default", Tag::DEFAULT},
       {"do", Tag::DO},
       {"double", Tag::DOUBLE},
       {"else", Tag::ELSE},
       {"enum", Tag::ENUM},
       {"extern", Tag::EXTERN},
       {"float", Tag::FLOAT},
       {"for", Tag::FOR},
       {"goto", Tag::GOTO},
       {"if", Tag::IF},
       {"inline", Tag::INLINE},
       {"int", Tag::INT},
       {"long", Tag::LONG},
       {"register", Tag::REGISTER},
       {"restrict", Tag::RESTRICT},
       {"return", Tag::RETURN},
       {"short", Tag::SHORT},
       {"signed", Tag::SIGNED},
       {"sizeof", Tag::SIZEOF},
       {"static", Tag::STATIC},
       {"struct", Tag::STRUCT},
       {"switch", Tag::SWITCH},
       {"typedef", Tag::TYPEDEF},
       {"union", Tag::UNION},
       {"unsigned", Tag::UNSIGNED},
       {"void", Tag::VOID},
       {"volatile", Tag::VOLATILE},
       {"while", Tag::WHILE},
       {"_Alignas", Tag::ALIGNAS},
       {"_Alignof", Tag::ALIGNOF},
       {"_Atomic", Tag::ATOMIC},
       {"_Bool", Tag::BOOL},
       {"_Complex", Tag::COMPLEX},
       {"_Generic", Tag::GENERIC},
       {"_Imaginary", Tag::IMAGINARY},
       {"_Noreturn", Tag::NORETURN},
       {"_Static_assert", Tag::STATIC_ASSERT},
       {"_Thread_local", Tag::THREAD_LOCAL}};

const std::unordered_map<std::string, Punctuator::Tag> Punctuator::PUNCTUATOR_MAP
    = {{"[", Tag::SL_PAREN},
       {"]", Tag::SR_PAREN},
       {"(", Tag::L_PAREN},
       {")", Tag::R_PAREN},
       {"{", Tag::BL_PAREN},
       {"}", Tag::BR_PAREN},
       {".", Tag::DOT},
       {"->", Tag::ARROR},
       {"++", Tag::INC},
       {"--", Tag::DEC},
       {"&", Tag::AMP},
       {"*", Tag::AST},
       {"+", Tag::PLUS},
       {"-", Tag::MINUS},
       {"~", Tag::TILDE},
       {"!", Tag::EXC},
       {"/", Tag::SLASH},
       {"%", Tag::PER},
       {"<<", Tag::L_SHIFT},
       {">>", Tag::R_SHIFT},
       {"<", Tag::LESS},
       {">", Tag::GREATER},
       {"<=", Tag::LESS_EQ},
       {">=", Tag::GREATER_EQ},
       {"==", Tag::EQUAL},
       {"!=", Tag::NOT_EQ},
       {"^", Tag::EX_OR},
       {"|", Tag::IN_OR},
       {"&&", Tag::AND},
       {"||", Tag::OR},
       {"\?", Tag::QUE},
       {":", Tag::COLON},
       {";", Tag::SEMI_COL},
       {"...", Tag::DDD},
       {"=", Tag::ASS},
       {"*=", Tag::MUL_ASS},
       {"/=", Tag::DIV_ASS},
       {"%=", Tag::REM_ASS},
       {"+=", Tag::ADD_ASS},
       {"-=", Tag::SUB_ASS},
       {"<<=", Tag::LSFT_ASS},
       {">>=", Tag::RSFT_ASS},
       {"&=", Tag::BW_AND_ASS},
       {"^=", Tag::EX_OR},
       {"|=", Tag::IN_OR},
       {",", Tag::COMMA},
       {"#", Tag::HASH},
       {"##", Tag::HSHS},
       {"<:", Tag::SL_PAREN},
       {":>", Tag::SR_PAREN},
       {"<%", Tag::BL_PAREN},
       {"%>", Tag::BR_PAREN},
       {"%:", Tag::HASH},
       {"%:%:", Tag::HSHS}};

void Simbol::destroy() noexcept
{
    for(auto &&bs : BaseSimbol::ALLOCATED_SIMBOLS)
        delete bs;
    
    BaseSimbol::ALLOCATED_SIMBOLS.clear();
}

bool Simbol::unexpectTag(const char *className) noexcept
{
    std::cerr << "implementation error:\n"
                 "    what: unexcepted enumeration.\n"
                 "    class name: " << className
              << std::endl;
    return false;
}

void *BaseSimbol::operator new(std::size_t size)
{
    void *alloc = ::operator new(size);
    ALLOCATED_SIMBOLS.push_back(static_cast<BaseSimbol*>(alloc));
    return alloc;
}

BaseSimbol::~BaseSimbol() noexcept
{
}

std::string Token::string() const
{
    std::string retval;
    switch(tag)
    {
        case(Tag::KEYWORD):
            retval = uni.keyword->string();
            break;
        case(Tag::IDENTIFIER):
            retval = uni.identifier->string();
            break;
        case(Tag::CONSTANT):
            retval = uni.constant->string();
            break;
        case(Tag::STRING_LITERAL):
            retval = uni.stringLiteral->string();
            break;
        case(Tag::PUNCTUATOR):
            retval = uni.punctuator->string();
            break;
        
        default:
            Simbol::unexpectTag("Token");
    }

    return retval;
}

std::string Keyword::string() const
{
    std::string retval;
    for(auto &&p : KEYWORD_MAP)
    {
        if(p.second == tag)
        {
            retval = p.first;
            break;
        }
    }

    return retval;
}

std::string Constant::string() const
{
    std::string retval;
    switch(tag)
    {
        case(Tag::INTEGER):
            retval = uni.integer->string();
            break;
        case(Tag::FLOATING):
            retval = uni.floating->string();
            break;
        case(Tag::ENUMERATION):
            retval = uni.enumeration->string();
            break;
        case(Tag::CHARACTER):
            retval = uni.character->string();
            break;
        
        default:
            Simbol::unexpectTag("Constant");
    }

    return retval;
}

std::string IntegerConstant::string() const
{
    std::string retval;

    switch(prefixTag)
    {
        case(PrefixTag::DECIMAL):
            break;
        case(PrefixTag::OCTAL):
            retval.push_back('0');
            break;
        case(PrefixTag::HEXADECIMAL):
            retval += "0x";
            break;
        
        default:
            Simbol::unexpectTag("IntegerConstant");
    }

    retval += str;

    switch(suffixTag)
    {
        case(SuffixTag::NONE):
            break;
        case(SuffixTag::UNSIGNED):
            retval.push_back('u');
            break;
        case(SuffixTag::UNSIGNED_LONG):
            retval += "ul";
            break;
        case(SuffixTag::UNSIGNED_LONG_LONG):
            retval += "ull";
            break;
        case(SuffixTag::LONG):
            retval.push_back('l');
            break;
        case(SuffixTag::LONG_LONG):
            retval += "ll";
            break;
        
        default:
            Simbol::unexpectTag("IntegerConstant");
    }

    return retval;
}

std::string FloatingConstant::string() const
{
    std::string retval;

    switch(radixTag)
    {
        case(RadixTag::DECIMAL):
            break;
        case(RadixTag::HEXADECIMAL):
            retval += "0x";
            break;
        
        default:
            Simbol::unexpectTag("FloatingConstant");
    }

    retval += integer;
    retval.push_back('.');
    retval += decimal;

    switch(radixTag)
    {
        case(RadixTag::DECIMAL):
            retval.push_back('e');
            break;
        case(RadixTag::HEXADECIMAL):
            retval.push_back('p');
            break;
        
        default:
            Simbol::unexpectTag("FloatingConstant");
    }

    switch(exponentTag)
    {
        case(ExponentTag::NONE):
            retval.push_back('+');
            break;
        case(ExponentTag::PLUS):
            retval.push_back('+');
            break;
        case(ExponentTag::MINUS):
            retval.push_back('-');
            break;
        
        default:
            Simbol::unexpectTag("FloatingConstant");
    }

    retval += exponent;

    switch(suffixTag)
    {
        case(SuffixTag::NONE):
            break;
        case(SuffixTag::FLOAT):
            retval.push_back('f');
            break;
        case(SuffixTag::LONG):
            retval.push_back('l');
            break;
        
        default:
            Simbol::unexpectTag("FloatingConstant");
    }

    return retval;
}

std::string EnumerationConstant::string() const
{
    return identifier->string();
}

std::string CharacterConstant::string() const
{
    std::string retval = "'";
    retval += str;
    retval.push_back('\'');
    return retval;
}

std::string HCharSequence::string() const
{
    std::string retval = "<";
    retval += str;
    retval.push_back('>');
    return retval;
}

std::string HeaderName::string() const
{
    std::string retval;

    switch(tag)
    {
        case(Tag::H):
            retval.push_back('<');
            retval += uni.h->string();
            retval.push_back('>');
            break;
        case(Tag::Q):
            retval.push_back('"');
            retval += uni.q->string();
            retval.push_back('"');
            break;
        
        default:
            Simbol::unexpectTag("HeaderName");
            break;
    }

    return retval;
}

std::string Identifier::string() const
{
    return str;
}

std::string PPNumber::string() const
{
    return str;
}

std::string PPToken::string() const
{
    std::string retval = isGlued ? "" : " ";

    switch(tag)
    {
        case(Tag::HEADER_NAME):
            retval += uni.headerName->string();
            break;
        case(Tag::IDENTIFIER):
            retval += uni.identifier->string();
            break;
        case(Tag::PP_NUMBER):
            retval += uni.ppNumber->string();
            break;
        case(Tag::CHARACTER_CONSTANT):
            retval += uni.characterConstant->string();
            break;
        case(Tag::STRING_LITERAL):
            retval += uni.stringLiteral->string();
            break;
        case(Tag::PUNCTUATOR):
            retval += uni.punctuator->string();
            break;
        case(Tag::OTHER):
            retval += std::string(1, uni.other);
            break;

        default:
            Simbol::unexpectTag("PPToken");
    }

    return retval;
}

std::string Punctuator::string() const
{
    std::string retval;

    for(auto&& p : PUNCTUATOR_MAP)
    {
        if(p.second == tag)
        {
            retval = p.first;
            break;
        }
    }

    if(retval.empty())
        Simbol::unexpectTag("Punctuator");
    
    return retval;
}

std::string QCharSequence::string() const
{
    std::string retval = "\"";
    retval += str;
    retval.push_back('"');
    return retval;
}

std::string StringLiteral::string() const
{
    std::string retval = "\"";
    retval += str;
    retval.push_back('"');
    return retval;
}

std::string IdentifierList::string() const
{
    std::string retval;
    for(std::size_t i = 0; i < ivec.size(); i++)
    {
        retval += ivec[i]->string();
        if(i + 1 != ivec.size())
            retval += ", ";
    }

    return retval;
}

std::string ControlLine::string() const
{
    std::string retval = std::string(1, '#');

    switch(tag)
    {
        case(Tag::INCLUDE):
            retval += "include ";
            retval += uni.sInclude.ppTokens->string();
            retval.push_back('\n');
            break;
        case(Tag::DEFINE):
            retval += "define ";
            retval += uni.sDefine.identifier->string();
            retval.push_back(' ');
            retval += uni.sDefine.replacementList->string();
            retval.push_back('\n');
            break;
        case(Tag::DEFINE_IL):
            retval += "define ";
            retval += uni.sDefineIL.identifier->string();
            retval.push_back('(');
            if(uni.sDefineIL.identifierList != nullptr)
                retval += uni.sDefineIL.identifierList->string();
            retval += ") ";
            retval += uni.sDefineIL.replacementList->string();
            retval.push_back('\n');
            break;
        case(Tag::DEFINE_V):
            retval += "define ";
            retval += uni.sDefineV.identifier->string();
            retval += "(...) ";
            retval += uni.sDefineV.replacementList->string();
            retval.push_back('\n');
            break;
        case(Tag::DEFINE_ILV):
            retval += "define ";
            retval += uni.sDefineILV.identifier->string();
            retval.push_back('(');
            retval += uni.sDefineILV.identifierList->string();
            retval += ", ...) ";
            retval += uni.sDefineILV.replacementList->string();
            retval.push_back('\n');
            break;
        case(Tag::UNDEF):
            retval += "undef ";
            retval += uni.sUndef.identifier->string();
            retval.push_back('\n');
            break;
        case(Tag::LINE):
            retval += "line ";
            retval += uni.sLine.ppTokens->string();
            retval.push_back('\n');
            break;
        case(Tag::ERROR):
            retval += "error ";
            if(uni.sError.ppTokens != nullptr)
                retval += uni.sError.ppTokens->string();
            retval.push_back('\n');
            break;
        case(Tag::PRAGMA):
            retval += "pragma ";
            if(uni.sPragma.ppTokens != nullptr)
                retval += uni.sPragma.ppTokens->string();
            retval.push_back('\n');
            break;
        case(Tag::NEW_LINE):
            retval.push_back('\n');
            break;

        default:
            Simbol::unexpectTag("ControlLine");
    }

    return retval;
}

std::string ElifGroup::string() const
{
    std::string retval = "#elif ";
    retval += ppTokens->string();
    retval.push_back('\n');
    if(group != nullptr)
        retval += group->string();

    return retval;
}

std::string ElifGroups::string() const
{
    std::string retval;
    for(auto&& eg : egvec)
        retval += eg->string();
    
    return retval;
}

std::string ElseGroup::string() const
{
    std::string retval = "#else\n";
    if(group != nullptr)
        retval += group->string();

    return retval;
}

std::string Group::string() const
{
    std::string retval;
    for(auto&& gp : gpvec)
        retval += gp->string();
    
    return retval;
}

std::string GroupPart::string() const
{
    std::string retval;

    switch(tag)
    {
        case(Tag::IF_SECTION):
            retval = uni.ifSection->string();
            break;
        case(Tag::CONTROL_LINE):
            retval = uni.controlLine->string();
            break;
        case(Tag::TEXT_LINE):
            retval = uni.textLine->string();
            break;
        case(Tag::NON_DIRECTIVE):
            retval.push_back('#');
            retval = uni.nonDirective->string();
            break;
        
        default:
            Simbol::unexpectTag("GroupPart");
    }

    return retval;
}

std::string IfGroup::string() const
{
    std::string retval = std::string(1, '#');
    
    switch(tag)
    {
        case(Tag::IF):
            retval += "if ";
            retval += uni.sIf.ppTokens->string();
            retval.push_back('\n');
            if(uni.sIf.group != nullptr)
                retval += uni.sIf.group->string();
            break;
        case(Tag::IFDEF):
            retval += "ifdef ";
            retval += uni.sIfdef.identifier->string();
            retval.push_back('\n');
            if(uni.sIfdef.group != nullptr)
                retval += uni.sIfdef.group->string();
            break;
        case(Tag::IFNDEF):
            retval += "ifndef ";
            retval += uni.sIfndef.identifier->string();
            retval.push_back('\n');
            if(uni.sIfndef.group != nullptr)
                retval += uni.sIfndef.group->string();
            break;
        
        default:
            Simbol::unexpectTag("IfGroup");
    }

    return retval;
}

std::string IfSection::string() const
{
    std::string retval = ifGroup->string();
    if(elifGroups != nullptr)
        retval += elifGroups->string();
    if(elseGroup != nullptr)
        retval += elseGroup->string();
    
    retval += "#endif\n";
    return retval;
}

std::string NonDirective::string() const
{
    std::string retval = ppTokens->string();
    retval.push_back('\n');
    return retval;
}

std::string PPFile::string() const
{
    return (group != nullptr) ? group->string() : std::string();
}

std::string PPTokens::string() const
{
    std::string retval;
    for(auto&& pt : ptvec)
        retval += pt->string();

    return retval;
}

std::string ReplacementList::string() const
{
    return (ppTokens != nullptr) ? ppTokens->string() : std::string();
}

std::string TextLine::string() const
{
    std::string retval = (ppTokens != nullptr) ? ppTokens->string() : std::string();
    retval.push_back('\n');
    return retval;
}