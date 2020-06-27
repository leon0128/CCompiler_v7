#include <iostream>

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
    bool isValid = false;
    auto befidx = mIdx;

    // declaration-specifiers
    if(isMatch(mIdx, Keyword::Tag::INT))
    {
        mIdx++;
        if(isIdentifier(mIdx))
        {
            mIdx++;
            
            mSStr << "    .global " << mTvec[mIdx - 1]->uni.identifier->str << "\n"
                     "    .data\n"
                     "    .align 4\n"
                     "    .type " << mTvec[mIdx - 1]->uni.identifier->str << ", @object" << "\n"
                     "    .size " << mTvec[mIdx - 1]->uni.identifier->str << ", 4\n"
                  << mTvec[mIdx - 1]->uni.identifier->str << ":\n"
                     "    .long 0\n"
                  << std::endl;

            if(isMatch(mIdx, Punctuator::Tag::SEMI_COL))
            {
                mIdx++;
                isValid = true;
            }
        }
    }

    if(isValid)
        return true;
    else
    {
        mIdx = befidx;
        return false;
    }
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

bool TP7::Translator::isIdentifier(std::size_t idx) const noexcept
{
    return idx < mTvec.size()
        ? mTvec[idx]->tag == Token::Tag::IDENTIFIER
            : false;
}