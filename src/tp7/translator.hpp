#ifndef TP7_TRANSLATOR_HPP
#define TP7_TRANSLATOR_HPP

#include <vector>
#include <string>
#include <sstream>

#include "../simbol.hpp"

namespace TP7
{

class Scope;

class Translator
{
public:
    static bool execute(const std::vector<Token*>&,
                        std::string&);

private:
    Translator(const std::vector<Token*>&) noexcept;
    ~Translator() noexcept;

    void translate();

    bool procTranslationUnit();
    bool procExternalDeclaration();
    bool procFunctionDefinition(){return false;}
    bool procDeclaration();

    bool isMatch(std::size_t idx, Punctuator::Tag tag) const noexcept;
    bool isMatch(std::size_t idx, Keyword::Tag tag) const noexcept;
    bool isIdentifier(std::size_t idx) const noexcept;

    const std::vector<Token*> &mTvec;
    std::stringstream mSStr;
    std::size_t mIdx;
    Scope *mScope;
    bool mIsValid;
};

}

#endif