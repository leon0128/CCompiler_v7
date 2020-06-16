#include <fstream>
#include <sstream>
#include <iostream>

#include "tp1.hpp"

std::string mStrInit;

const std::unordered_map<char, char> TP1::TP1::TRIGRAPH_MAP
    = {{'=', '#'},
       {'(', '['},
       {'/', '\\'},
       {')', ']'},
       {'\'', '^'},
       {'<', '{'},
       {'!', '|'},
       {'>', '}'},
       {'-', '~'}};

TP1::TP1::TP1() noexcept:
    mFilename(),
    mSrc(::mStrInit),
    mIsValid(true)
{
}

bool TP1::TP1::execute(const char *filename,
                       std::string &src)
{
    mSrc = src;
    mFilename = (filename != nullptr) ? std::string(filename) : std::string();

    openFile();
    if(mIsValid)
        replaceTrigraph();

    mSrc = ::mStrInit;
    return mIsValid;
}

void TP1::TP1::openFile()
{
    std::ifstream fstr(mFilename);

    if(fstr.is_open())
    {
        std::stringstream sstr;
        sstr << fstr.rdbuf();
        fstr.close();
        mSrc = sstr.str();
    }
    else
    {
        mIsValid = false;
        std::cout << "TP1 error:\n"
                     "    what: failed to open file.\n"
                     "    filename: " << mFilename
                  << std::endl;       
    }
}

void TP1::TP1::replaceTrigraph()
{
    for(std::size_t pos = mSrc.find("\?\?");
        pos + 2 < mSrc.size() && pos != std::string::npos;
        pos = mSrc.find("\?\?", pos + 1))
    {
        auto iter = TRIGRAPH_MAP.find(mSrc[pos + 2]);
        if(iter != TRIGRAPH_MAP.end())
            mSrc.replace(pos, 3, 1, iter->second);
    }
}