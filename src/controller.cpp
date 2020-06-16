#include <string>

#include "tp1/tp1.hpp"
#include "controller.hpp"

Controller::Controller() noexcept:
    mIsValid(true)
{
}

bool Controller::execute(const char *filename)
{
    TP1::TP1 tp1;
    std::string src;
    if(!(mIsValid = tp1.execute(filename, src)))
        return false;

    return mIsValid;
}