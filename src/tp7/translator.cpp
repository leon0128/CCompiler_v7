#include <iostream>

#include "type_specifier.hpp"
#include "scope.hpp"
#include "arithmetic_type.hpp"
#include "tokenizer.hpp"
#include "translator.hpp"

bool TP7::Translator::execute(const std::vector<Token*> &tvec,
                              std::string &dst)
{
    if(!TypeSpecifier::initialize())
        return false;

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
    auto befidx = mIdx;

    // declaration-specifiers
    std::vector<Keyword::Tag> typeSpecifierVec;
    while(isMatch(mIdx, Token::Tag::KEYWORD)
        ? TypeSpecifier::isTypeSpecifier(mTvec[mIdx]->uni.keyword->tag)
            : false)
        typeSpecifierVec.push_back(mTvec[mIdx++]->uni.keyword->tag);

    // get TypeSpecifier::Tag
    TypeSpecifier::Tag typeSpecifierTag = TypeSpecifier::convert(typeSpecifierVec);
    if(typeSpecifierTag == TypeSpecifier::Tag::NONE)
    {
        mIsValid = false;
        std::cout << "TP7 Translator error:\n"
                     "    what: failed to convert to type from type-specifier.\n"
                     "    idx: " << mIdx
                  << std::endl;
        mIdx = befidx;
        return false;
    }

    // declarator
    Identifier *identifier
        = isMatch(mIdx, Token::Tag::IDENTIFIER)
            ? mTvec[mIdx++]->uni.identifier
                : nullptr;
    if(identifier == nullptr)
    {
        mIsValid = false;
        std::cout << "TP7 Translator error:\n"
                     "    what: failed to get identifier.\n"
                     "    idx: " << mIdx
                  << std::endl;
        mIdx = befidx;
        return false;
    }
    if(!Scope::emplace(identifier->str))
    {
        mIsValid = false;
        std::cout << "TP7 Translator error:\n"
            "    what: identifier is redefined.\n"
            "    ident: " << identifier->str
            << std::endl;
        mIdx = befidx;
        return false;
    }

    // semicolon
    if(isMatch(mIdx, Punctuator::Tag::SEMI_COL))
        mIdx++;
    else
    {
        mIsValid = false;
        std::cout << "TP7 Translator error:\n"
            "    what: semicolon not exists.\n"
            "    idx: " << mIdx
            << std::endl;
        mIdx = befidx;
        return false;
    }

    // write
    mSStr << "    .comm " 
        << identifier->str << ", "
        << TypeSpecifier::getAttributeMap().at(typeSpecifierTag).size << ", "
        << TypeSpecifier::getAttributeMap().at(typeSpecifierTag).align
        << std::endl;

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