#include "expression_simbol.hpp"
#include "static_assert_simbol.hpp"

namespace TP7
{

std::string StaticAssertDeclaration::string() const
{
    std::string ret = "_Static_assert(";
    ret += constant->string();
    ret += ", ";
    ret += str->string();
    ret += ");";
    return ret;
}

}