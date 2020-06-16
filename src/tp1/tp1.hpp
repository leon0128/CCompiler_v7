#ifndef TP1_HPP
#define TP1_HPP

namespace TP1
{

class TP1
{
public:
    TP1() noexcept;

    bool execute(const char *filename);

private:
    bool mIsValid;
};

}

#endif