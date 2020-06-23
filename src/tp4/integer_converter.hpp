#ifndef TP4_INTEGER_CONVERTER
#define TP4_INTEGER_CONVERTER

class PPToken;

namespace TP4
{

class Integer;

class IntegerConverter
{
public:
    static bool convert(PPToken*, Integer&);

private:
    // convert from integer-constant
    static bool convIntegerConstant(const std::string&,
                                    Integer&) noexcept;
    // convert int from char
    static bool convertHexadecimal(char, int&) noexcept;
    static bool convertOctal(char, int&) noexcept;
    static bool convertDecimal(char, int&) noexcept;
    static bool isUnsignedSuffix(const std::string&,
                                 std::size_t idx) noexcept;
    static bool isLongSuffix(const std::string&,
                             std::size_t idx) noexcept;
    static bool isLongLongSuffix(const std::string&,
                                 std::size_t idx) noexcept;

    // convert from character-constant
    static bool convCharacterConstant(const std::string&,
                                      Integer&) noexcept;
};

}

#endif