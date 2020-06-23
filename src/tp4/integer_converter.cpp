#include <iostream>
#include <string>

#include "../global.hpp"
#include "../simbol.hpp"
#include "integer.hpp"
#include "integer_converter.hpp"

bool TP4::IntegerConverter::convert(PPToken *ppToken, Integer &result)
{
    bool isValid = true;

    switch(ppToken->tag)
    {
        case(PPToken::Tag::PP_NUMBER):
        {
            isValid = convIntegerConstant(ppToken->uni.ppNumber->str, result);
            break;
        }
        case(PPToken::Tag::CHARACTER_CONSTANT):
        {
            isValid = convCharacterConstant(ppToken->uni.characterConstant->str, result);
            break;
        }

        default:
            isValid = false;
            std::cout << "TP4 IntegerConverter error:\n"
                         "    what: cannot convert to integer othrer than integer-constant and character-constant.\n"
                         "    filename: " << Global::CURRENT_FILENAME << "\n"
                         "    content: " << ppToken->string()
                      << std::endl;
    }

    return isValid;
}

bool TP4::IntegerConverter::convIntegerConstant(const std::string &str,
                                                Integer &result) noexcept
{
    bool isValid = true;
    std::size_t idx = 0;
    int mag = 0;
    bool (*convFunc)(char, int&) noexcept = nullptr;

    // decimal
    if(idx < str.size() ? str[idx] >= '1' && str[idx] <= '9' : false)
    {
        convFunc = &IntegerConverter::convertDecimal;
        mag = 10;
    }
    // hexadecimal
    else if(idx + 2 < str.size() ? str[idx] == '0' && (str[idx + 1] == 'x' || str[idx + 1] == 'X') && convertHexadecimal(str[idx + 2], mag) : false)
    {
        idx += 2;
        convFunc = &IntegerConverter::convertHexadecimal;
        mag = 16;
    }
    // octal
    else if(idx < str.size() ? str[idx] == '0' : false)
    {
        convFunc = &IntegerConverter::convertOctal;
        mag = 8;
    }
    else
        isValid = false;

    // convert
    if(isValid)
    {
        // value
        std::uintmax_t val = 0;
        int tmp = 0;
        while(idx < str.size() ? convFunc(str[idx], tmp) : false)
        {
            idx++;
            val += tmp * mag;
        }
        
        // suffix
        Integer::Tag tag = Integer::Tag::SIGNED;
        if(isUnsignedSuffix(str, idx))
        {
            if(isLongLongSuffix(str, idx + 1))
                idx += 3;
            else if(isLongSuffix(str, idx + 1))
                idx += 2;
            else
                idx += 1;
            
            tag = Integer::Tag::UNSIGNED;

        }
        else if(isLongLongSuffix(str, idx))
        {
            if(isUnsignedSuffix(str, idx + 2))
            {
                tag = Integer::Tag::UNSIGNED;
                idx += 3;    
            }
            else
                idx += 2;
        }
        else if(isLongSuffix(str, idx))
        {
            if(isUnsignedSuffix(str, idx + 1))
            {
                tag = Integer::Tag::UNSIGNED;
                idx += 2;
            }
            else
                idx += 1;
        }

        // assignment
        result.tag = tag;
        tag == Integer::Tag::SIGNED
            ? result.uni.i = static_cast<std::intmax_t>(val)
                : result.uni.u = val;
    }

    // is valid
    if(idx != str.size() || !isValid)
    {
        isValid = false;
        std::cout << "TP4 IntegerConverter error:\n"
                     "    what: failed to convert to integer-constant from pp-token.\n"
                     "    filename: " << Global::CURRENT_FILENAME << "\n"
                     "    content: " << str
                  << std::endl;
    }

    return isValid;
}

bool TP4::IntegerConverter::convertHexadecimal(char c, int &res) noexcept
{
    res = 0;
    if(c >= '0' && c <= '9')
    {
        res = c - '0';
        return true;
    }
    else if(c >= 'a' && c <= 'f')
    {
        res = c - 'a' + 10;
        return true;
    }
    else if(c >= 'A' && c <= 'F')
    {
        res = c - 'A' + 10;
        return true;
    }
    else
        return false;
}

bool TP4::IntegerConverter::convertOctal(char c, int &res) noexcept
{
    res = 0;
    if(c >= '0' && c <= '7')
    {
        res = c - '0';
        return true;
    }
    else
        return false;
}

bool TP4::IntegerConverter::convertDecimal(char c, int &res) noexcept
{
    res = 0;
    if(c >= '0' && c <= '9')
    {
        res = c - '0';
        return true;
    }
    else
        return false;
}

bool TP4::IntegerConverter::isUnsignedSuffix(const std::string &str,
                                             std::size_t idx) noexcept
{
    return idx < str.size() ? str[idx] == 'u' || str[idx] == 'U' : false;
}

bool TP4::IntegerConverter::isLongSuffix(const std::string &str,
                                         std::size_t idx) noexcept
{
    return idx < str.size() ? str[idx] == 'l' || str[idx] == 'L' : false;
}

bool TP4::IntegerConverter::isLongLongSuffix(const std::string &str,
                                             std::size_t idx) noexcept
{
    return idx + 1 < str.size()
        ? (str[idx] == 'l' && str[idx + 1] == 'l') ||
          (str[idx] == 'L' && str[idx + 1] == 'L')
            : false;
}

bool TP4::IntegerConverter::convCharacterConstant(const std::string &str,
                                                  Integer &result) noexcept
{
    result.tag = Integer::Tag::UNSIGNED;
    for(const char &c : str)
        result.uni.u = result.uni.u * 256 + c;
    
    return true;
}