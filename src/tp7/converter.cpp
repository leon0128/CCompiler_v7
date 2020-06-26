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

FloatingConstant *TP7::Converter::convFloating(const std::string &str)
{
    FloatingConstant retval;
    std::size_t idx = 0;
    bool (*isValidValue)(const std::string&, std::size_t) = nullptr;
    bool (*isValidExponent)(const std::string&, std::size_t) = nullptr;
    bool isConsumedDot = false;
    bool isConsumedExponent = false;

    retval.radixTag = FloatingConstant::RadixTag::DECIMAL;
    
    // prefix
    if(isHexadecimalPrefix(str, idx))
    {
        idx += 2;
        retval.radixTag = FloatingConstant::RadixTag::HEXADECIMAL;
        isValidValue = &isHexadecimal;
        isValidExponent = &isBinaryExponent;
    }
    else
    {
        retval.radixTag = FloatingConstant::RadixTag::DECIMAL;
        isValidValue = &isDecimal;
        isValidExponent = &isExponent;
    }

    // integer part
    while(isValidValue(str, idx))
        retval.integer.push_back(str[idx++]);

    // fractional and decimal part
    if(isDot(str, idx))
    {
        idx++;
        isConsumedDot = true;
        while(isValidValue(str, idx))
            retval.decimal.push_back(str[idx++]);  
    }

    // exponent-part
    if(isValidExponent(str, idx))
    {
        idx++;
        isConsumedExponent = true;
        if(isPlus(str, idx))
            retval.exponentTag = FloatingConstant::ExponentTag::PLUS;
        else if(isMinus(str, idx))
            retval.exponentTag = FloatingConstant::ExponentTag::MINUS;
        
        if(retval.exponentTag != FloatingConstant::ExponentTag::NONE)
            idx++;

        while(isDecimal(str, idx))
            retval.exponent.push_back(str[idx++]);
    }

    // suffix
    if(isFloatSuffix(str, idx))
    {
        idx++;
        retval.suffixTag = FloatingConstant::SuffixTag::FLOAT;
    }
    else if(isLongSuffix(str, idx))
    {
        idx++;
        retval.suffixTag = FloatingConstant::SuffixTag::LONG;
    }

    // check invalid form
    if(idx != str.size())
        return nullptr;
    if(isConsumedDot &&
       retval.integer.empty() &&
       retval.decimal.empty())
        return nullptr;
    if(!isConsumedDot &&
       retval.integer.empty())
        return nullptr;
    if(isConsumedExponent &&
       retval.exponent.empty())
        return nullptr;
    if(!isConsumedDot &&
       !isConsumedExponent)
        return nullptr;
    if(retval.radixTag == FloatingConstant::RadixTag::HEXADECIMAL &&
       !isConsumedExponent)
        return nullptr;
    
    return new FloatingConstant(std::move(retval));
}