#ifndef TP1_HPP
#define TP1_HPP

#include <string>
#include <unordered_map>

namespace TP1
{

class TP1
{
public:
    static bool execute(const char *filename,
                        std::string &str);

private:
    static const std::unordered_map<char, char> TRIGRAPH_MAP;

    TP1() noexcept;

    void openFile();
    void replaceTrigraph();

    std::string mFilename;
    std::string mSrc;
    bool mIsValid;
};

}

#endif