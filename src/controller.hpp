#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

class Controller
{
public:
    Controller() noexcept;

    bool execute(const char *filename);

private:
    bool procTP1(const char *filename) const;

    bool mIsValid;
};

#endif