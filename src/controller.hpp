#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <string>
#include <vector>

class Controller
{
public:
    static bool execute(const char *filename);

    static bool retokenize(std::string &src,
                           std::vector<class PPToken*> &dst);
};

#endif