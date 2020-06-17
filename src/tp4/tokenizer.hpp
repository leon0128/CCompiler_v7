#ifndef TP4_TOKENIZER_HPP
#define TP4_TOKENIZER_HPP

#include <vector>

class PPFile;
class PPToken;

namespace TP4
{

class Tokenizer
{
public:
    static bool execute(const std::vector<PPToken*>&,
                        PPFile*&) noexcept;

private:
    const std::vector<PPToken*> &mPtvec;
    PPFile *&mPPFile;
    std::size_t mIdx;

    bool isValid;
};

}

#endif