#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

class Controller
{
public:
    Controller() noexcept;

    bool execute(const char *filename);

private:
    bool mIsValid;
};

#endif