#ifndef TP1_HPP
#define TP1_HPP

#include <string>
#include <unordered_map>

namespace TP1
{

class TP1
{
private:
    static const std::unordered_map<char, char> TRIGRAPH_MAP;

public:
    TP1() noexcept;

    bool execute(const char *filename,
                 std::string &src);

private:
    void openFile();
    void replaceTrigraph();

    std::string mFilename;
    std::string &mSrc;
    bool mIsValid;
};

}

#endif