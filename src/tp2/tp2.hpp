#ifndef TP2_TP2_HPP
#define TP2_TP2_HPP

#include <string>

namespace TP2
{

class TP2
{
public:
    static bool execute(std::string &src);

private:
    TP2() noexcept;

    void concatenate();

    std::string mSrc;
    bool mIsValid;
};

}

#endif