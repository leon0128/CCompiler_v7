#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

class Controller
{
public:
    static bool execute(const char *filename);

private:
    Controller() noexcept;

    bool mIsValid;
};

#endif