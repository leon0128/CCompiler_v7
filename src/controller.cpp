#include "tp1/tp1.hpp"
#include "controller.hpp"

Controller::Controller() noexcept:
    mIsValid(true)
{
}

bool Controller::execute(const char *filename)
{
    if(!(mIsValid = procTP1(filename)))
        return false;

    return mIsValid;
}

bool Controller::procTP1(const char *filename) const
{
    TP1::TP1 tp1;
    return tp1.execute(filename);
}