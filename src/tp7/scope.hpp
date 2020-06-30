#ifndef TP7_SCOPE_HPP
#define TP7_SCOPE_HPP

#include <unordered_set>
#include <utility>
#include <string>

namespace TP7
{

class Scope
{
public:
    template<class String>
    static bool emplace(String &&ident)
        {return IDENTIFIER_SET.emplace(std::forward<String>(ident)).second;}

private:
    static std::unordered_set<std::string> IDENTIFIER_SET;
};

}

#endif