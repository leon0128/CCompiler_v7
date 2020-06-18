#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <string>
#include <vector>
#include "integer.hpp"

class Controller
{
public:
    // for main function
    static bool execute(const char *filename);

    // for tp4 ## opeartor
    static bool retokenize(std::string &src,
                           std::vector<class PPToken*> &dst);
    
    // for tp4 constant-expression
    static bool evaluate(const std::vector<PPToken*>&,
                         Integer&);
};

#endif