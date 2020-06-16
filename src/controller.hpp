#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

class Controller
{
public:
    Controller();

    bool execute(const char *filename);

private:
    bool mIsValid;
};

#endif