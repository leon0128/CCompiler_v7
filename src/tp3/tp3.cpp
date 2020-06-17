#include <iostream>
#include <utility>
#include <climits>

#include "../global.hpp"
#include "../simbol.hpp"
#include "tp3.hpp"

bool TP3::TP3::execute(std::string &src,
                       std::vector<PPToken*>& ptvec)
{
    TP3 tp3(src, ptvec);

    tp3.decompose();

    return tp3.mIsValid;
}

TP3::TP3::TP3(std::string &src,
              std::vector<PPToken*> &ptvec) noexcept:
    mSrc(src),
    mIdx(0),
    mPtvec(ptvec),
    mIsValid(true)
{
}

void TP3::TP3::decompose()
{
    bool isGlued = true;
    while(mIdx < mSrc.size() && mIsValid)
    {
        if(mSrc[mIdx] == ' ' ||
           mSrc[mIdx] == '\f' ||
           mSrc[mIdx] == '\r' ||
           mSrc[mIdx] == '\t' ||
           mSrc[mIdx] == '\v')
        {
            isGlued = false;
            mIdx++;
        }
        else if(procComment())
            isGlued = false;
        else
        {
            mPtvec.push_back(etrPPToken());
            mPtvec.back()->isGlued = isGlued;
            isGlued = true;
        }
    }
}

CharacterConstant *TP3::TP3::etrCharacterConstant()
{
    if(mSrc[mIdx] != '\'')
        return nullptr;
    else
        mIdx++;

    std::string str;
    for(; mIdx < mSrc.size(); mIdx++)
    {
        if(mSrc[mIdx] != '\'' &&
           mSrc[mIdx] != '\\' &&
           mSrc[mIdx] != '\n')
            str.push_back(mSrc[mIdx]);
        else if(mSrc[mIdx] == '\\')
        {
            mIdx++;
            auto pair = procEscapeSequence();
            if(pair.first)
                str.push_back(pair.second);
            else
            {
                mIdx--;
                break;
            }
        }
        else
            break;
    }

    if((mIdx < mSrc.size()) ? (mSrc[mIdx] != '\'') : true)
    {
        mIsValid = false;
        std::cout << "TP3 error:\n"
                     "    what: character-constant is not ended.\n"
                     "    filename: " << Global::CURRENT_FILENAME
                  << std::endl;
        return nullptr;
    }
    else
        mIdx++;

    if(str.empty())
    {
        mIsValid = false;
        std::cout << "TP3 error:\n"
                     "    what: character-constant is empty.\n"
                     "    filename: " << Global::CURRENT_FILENAME
                  << std::endl;
        return nullptr;
    }

    CharacterConstant *retval = new CharacterConstant();
    retval->str = std::move(str);
    return retval;
}

Identifier *TP3::TP3::etrIdentifier()
{
    if((mSrc[mIdx] >= 'a' && mSrc[mIdx] <= 'z') ||
       (mSrc[mIdx] >= 'A' && mSrc[mIdx] <= 'Z') ||
       mSrc[mIdx] == '_');
    else
        return nullptr;

    std::string str;
    for(; mIdx < mSrc.size(); mIdx++)
    {
        if((mSrc[mIdx] >= 'a' && mSrc[mIdx] <= 'z') ||
           (mSrc[mIdx] >= 'A' && mSrc[mIdx] <= 'Z') ||
           (mSrc[mIdx] >= '0' && mSrc[mIdx] <= '9') ||
           mSrc[mIdx] == '_')
            str.push_back(mSrc[mIdx]);
        else
            break;
    }

    Identifier *retval = new Identifier();
    retval->str = std::move(str);
    return retval;
}

PPNumber *TP3::TP3::etrPPNumber()
{
    if(mSrc[mIdx] >= '0' && mSrc[mIdx] <= '9');
    else if((mIdx + 1 < mSrc.size()) ? (mSrc[mIdx] == '.' && (mSrc[mIdx + 1] >= '0' && mSrc[mIdx + 1] <= '9')) : false);
    else
        return nullptr;
    
    std::string str;
    for(; mIdx < mSrc.size(); mIdx++)
    {
        if(mSrc[mIdx] >= '0' && mSrc[mIdx] <= '9')
            str.push_back(mSrc[mIdx]);
        else if(mSrc[mIdx] == '.')
            str.push_back(mSrc[mIdx]);
        else if((mSrc[mIdx] == 'e' ||
                mSrc[mIdx] == 'E' ||
                mSrc[mIdx] == 'p' ||
                mSrc[mIdx] == 'P') &&
                (mIdx + 1 < mSrc.size() ? (mSrc[mIdx + 1] == '+' || mSrc[mIdx + 1] == '-') : false))
        {
            str.push_back(mSrc[mIdx]);
            str.push_back(mSrc[mIdx + 1]);
            mIdx++;
        }
        else if((mSrc[mIdx] >= 'a' && mSrc[mIdx] <= 'z') ||
                (mSrc[mIdx] >= 'A' && mSrc[mIdx] <= 'Z'))
            str.push_back(mSrc[mIdx]);
        else
            break;
    }

    PPNumber *retval = new PPNumber();
    retval->str = std::move(str);
    return retval;
}

PPToken *TP3::TP3::etrPPToken()
{
    PPToken retval;

    if((retval.uni.identifier = etrIdentifier()) != nullptr)
        retval.tag = PPToken::Tag::IDENTIFIER;
    else if((retval.uni.ppNumber = etrPPNumber()) != nullptr)
        retval.tag = PPToken::Tag::PP_NUMBER;
    else if((retval.uni.characterConstant = etrCharacterConstant()) != nullptr)
        retval.tag = PPToken::Tag::CHARACTER_CONSTANT;
    else if((retval.uni.stringLiteral = etrStringLiteral()) != nullptr)
        retval.tag = PPToken::Tag::STRING_LITERAL;
    else if((retval.uni.punctuator = etrPunctuator()) != nullptr)
        retval.tag = PPToken::Tag::PUNCTUATOR;
    else
    {
        retval.uni.other = mSrc[mIdx++];
        retval.tag = PPToken::Tag::OTHER;
    }

    return new PPToken(retval);
}

Punctuator *TP3::TP3::etrPunctuator()
{
    auto iter = Punctuator::PUNCTUATOR_MAP.end();
    for(std::size_t i = 4; i >= 1; i--)
    {
        iter = Punctuator::PUNCTUATOR_MAP.find(mSrc.substr(mIdx, i));
        if(iter != Punctuator::PUNCTUATOR_MAP.end())
            break;
    }

    if(iter != Punctuator::PUNCTUATOR_MAP.end())
    {
        Punctuator *retval = new Punctuator();
        retval->tag = iter->second;
        mIdx += iter->first.size();
        return retval;
    }
    else
        return nullptr;
}

StringLiteral *TP3::TP3::etrStringLiteral()
{
    if(mSrc[mIdx] != '\"')
        return nullptr;
    else
        mIdx++;

    std::string str;
    for(; mIdx < mSrc.size(); mIdx++)
    {
        if(mSrc[mIdx] != '\"' &&
           mSrc[mIdx] != '\\' &&
           mSrc[mIdx] != '\n')
            str.push_back(mSrc[mIdx]);
        else if(mSrc[mIdx] == '\\')
        {
            mIdx++;
            auto pair = procEscapeSequence();
            if(pair.first)
                str.push_back(pair.second);
            else
            {
                mIdx--;
                break;
            }
        }
        else
            break;
    }

    if((mIdx < mSrc.size()) ? (mSrc[mIdx] != '\"') : true)
    {
        mIsValid = false;
        std::cout << "TP3 error:\n"
                     "    what: string-literal is not ended.\n"
                     "    filename: " << Global::CURRENT_FILENAME
                  << std::endl;
        return nullptr;
    }
    else
        mIdx++;

    StringLiteral *retval = new StringLiteral();
    retval->str = std::move(str);
    return retval;
}

std::pair<bool, char> TP3::TP3::procEscapeSequence()
{
    char c = (mIdx < mSrc.size()) ? mSrc[mIdx] : 0;

    // simple-escape-sequence
    if(c == '\'' ||
       c == '\"' ||
       c == '\?' ||
       c == '\\' ||
       c == '\a' ||
       c == '\b' ||
       c == '\f' ||
       c == '\n' ||
       c == '\r' ||
       c == '\t' ||
       c == '\v')
        return std::make_pair(true, c);
    // octal-escape-sequence
    else if(c >= '0' && c <= '7')
    {
        int val = static_cast<int>(c - '0');
        for(;;)
        {
            if((mIdx + 1 < mSrc.size()) ? (mSrc[mIdx + 1] >= '0' && mSrc[mIdx + 1] <= '7') : false)
            {
                if(val * 8 + (mSrc[mIdx + 1] - '0') <= CHAR_MAX)
                {
                    val = val * 8 + (mSrc[mIdx + 1] - '0');
                    mIdx++;
                }
                else
                    break;
            }
            else
                break;
        }

        return std::make_pair(true, static_cast<char>(val));
    }
    // hexadecimal-escape-sequence
    else if(c == 'x')
    {
        int val = 0;
        auto befidx = mIdx;

        for(int i = 0; i < 2; i++)
        {
            if(mIdx + 1 >= mSrc.size())
                break;

            if(mSrc[mIdx + 1] >= '0' && mSrc[mIdx + 1] <= '9')
                val = val * 16 + (mSrc[mIdx + 1] - '0');
            else if(mSrc[mIdx + 1] >= 'a' && mSrc[mIdx + 1] <= 'f')
                val = val * 16 + (mSrc[mIdx + 1] - 'a' + 10);
            else if(mSrc[mIdx + 1] >= 'A' && mSrc[mIdx + 1] <= 'F')
                val = val * 16 + (mSrc[mIdx + 1] - 'A' + 10);
            else
                break;

            mIdx++;
        }

        if(befidx == mIdx)
        {
            mIsValid = false;
            std::cout << "TP3 error:\n"
                         "    what: hexadecimal-escape-sequence is not set value.\n"
                         "    filename: " << Global::CURRENT_FILENAME
                      << std::endl;
            return std::make_pair(false, 0);
        }
        else
            return std::make_pair(true, static_cast<char>(val));
    }
    else
    {
        mIsValid = false;
        std::cout << "TP3 error:\n"
                     "    what: back-slash is not evaluated.\n"
                     "    filename: " << Global::CURRENT_FILENAME
                  << std::endl;
        return std::make_pair(false, 0);
    }
}

bool TP3::TP3::procComment()
{
    if(mSrc[mIdx] != '/')
        return false;

    char c = (mIdx + 1 < mSrc.size()) ? mSrc[mIdx + 1] : 0;
    bool isProc = true;

    // // comment
    if(c == '/')
    {
        auto pos = mSrc.find('\n', mIdx + 2);
        if(pos != std::string::npos)
            mSrc.replace(mIdx, pos - mIdx, " ");
        else
            mSrc.replace(mIdx, pos, " ");
    }
    // /* */ comment
    else if(c == '*')
    {
        auto pos = mSrc.find("*/", mIdx + 2);
        if(pos != std::string::npos)
            mSrc.replace(mIdx, pos + 2 - mIdx, " ");
        else
        {
            mIsValid = false;
            std::cout << "TP3 error:\n"
                      << "    what: comment is not ended.\n"
                      << "    filename: " << Global::CURRENT_FILENAME
                      << std::endl;
        }
    }
    else
        isProc = false;

    return isProc;
}