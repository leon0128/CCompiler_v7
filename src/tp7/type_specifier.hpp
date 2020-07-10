#ifndef TP7_TYPE_SPECIFIER_HPP
#define TP7_TYPE_SPECIFIER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "../simbol.hpp"

namespace TP7
{

struct TypeAttribute
{
    int size, align;
};

class TypeSpecifier
{
public:
    enum class Tag;

    static bool initialize();

    static Tag convert(std::vector<Keyword::Tag>&);
    static bool isTypeSpecifier(Keyword::Tag) noexcept;
    static bool isIntegerType(Tag) noexcept;
    static bool isFloatingType(Tag) noexcept;

    static const std::unordered_map<Tag, TypeAttribute> &getAttributeMap() noexcept
        {return TYPE_ATTRIBUTE_MAP;}

private:
    static void sort();
    static bool read();

    static const std::unordered_set<Keyword::Tag> TYPE_SPECIFIER_SET;
    static const std::unordered_map<std::string, Tag> TYPE_NAME_MAP;
    static const std::string NAME_KEY;
    static const std::string SIZE_KEY;
    static const std::string ALIGN_KEY;

    static std::unordered_map<Tag, TypeAttribute> TYPE_ATTRIBUTE_MAP;
    static std::unordered_map<Tag, std::vector<std::vector<Keyword::Tag>>> TYPE_SPECIFIER_MAP;
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