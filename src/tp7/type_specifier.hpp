#ifndef TP7_TYPE_SPECIFIER_HPP
#define TP7_TYPE_SPECIFIER_HPP

#include <vector>
#include <unordered_map>

#include "../simbol.hpp"

namespace TP7
{

class TypeSpecifier
{
public:
    enum class Tag;

    static Tag convert(const std::vector<Keyword::Tag>&);
    static bool isTypeSpecifier(Keyword::Tag) noexcept;

private:
    static const std::unordered_map<Tag, std::vector<std::vector<Keyword::Tag>>> TYPE_SPECIFIER_MAP;
};

enum class TypeSpecifier::Tag
{
    NONE,
    VOID,
    CHAR,
    S_CHAR,
    U_CHAR,
    S_SHORT,
    U_SHORT,
    S_INT,
    U_INT,
    S_LONG,
    U_LONG,
    S_LONG_LONG,
    U_LONG_LONG,
    FLOAT,
    DOUBLE,
    LONG_DOUBLE,
    BOOL
};

}

#endif