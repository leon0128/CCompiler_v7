#ifndef TP7_CONVERTER_HPP
#define TP7_CONVERTER_HPP

#include <vector>
#include <string>

#include "../simbol.hpp"

namespace TP7
{

class Converter
{
public:
    static bool execute(std::vector<PPToken*>&,
                        std::vector<Token*>&);

private:
    static Token *convert(PPToken*);

    static Keyword *convKeyword(PPToken*);
    static Identifier *convIdentifier(PPToken*);
    static Constant *convConstant(PPToken*);
    static StringLiteral *convStringLiteral(PPToken*);
    static Punctuator *convPunctuator(PPToken*);
    
    static IntegerConstant *convInteger(const std::string&);
    static FloatingConstant *convFloating(const std::string&);

// function-definition macro
// type = bool (*)(const std::string&, std::size_t) noexcept
#define IS_MATCH_CONDITION(func_name, count, conditional_expression) \
    inline static bool func_name(const std::string &str, std::size_t idx) noexcept \
    { \
        return idx + (count - 1) < str.size() \
            ? (conditional_expression) \
                : false; \
    }

    // helper for constant
    // prefix
    IS_MATCH_CONDITION(isDecimalPrefix, 1,
                       (str[idx] >= '1' && str[idx] <= '9'))
    IS_MATCH_CONDITION(isOctalPrefix, 1,
                       (str[idx] == '0'))
    IS_MATCH_CONDITION(isHexadecimalPrefix, 2,
                       (str[idx] == '0' &&
                        (str[idx + 1] == 'x' || str[idx + 1] == 'X')))
    // value
    IS_MATCH_CONDITION(isDecimal, 1,
                       (str[idx] >= '0' && str[idx] <= '9'))
    IS_MATCH_CONDITION(isOctal, 1,
                        (str[idx] >= '0' && str[idx] < '7'))
    IS_MATCH_CONDITION(isHexadecimal, 1,
                       ((str[idx] >= '0' && str[idx] <= '9') ||
                        (str[idx] >= 'a' && str[idx] <= 'f') ||
                        (str[idx] >= 'A' && str[idx] <= 'F')))
    // suffix
    IS_MATCH_CONDITION(isUnsignedSuffix, 1,
                       (str[idx] == 'u' || str[idx] == 'U'))
    IS_MATCH_CONDITION(isLongSuffix, 1,
                       (str[idx] == 'l' || str[idx] == 'L'))
    IS_MATCH_CONDITION(isLongLongSuffix, 2,
                       ((str[idx] == 'l' && str[idx + 1] == 'l') &&
                        (str[idx] == 'L' && str[idx + 1] == 'L')))
    IS_MATCH_CONDITION(isFloatSuffix, 1,
                       (str[idx] == 'f' || str[idx] == 'F'))
    // exponent
    IS_MATCH_CONDITION(isExponent, 1,
                       (str[idx] == 'e' || str[idx] == 'E'))
    IS_MATCH_CONDITION(isBinaryExponent, 1,
                       (str[idx] == 'p' || str[idx] == 'P'))
    // other
    IS_MATCH_CONDITION(isDot, 1,
                       (str[idx] == '.'))
    IS_MATCH_CONDITION(isPlus, 1,
                       (str[idx] == '+'))
    IS_MATCH_CONDITION(isMinus, 1,
                       (str[idx] == '-'))
#undef IS_MATCH_CONDITION
};

}

#endif