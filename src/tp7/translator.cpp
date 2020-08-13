#include <iostream>

#include "static_assert_simbol.hpp"
#include "type.hpp"
#include "scope.hpp"
#include "tokenizer.hpp"
#include "translator.hpp"

bool TP7::Translator::execute(const std::vector<Token*> &tvec,
                              std::string &dst)
{
    Translator translator(tvec);
    translator.translate();

    if(translator.mIsValid)
        dst = translator.mSStr.str();

    return translator.mIsValid;
}

TP7::Translator::Translator(const std::vector<Token*> &tvec) noexcept:
    mTvec(tvec),
    mSStr(),
    mIdx(0),
    mIsValid(true)
{
}

void TP7::Translator::translate()
{
    mSStr << "    .intel_syntax noprefix\n\n"
          << std::flush;

    mIsValid = procTranslationUnit();

    if(!mIsValid)
    {
        std::cout << "TP7 Translator error:\n"
                     "     idx: " << mIdx
                  << std::endl;
    }
}

bool TP7::Translator::procTranslationUnit()
{
    while(mIdx < mTvec.size())
    {
        if(!procExternalDeclaration())
            return false;
    }
    
    return true;
}

bool TP7::Translator::procExternalDeclaration()
{
    if(procFunctionDefinition())
        return true;
    else if(procDeclaration())
        return true;
    else
        return false;
}

bool TP7::Translator::procDeclaration()
{
    return true;
}

bool TP7::Translator::procStaticAssertDeclaration()
{


    return true;
}

bool TP7::Translator::isMatch(std::size_t idx, Punctuator::Tag tag) const noexcept
{
    return idx < mTvec.size()
        ? mTvec[idx]->tag == Token::Tag::PUNCTUATOR
            ? mTvec[idx]->uni.punctuator->tag == tag
                : false
                    : false;
}

bool TP7::Translator::isMatch(std::size_t idx, Keyword::Tag tag) const noexcept
{
    return idx < mTvec.size()
        ? mTvec[idx]->tag == Token::Tag::KEYWORD
            ? mTvec[idx]->uni.keyword->tag == tag
                : false
                    : false;
}

bool TP7::Translator::isMatch(std::size_t idx, Token::Tag tag) const noexcept
{
    return idx < mTvec.size()
        ? mTvec[idx]->tag == tag
            : false;
}