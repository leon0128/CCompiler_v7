#include <iostream>

#include "../global.hpp"
#include "tp2.hpp"

bool TP2::TP2::execute(std::string &src)
{
    TP2 tp2(src);

    tp2.concatenate();
    if(!tp2.mIsValid)
        return false;
    
    return true;
}

TP2::TP2::TP2(std::string &src) noexcept:
    mSrc(src),
    mIsValid(true)
{
}

void TP2::TP2::concatenate()
{
    for(std::string::size_type pos = mSrc.find("\\\n");
        pos != std::string::npos;
        pos = mSrc.find("\\\n", pos))
    {
        mSrc.replace(pos, 2, "");
    }

    if(mSrc.empty() ? false : (mSrc.back() != '\n'))
    {
        mIsValid = false;
        std::cout << "TP2 error:\n"
                     "    what: source that is not empty shall end in a new-line character.\n"
                     "    filename: " << Global::CURRENT_FILENAME
                  << std::endl;           
    }
}