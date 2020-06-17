#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <string>

class Controller
{
public:
    static bool execute(const char *filename);

private:
    Controller() noexcept;
    
    bool mIsValid;
};

#endif