#include <string>
#include <iostream>

#include "simbol.hpp"

std::vector<BaseSimbol*> BaseSimbol::ALLOCATED_SIMBOLS;

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

bool BaseSimbol::unexpectTag(const char *className) noexcept
{
    std::cerr << "implementation error:\n"
                 "    class-name: " << className
              << std::endl;

    return false;
}

void BaseSimbol::destroy()
{
    for(auto&& bs : ALLOCATED_SIMBOLS)
        delete bs;
    
    ALLOCATED_SIMBOLS.clear();
}

PPToken *BaseSimbol::strToPt(std::string &&str)
{
    PPToken *retval = new PPToken();
    retval->tag = PPToken::Tag::STRING_LITERAL;
    retval->uni.stringLiteral = new StringLiteral();
    retval->uni.stringLiteral->str = str;
    return retval;
}

PPToken *BaseSimbol::numToPt(std::string &&str)
{
    PPToken *retval = new PPToken();
    retval->tag = PPToken::Tag::PP_NUMBER;
    retval->uni.ppNumber = new PPNumber();
    retval->uni.ppNumber->str = str;
    return retval;
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

std::string CharacterConstant::string()
{
    std::string retval = "'";
    retval += str;
    retval.push_back('\'');
    return retval;
}

std::string HCharSequence::string()
{
    std::string retval = "<";
    retval += str;
    retval.push_back('>');
    return retval;
}

std::string HeaderName::string()
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
            unexpectTag("HeaderName");
            break;
    }

    return retval;
}

std::string Identifier::string()
{
    return str;
}

std::string PPNumber::string()
{
    return str;
}

std::string PPToken::string()
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
            unexpectTag("PPToken");
    }

    return retval;
}

std::string Punctuator::string()
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
        unexpectTag("Punctuator");
    
    return retval;
}

std::string QCharSequence::string()
{
    std::string retval = "\"";
    retval += str;
    retval.push_back('"');
    return retval;
}

std::string StringLiteral::string()
{
    std::string retval = "\"";
    retval += str;
    retval.push_back('"');
    return retval;
}

std::string IdentifierList::string()
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

std::string ControlLine::string()
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
            unexpectTag("ControlLine");
    }

    return retval;
}

std::string ElifGroup::string()
{
    std::string retval = "#elif ";
    retval += ppTokens->string();
    retval.push_back('\n');
    if(group != nullptr)
        retval += group->string();

    return retval;
}

std::string ElifGroups::string()
{
    std::string retval;
    for(auto&& eg : egvec)
        retval += eg->string();
    
    return retval;
}

std::string ElseGroup::string()
{
    std::string retval = "#else\n";
    if(group != nullptr)
        retval += group->string();

    return retval;
}

std::string Group::string()
{
    std::string retval;
    for(auto&& gp : gpvec)
        retval += gp->string();
    
    return retval;
}

std::string GroupPart::string()
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
            unexpectTag("GroupPart");
    }

    return retval;
}

std::string IfGroup::string()
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
            unexpectTag("IfGroup");
    }

    return retval;
}

std::string IfSection::string()
{
    std::string retval = ifGroup->string();
    if(elifGroups != nullptr)
        retval += elifGroups->string();
    if(elseGroup != nullptr)
        retval += elseGroup->string();
    
    retval += "#endif\n";
    return retval;
}

std::string NonDirective::string()
{
    std::string retval = ppTokens->string();
    retval.push_back('\n');
    return retval;
}

std::string PPFile::string()
{
    return (group != nullptr) ? group->string() : std::string();
}

std::string PPTokens::string()
{
    std::string retval;
    for(auto&& pt : ptvec)
    {
        retval += pt->string();
        retval.push_back('\n');
    }

    return retval;
}

std::string ReplacementList::string()
{
    return (ppTokens != nullptr) ? ppTokens->string() : std::string();
}

std::string TextLine::string()
{
    std::string retval = (ppTokens != nullptr) ? ppTokens->string() : std::string();
    retval.push_back('\n');
    return retval;
}