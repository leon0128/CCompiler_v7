#include <iostream>
#include <utility>

#include "../global.hpp"
#include "tokenizer.hpp"

const std::unordered_map<std::string, bool> TP4::Tokenizer::DIRECTIVE_MAP
    = {{"if", true},
       {"ifdef", true},
       {"ifndef", true},
       {"elif", true},
       {"else", true},
       {"endif", true},
       {"include", true},
       {"define", true},
       {"undef", true},
       {"line", true},
       {"error", true},
       {"pragma", true}};

bool TP4::Tokenizer::execute(std::vector<PPToken*> &ptvec,
                             PPFile *&ppFile)
{
    Tokenizer tokenizer(ptvec, ppFile);

    tokenizer.tokenize();

    return tokenizer.mIsValid;
}

TP4::Tokenizer::Tokenizer(std::vector<PPToken*> &ptvec,
                          PPFile *&ppFile) noexcept:
    mPtvec(ptvec),
    mPPFile(ppFile),
    mIdx(0),
    mIsValid(true)
{
}

void TP4::Tokenizer::tokenize()
{
    mPPFile = tokPPFile();

    if(mIdx < mPtvec.size())
    {
        mIsValid = false;
        std::cerr << "TP4 error:\n"
                     "    what: pp-token sequence not fully evaluated.\n"
                     "    filename: " << Global::CURRENT_FILENAME
                  << std::endl;
    }
}

ControlLine *TP4::Tokenizer::tokControlLine()
{
    ControlLine retval;
    auto befidx = mIdx;
    bool isValid = false;

    if(isMatch(mIdx, Punctuator::Tag::HASH))
    {
        mIdx++;

        if(isMatch(mIdx, "include"))
        {
            mIdx++;
            if((retval.uni.sInclude.ppTokens = tokPPTokens()) != nullptr)
            {
                if(isMatch(mIdx, '\n'))
                {
                    mIdx++;
                    retval.tag = ControlLine::Tag::INCLUDE;
                    isValid = true;
                }
            }
        }
        else if(isMatch(mIdx, "define") &&
                isMatch(mIdx + 1))
        {
            mIdx += 2;
            Identifier *identifier = mPtvec[mIdx - 1]->uni.identifier;

            if(isMatch(mIdx, Punctuator::Tag::L_PAREN) &&
               ((mIdx < mPtvec.size()) ? mPtvec[mIdx]->isGlued : false))
            {
                mIdx++;

                if(isMatch(mIdx, Punctuator::Tag::DDD) &&
                   isMatch(mIdx + 1, Punctuator::Tag::R_PAREN))
                {
                    mIdx += 2;
                    if((retval.uni.sDefineV.replacementList = tokReplacementList()) != nullptr)
                    {
                        if(isMatch(mIdx, '\n'))
                        {
                            mIdx++;
                            retval.uni.sDefineV.identifier = identifier;
                            retval.tag = ControlLine::Tag::DEFINE_V;
                            isValid = true;
                        }
                    }
                }
                else
                {
                    IdentifierList *identifierList = tokIdentifierList();
                    
                    if(isMatch(mIdx, Punctuator::Tag::R_PAREN))
                    {
                        mIdx++;
                        if((retval.uni.sDefineIL.replacementList = tokReplacementList()) != nullptr)
                        {
                            if(isMatch(mIdx, '\n'))
                            {
                                mIdx++;
                                retval.uni.sDefineIL.identifier = identifier;
                                retval.uni.sDefineIL.identifierList = identifierList;
                                retval.tag = ControlLine::Tag::DEFINE_IL;
                                isValid = true;
                            }
                        }
                    }
                    else if(identifierList != nullptr &&
                            isMatch(mIdx, Punctuator::Tag::COMMA) &&
                            isMatch(mIdx + 1, Punctuator::Tag::DDD) &&
                            isMatch(mIdx + 2, Punctuator::Tag::R_PAREN))
                    {
                        mIdx += 3;
                        if((retval.uni.sDefineILV.replacementList = tokReplacementList()) != nullptr)
                        {
                            if(isMatch(mIdx, '\n'))
                            {
                                mIdx++;
                                retval.uni.sDefineILV.identifier = identifier;
                                retval.uni.sDefineILV.identifierList = identifierList;
                                retval.tag = ControlLine::Tag::DEFINE_ILV;
                                isValid = true;
                            }
                        }
                    }
                }
            }
        }
        else if(isMatch(mIdx, "undef") &&
                isMatch(mIdx + 1) &&
                isMatch(mIdx + 2, '\n'))
        {
            mIdx += 3;
            retval.uni.sUndef.identifier = mPtvec[mIdx - 2]->uni.identifier;
            retval.tag = ControlLine::Tag::UNDEF;
            isValid = true;
        }
        else if(isMatch(mIdx, "line"))
        {
            mIdx++;
            if((retval.uni.sLine.ppTokens = tokPPTokens()) != nullptr)
            {
                if(isMatch(mIdx, '\n'))
                {
                    mIdx++;
                    retval.tag = ControlLine::Tag::LINE;
                    isValid = true;
                }
            }
        }
        else if(isMatch(mIdx, "error"))
        {
            mIdx++;
            retval.uni.sError.ppTokens = tokPPTokens();
            if(isMatch(mIdx, '\n'))
            {
                mIdx++;
                retval.tag = ControlLine::Tag::ERROR;
                isValid = true;
            }
        }
        else if(isMatch(mIdx, "pragma"))
        {
            mIdx++;
            retval.uni.sPragma.ppTokens = tokPPTokens();
            if(isMatch(mIdx, '\n'))
            {
                mIdx++;
                retval.tag = ControlLine::Tag::PRAGMA;
                isValid = true;
            }
        }
        else if(isMatch(mIdx, '\n'))
        {
            mIdx++;
            retval.tag = ControlLine::Tag::NEW_LINE;
            isValid = true;
        }
    }

    if(isValid)
        return new ControlLine(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

ElifGroup *TP4::Tokenizer::tokElifGroup()
{
    ElifGroup retval;
    auto befidx = mIdx;
    bool isValid = false;

    if(isMatch(mIdx, Punctuator::Tag::HASH) &&
       isMatch(mIdx + 1, "elif"))
    {
        mIdx += 2;
        if((retval.ppTokens = tokPPTokens()) != nullptr)
        {
            if(isMatch(mIdx, '\n'))
            {
                mIdx++;
                retval.group = tokGroup();
                isValid = true;
            }
        }
    }

    if(isValid)
        return new ElifGroup(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

ElifGroups *TP4::Tokenizer::tokElifGroups()
{
    std::vector<ElifGroup*> egvec;
    for(ElifGroup *eg = tokElifGroup(); eg != nullptr; eg = tokElifGroup())
        egvec.push_back(eg);
    
    if(!egvec.empty())
    {
        ElifGroups *retval = new ElifGroups();
        retval->egvec = std::move(egvec);
        return retval;
    }
    else
        return nullptr;
}

ElseGroup *TP4::Tokenizer::tokElseGroup()
{
    ElseGroup retval;
    auto befidx = mIdx;
    bool isValid = true;

    if(isMatch(mIdx, Punctuator::Tag::HASH) &&
       isMatch(mIdx + 1, "else") &&
       isMatch(mIdx + 2, '\n'))
    {
        mIdx += 3;
        retval.group = tokGroup();
        isValid = true;
    }

    if(isValid)
        return new ElseGroup(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

Group *TP4::Tokenizer::tokGroup()
{
    std::vector<GroupPart*> gpvec;
    for(GroupPart *gp = tokGroupPart(); gp != nullptr; gp = tokGroupPart())
        gpvec.push_back(gp);

    // concatenate text-line
    for(std::size_t i = 0; i + 1 < gpvec.size(); i++)
    {
        if(gpvec[i]->tag == GroupPart::Tag::TEXT_LINE &&
           gpvec[i + 1]->tag == GroupPart::Tag::TEXT_LINE)
        {
            if(gpvec[i]->uni.textLine->ppTokens != nullptr &&
               gpvec[i + 1]->uni.textLine->ppTokens != nullptr)
            {
                gpvec[i]->uni.textLine->ppTokens->ptvec
                    .insert(gpvec[i]->uni.textLine->ppTokens->ptvec.end(),
                            gpvec[i + 1]->uni.textLine->ppTokens->ptvec.begin(),
                            gpvec[i + 1]->uni.textLine->ppTokens->ptvec.end());
            }
            else if(gpvec[i]->uni.textLine->ppTokens == nullptr)
                gpvec[i]->uni.textLine->ppTokens = gpvec[i + 1]->uni.textLine->ppTokens;

            gpvec.erase(gpvec.begin() + i + 1);
            i--;
        }
    }

    if(!gpvec.empty())
    {
        Group *retval = new Group();
        retval->gpvec = std::move(gpvec);
        return retval;
    }
    else
        return nullptr;
}

GroupPart *TP4::Tokenizer::tokGroupPart()
{
    GroupPart retval;
    auto befidx = mIdx;

    if((retval.uni.ifSection = tokIfSection()) != nullptr)
        retval.tag = GroupPart::Tag::IF_SECTION;
    else if((retval.uni.controlLine = tokControlLine()) != nullptr)
        retval.tag = GroupPart::Tag::CONTROL_LINE;
    else if((retval.uni.textLine = tokTextLine()) != nullptr)
        retval.tag = GroupPart::Tag::TEXT_LINE;
    else if(isMatch(mIdx, Punctuator::Tag::HASH))
    {
        mIdx++;
        if((retval.uni.nonDirective = tokNonDirective()) != nullptr)
            retval.tag = GroupPart::Tag::NON_DIRECTIVE;
    }

    if(retval.tag != GroupPart::Tag::NONE)
        return new GroupPart(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

IdentifierList *TP4::Tokenizer::tokIdentifierList()
{
    std::vector<Identifier*> ivec;

    if(isMatch(mIdx))
    {
        ivec.push_back(mPtvec[mIdx]->uni.identifier);
        mIdx++;
    }

    for(;;)
    {
        if(isMatch(mIdx, Punctuator::Tag::COMMA) &&
           isMatch(mIdx + 1))
        {
            mIdx += 2;
            ivec.push_back(mPtvec[mIdx - 1]->uni.identifier);
        }
        else
            break;
    }

    if(!ivec.empty())
    {
        IdentifierList *retval = new IdentifierList();
        retval->ivec = std::move(ivec);
        return retval;
    }
    else
        return nullptr;
}

IfGroup *TP4::Tokenizer::tokIfGroup()
{
    IfGroup retval;
    auto befidx = mIdx;
    bool isValid = false;

    if(isMatch(mIdx, Punctuator::Tag::HASH))
    {
        mIdx++;

        if(isMatch(mIdx, "if"))
        {
            mIdx++;
            if((retval.uni.sIf.ppTokens = tokPPTokens()) != nullptr)
            {
                if(isMatch(mIdx, '\n'))
                {
                    mIdx++;
                    retval.uni.sIf.group = tokGroup();
                    retval.tag = IfGroup::Tag::IF;
                    isValid = true;
                }
            }
        }
        else if(isMatch(mIdx, "ifdef") &&
                isMatch(mIdx + 1) &&
                isMatch(mIdx + 2, '\n'))
        {
            mIdx += 3;
            retval.uni.sIfdef.identifier = mPtvec[mIdx - 2]->uni.identifier;
            retval.uni.sIfdef.group = tokGroup();
            retval.tag = IfGroup::Tag::IFDEF;
            isValid = true;
        }
        else if(isMatch(mIdx, "ifndef") &&
                isMatch(mIdx + 1) &&
                isMatch(mIdx + 2, '\n'))
        {
            mIdx += 3;
            retval.uni.sIfndef.identifier = mPtvec[mIdx - 2]->uni.identifier;
            retval.uni.sIfndef.group = tokGroup();
            retval.tag = IfGroup::Tag::IFNDEF;
            isValid = true;
        }
    }

    if(isValid)
        return new IfGroup(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

IfSection *TP4::Tokenizer::tokIfSection()
{
    IfSection retval;
    auto befidx = mIdx;
    bool isValid = false;

    if((retval.ifGroup = tokIfGroup()) != nullptr)
    {
        retval.elifGroups = tokElifGroups();
        retval.elseGroup = tokElseGroup();

        if(isMatch(mIdx, Punctuator::Tag::HASH) &&
           isMatch(mIdx + 1, "endif") &&
           isMatch(mIdx + 2, '\n'))
        {
            mIdx += 3;
            isValid = true;
        }
    }

    if(isValid)
        return new IfSection(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

NonDirective *TP4::Tokenizer::tokNonDirective()
{
    NonDirective retval;
    auto befidx = mIdx;
    bool isValid = true;

    if(isMatch(mIdx))
            isValid = DIRECTIVE_MAP.find(mPtvec[mIdx]->uni.identifier->str) == DIRECTIVE_MAP.end();

    if(isValid)
    {
        isValid = false;

        if((retval.ppTokens = tokPPTokens()) != nullptr)
        {
            if(isMatch(mIdx, '\n'))
            {
                mIdx++;
                isValid = true;
            }
        }
    }

    if(isValid)
        return new NonDirective(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

PPFile *TP4::Tokenizer::tokPPFile()
{
    PPFile *retval = new PPFile();
    retval->group = tokGroup();
    return retval;
}

PPTokens *TP4::Tokenizer::tokPPTokens()
{
    std::vector<PPToken*> ptvec;
    for(; mIdx < mPtvec.size() && !isMatch(mIdx, '\n'); mIdx++)
        ptvec.push_back(mPtvec[mIdx]);
    
    if(!ptvec.empty())
    {
        PPTokens *retval = new PPTokens();
        retval->ptvec = std::move(ptvec);
        return retval;
    }
    else
        return nullptr;
}

ReplacementList *TP4::Tokenizer::tokReplacementList()
{
    ReplacementList *retval = new ReplacementList();
    retval->ppTokens = tokPPTokens();
    return retval;
}

TextLine *TP4::Tokenizer::tokTextLine()
{
    TextLine retval;
    auto befidx = mIdx;
    bool isValid = false;
    
    retval.ppTokens = tokPPTokens();
    if(isMatch(mIdx, '\n'))
    {
        mIdx++;
        isValid = true;
    }

    if(isValid)
        return new TextLine(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

bool TP4::Tokenizer::isMatch(std::size_t idx, Punctuator::Tag tag) const
{
    if((idx < mPtvec.size()) ? (mPtvec[idx]->tag == PPToken::Tag::PUNCTUATOR) : false)
        return mPtvec[idx]->uni.punctuator->tag == tag;
    else
        return false;
}

bool TP4::Tokenizer::isMatch(std::size_t idx, std::string str) const
{
    if((idx < mPtvec.size()) ? (mPtvec[idx]->tag == PPToken::Tag::IDENTIFIER) : false)
        return (!str.empty()) ? (mPtvec[idx]->uni.identifier->str == str) : true;
    else
        return false;
}

bool TP4::Tokenizer::isMatch(std::size_t idx, char c) const
{
    if((idx < mPtvec.size()) ? (mPtvec[idx]->tag == PPToken::Tag::OTHER) : false)
        return mPtvec[idx]->uni.other == c;
    else
        return false;
}