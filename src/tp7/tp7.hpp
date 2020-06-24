#ifndef TP7_TP7_HPP
#define TP7_TP7_HPP

#include <vector>
#include <string>

class PPToken;

namespace TP7
{

class TP7
{
public:
    static bool execute(std::vector<PPToken*>&,
                        std::string&);
};

}

#endif