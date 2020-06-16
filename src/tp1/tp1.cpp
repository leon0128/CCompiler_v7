#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>

#include "tp1.hpp"

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

bool TP1::TP1::execute(const char *filename,
                       std::string &src)
{
    TP1 tp1;
    tp1.mFilename = (filename != nullptr) ? std::string(filename) : std::string();

    tp1.openFile();

    if(tp1.mIsValid)
        tp1.replaceTrigraph();

    src = std::move(tp1.mSrc);
    return tp1.mIsValid;
}

TP1::TP1::TP1() noexcept:
    mFilename(),
    mSrc(),
    mIsValid(true)
{
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