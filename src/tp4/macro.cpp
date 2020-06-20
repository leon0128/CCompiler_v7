#include "../simbol.hpp"
#include "macro.hpp"

bool operator ==(const TP4::Macro &lhs, const TP4::Macro &rhs)
{
    if(lhs.isFunction != rhs.isFunction)
        return false;
    
    // object-like
    if(!lhs.isFunction)
    {
        if(lhs.ident != rhs.ident)
            return false;
        
        std::string lhsstr, rhsstr;
        for(auto &&pt : lhs.repVec)
            lhsstr += pt->string();
        for(auto &&pt : rhs.repVec)
            rhsstr += pt->string();
        
        return lhsstr == rhsstr;
    }
    // function-like
    else
    {
        if(lhs.isVariable != rhs.isVariable)
            return false;
        
        if(lhs.paramMap != rhs.paramMap)
            return false;
        
        std::string lhsstr, rhsstr;
        for(auto &&pt : lhs.repVec)
            lhsstr += pt->string();
        for(auto &&pt : rhs.repVec)
            rhsstr += pt->string();
        
        return lhsstr == rhsstr;
    }
}

bool operator !=(const TP4::Macro &lhs, const TP4::Macro &rhs)
{
    return !(lhs == rhs);
}