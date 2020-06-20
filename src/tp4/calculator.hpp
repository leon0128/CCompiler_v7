#ifndef TP4_Calculator_HPP
#define TP4_Calculator_HPP

#include <vector>

#include "integer.hpp"

class PPToken;

namespace TP4
{

class Calculator
{
public:
    static bool execute(std::vector<PPToken*>&,
                        Integer&);
};

}

#endif