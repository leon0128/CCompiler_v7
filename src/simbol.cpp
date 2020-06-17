#include <string>
#include <iostream>

#include "simbol.hpp"

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

bool BaseSimbol::unexceptTag(const char *className) noexcept
{
    std::cerr << "implementation error:\n"
                 "    class-name: " << className
              << std::endl;

    return false;
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
            unexceptTag("HeaderName");
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
            unexceptTag("PPToken");
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
        unexceptTag("Punctuator");
    
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