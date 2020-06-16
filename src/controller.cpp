#include "controller.hpp"

Controller::Controller():
    mIsValid(true)
{
}

bool Controller::execute(const char *filename)
{
    return mIsValid;
}