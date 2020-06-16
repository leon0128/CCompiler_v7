#include <string>
#include <iostream>

#include "tp1/tp1.hpp"
#include "controller.hpp"

bool Controller::execute(const char *filename)
{
    std::string str;
    if(!TP1::TP1::execute(filename, str))
        return false;

    std::cout << str << std::endl;

    return true;
}

Controller::Controller() noexcept:
    mIsValid(true)
{
}