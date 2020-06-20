#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <string>
#include <vector>

class PPToken;

class Controller
{
public:
    // for main function
    static bool execute(const char *filename);

    // for tp4 ## opeartor
    static bool retokenize(std::string &src,
                           std::vector<class PPToken*> &dst);

    // for tp4 include directive
    static bool include(const std::string &filename,
                        std::vector<PPToken*>&);

private:
    static bool readIncludeSystemPaths();
};

#endif