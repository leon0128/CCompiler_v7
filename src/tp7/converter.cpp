#include <iostream>

#include "../simbol.hpp"
#include "converter.hpp"

bool TP7::Converter::execute(std::vector<PPToken*> &src,
                             std::vector<Token*> &dst)
{
    bool isValid = true;
    for(std::size_t i = 0; i < src.size(); i++)
    {
        Token *token = convert(src[i]);
        if(token != nullptr)
            dst.push_back(token);
        else
        {
            std::cout << "TP7 Converter error:\n"
                         "    what: failed to convert to token from pp-token.\n"
                         "    content: " << src[i]->string()
                      << std::endl;
            isValid = false;
            break;
        }
    }

    return isValid;
}

Token *TP7::Converter::convert(PPToken *ppToken)
{
    Token retval;

    if((retval.uni.punctuator = convPunctuator(ppToken)) != nullptr)
        retval.tag = Token::Tag::PUNCTUATOR;
    else if((retval.uni.stringLiteral = convStringLiteral(ppToken)) != nullptr)
        retval.tag = Token::Tag::STRING_LITERAL;
    else if((retval.uni.keyword = convKeyword(ppToken)) != nullptr)
        retval.tag = Token::Tag::KEYWORD;
    else if((retval.uni.identifier = convIdentifier(ppToken)) != nullptr)
        retval.tag = Token::Tag::IDENTIFIER;
    else if((retval.uni.constant = convConstant(ppToken)) != nullptr)
        retval.tag = Token::Tag::CONSTANT;

    return retval.tag != Token::Tag::NONE
        ? new Token(retval)
            : nullptr;
}

Punctuator *TP7::Converter::convPunctuator(PPToken *ppToken)
{
    return ppToken->tag == PPToken::Tag::PUNCTUATOR
        ? ppToken->uni.punctuator
            : nullptr;
}

StringLiteral *TP7::Converter::convStringLiteral(PPToken *ppToken)
{
    return ppToken->tag == PPToken::Tag::STRING_LITERAL
        ? ppToken->uni.stringLiteral
            : nullptr;
}

Keyword *TP7::Converter::convKeyword(PPToken *ppToken)
{
    Keyword retval;

    if(ppToken->tag == PPToken::Tag::IDENTIFIER)
    {
        auto iter = Keyword::KEYWORD_MAP.find(ppToken->uni.identifier->str);
        if(iter != Keyword::KEYWORD_MAP.end())
            retval.tag = iter->second;
    }

    return retval.tag != Keyword::Tag::NONE
        ? new Keyword(retval)
            : nullptr;
}

Identifier *TP7::Converter::convIdentifier(PPToken *ppToken)
{
    return ppToken->tag == PPToken::Tag::IDENTIFIER
        ? ppToken->uni.identifier
            : nullptr;
}

Constant *TP7::Converter::convConstant(PPToken *ppToken)
{
    Constant retval;

    if(ppToken->tag == PPToken::Tag::CHARACTER_CONSTANT)
    {
        retval.tag = Constant::Tag::CHARACTER;
        retval.uni.character = ppToken->uni.characterConstant;
    }
    else if(ppToken->tag == PPToken::Tag::PP_NUMBER)
    {
        if((retval.uni.floating = convFloating(ppToken->uni.ppNumber->str)) != nullptr)
            retval.tag = Constant::Tag::FLOATING;
        else if((retval.uni.integer = convInteger(ppToken->uni.ppNumber->str)) != nullptr)
            retval.tag = Constant::Tag::INTEGER;
    }

    return retval.tag != Constant::Tag::NONE
        ? new Constant(retval)
            : nullptr;
}

IntegerConstant *TP7::Converter::convInteger(const std::string &str)
{
    IntegerConstant retval;
    std::size_t idx = 0;
    bool (*isValid)(const std::string&, std::size_t) noexcept = nullptr;

    // prefix
    if(isDecimalPrefix(str, idx))
    {
        retval.prefixTag = IntegerConstant::PrefixTag::DECIMAL;
        isValid = &isDecimal;
    }
    else if(isHexadecimalPrefix(str, idx))
    {
        idx += 2;
        retval.prefixTag = IntegerConstant::PrefixTag::HEXADECIMAL;
        isValid = &isHexadecimal;
    }
    else if(isOctalPrefix(str, idx))
    {
        idx++;
        retval.prefixTag = IntegerConstant::PrefixTag::OCTAL;
        isValid = &isOctal;
    }
    // check invalid form
    if(retval.prefixTag == IntegerConstant::PrefixTag::NONE)
        return nullptr;

    // value
    while(isValid(str, idx))
        retval.str.push_back(str[idx++]);
    // check invalid form
    if(retval.prefixTag != IntegerConstant::PrefixTag::OCTAL &&
       retval.str.empty())
        return nullptr;
    
    // suffix
    if(isUnsignedSuffix(str, idx))
    {
        idx++;
        if(isLongLongSuffix(str, idx))
        {
            idx += 2;
            retval.suffixTag = IntegerConstant::SuffixTag::UNSIGNED_LONG_LONG;
        }
        else if(isLongSuffix(str, idx))
        {
            idx++;
            retval.suffixTag = IntegerConstant::SuffixTag::UNSIGNED_LONG;
        }
        else
            retval.suffixTag = IntegerConstant::SuffixTag::UNSIGNED;
    }
    else if(isLongLongSuffix(str, idx))
    {
        idx += 2;
        if(isUnsignedSuffix(str, idx))
        {
            idx++;
            retval.suffixTag = IntegerConstant::SuffixTag::UNSIGNED_LONG_LONG;
        }
        else
            retval.suffixTag = IntegerConstant::SuffixTag::LONG_LONG;
    }
    else if(isLongSuffix(str, idx))
    {
        idx++;
        if(isUnsignedSuffix(str, idx))
        {
            idx++;
            retval.suffixTag = IntegerConstant::SuffixTag::UNSIGNED_LONG;
        }
        else
            retval.suffixTag = IntegerConstant::SuffixTag::LONG;
    }

    return idx == str.size()
        ? new IntegerConstant(std::move(retval))
            : nullptr;
}