#include <string>
#include <vector>
#include <iostream>

#include "tp1/tp1.hpp"
#include "tp2/tp2.hpp"
#include "tp3/tp3.hpp"
#include "controller.hpp"

std::string Controller::CURRENT_FILENAME;

bool Controller::execute(const char *filename)
{
    std::string src;
    if(!TP1::TP1::execute(filename, src))
        return false;

    if(!TP2::TP2::execute(src))
        return false;

    std::vector<PPToken*> ptvec;
    if(!TP3::TP3::execute(src, ptvec))
        return false;

    std::cout << src << std::endl;

    return true;
}

Controller::Controller() noexcept:
    mIsValid(true)
{
}