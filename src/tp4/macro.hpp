#ifndef TP4_MACRO_HPP
#define TP4_MACRO_HPP

#include <unordered_map>
#include <vector>
#include <string>

class PPToken;

namespace TP4
{

class Macro
{
public:
    Macro() noexcept:
        ident(),
        paramMap(),
        repVec(),
        isFunction(false),
        isVariable(false){}

    std::string ident;
    std::unordered_map<std::string, std::size_t> paramMap;
    std::vector<PPToken*> repVec;

    bool isFunction;
    bool isVariable;
};

bool operator ==(const Macro&, const Macro&);
bool operator !=(const Macro&, const Macro&);

}

#endif