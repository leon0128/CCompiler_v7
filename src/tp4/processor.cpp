#include <iostream>

#include "../integer.hpp"
#include "../global.hpp"
#include "../controller.hpp"
#include "processor.hpp"

std::unordered_map<std::string, TP4::Macro> TP4::Processor::MACRO_MAP;

bool TP4::Processor::execute(PPFile *ppFile,
                             std::vector<PPToken*> &ptvec)
{
    Processor processor(ppFile, ptvec);

    processor.process();

    return processor.mIsValid;
}

TP4::Processor::Processor(PPFile *ppFile,
                          std::vector<PPToken*> &ptvec) noexcept:
    mPPFile(ppFile),
    mPtvec(ptvec),
    mIsValid(true)
{
}

void TP4::Processor::process()
{
    proc(mPPFile);
}

void TP4::Processor::proc(ControlLine *controlLine)
{
    if(!mIsValid)
        return;

    switch(controlLine->tag)
    {
        case(ControlLine::Tag::INCLUDE):
            ctrlInclude(controlLine);
            break;
        case(ControlLine::Tag::DEFINE):
            ctrlDefine(controlLine);
            break;
        case(ControlLine::Tag::DEFINE_IL):
            ctrlDefineIL(controlLine);
            break;
        case(ControlLine::Tag::DEFINE_V):
            ctrlDefineV(controlLine);
            break;
        case(ControlLine::Tag::DEFINE_ILV):
            ctrlDefineILV(controlLine);
            break;
        case(ControlLine::Tag::UNDEF):
            ctrlUndef(controlLine);
            break;
        case(ControlLine::Tag::LINE):
            ctrlLine(controlLine);
            break;
        case(ControlLine::Tag::ERROR):
            ctrlError(controlLine);
            break;
        case(ControlLine::Tag::PRAGMA):
            ctrlPragma(controlLine);
            break;
        case(ControlLine::Tag::NEW_LINE):
            ctrl(controlLine);
            break;
        
        default:
            mIsValid = false;
            BaseSimbol::unexpectTag("ControlLine");
            break;
    }
}

void TP4::Processor::proc(Group *group)
{
    for(std::size_t i = 0; i < group->gpvec.size() && mIsValid; i++)
        proc(group->gpvec[i]);
}

void TP4::Processor::proc(GroupPart *groupPart)
{
    if(!mIsValid)
        return;

    switch(groupPart->tag)
    {
        case(GroupPart::Tag::IF_SECTION):
            proc(groupPart->uni.ifSection);
            break;
        case(GroupPart::Tag::CONTROL_LINE):
            proc(groupPart->uni.controlLine);
            break;
        case(GroupPart::Tag::TEXT_LINE):
            proc(groupPart->uni.textLine);
            break;
        case(GroupPart::Tag::NON_DIRECTIVE):
            proc(groupPart->uni.nonDirective);
            break;
        
        default:
            BaseSimbol::unexpectTag("GroupPart");
    }
}

void TP4::Processor::proc(IfSection *ifSection)
{
    if(!mIsValid)
        return;

    bool isConsumed = false;

    // if-group
    switch(ifSection->ifGroup->tag)
    {
        case(IfGroup::Tag::IF):
            if(isEvaluated(ifSection->ifGroup->uni.sIf.ppTokens))
            {
                isConsumed = true;
                if(ifSection->ifGroup->uni.sIf.group != nullptr)
                    proc(ifSection->ifGroup->uni.sIf.group);
            }
            break;
        case(IfGroup::Tag::IFDEF):
            if(isEvaluated(ifSection->ifGroup->uni.sIfdef.identifier))
            {
                isConsumed = true;
                if(ifSection->ifGroup->uni.sIfdef.group != nullptr)
                    proc(ifSection->ifGroup->uni.sIfdef.group);
            }
            break;
        case(IfGroup::Tag::IFNDEF):
            if(isEvaluated(ifSection->ifGroup->uni.sIfndef.identifier))
            {
                isConsumed = true;
                if(ifSection->ifGroup->uni.sIfndef.group != nullptr)
                    proc(ifSection->ifGroup->uni.sIfndef.group);
            }
            break;
        
        default:
            mIsValid = false;
            BaseSimbol::unexpectTag("IfGroup");
    }

    // elif-groups
    if(!isConsumed &&
       (ifSection->elifGroups != nullptr))
    {
        for(auto&& eg : ifSection->elifGroups->egvec)
        {
            if(isEvaluated(eg->ppTokens))
            {
                isConsumed = true;
                if(eg->group != nullptr)
                    proc(eg->group);
                break;
            }
        }
    }

    // else-group
    if(!isConsumed &&
       (ifSection->elseGroup != nullptr))
    {
        isConsumed = true;
        if(ifSection->elseGroup->group != nullptr)
            proc(ifSection->elseGroup->group);
    }
}

void TP4::Processor::proc(NonDirective *nonDirective)
{
    if(!mIsValid)
        return;

    mIsValid = false;
    std::cerr << "TP4 Processor implementation error:\n"
                 "    what: non-directive is no implementation.\n"
                 "    filename: " << Global::CURRENT_FILENAME << "\n"
                 "    non-directive: " << nonDirective->string()
              << std::endl; 
}

void TP4::Processor::proc(PPFile *ppFile)
{
    if(!mIsValid)
        return;
    
    if(ppFile->group != nullptr)
        proc(ppFile->group);
}

void TP4::Processor::proc(TextLine *textLine)
{
    if(!mIsValid)
        return;

    if(textLine->ppTokens != nullptr)
        expand(textLine->ppTokens->ptvec, mPtvec);
}

void TP4::Processor::ctrlInclude(ControlLine* controlLine)
{
    
}

bool TP4::Processor::isEvaluated(PPTokens *ppTokens)
{
    std::vector<PPToken*> ptvec = ppTokens->ptvec;
    
    // defined operator
    for(std::size_t i = 0; i + 1 < ptvec.size(); i++)
    {
        if((ptvec[i]->tag == PPToken::Tag::IDENTIFIER) ? (ptvec[i]->uni.identifier->str == "defined") : false)
        {
            // defined identifier
            if(ptvec[i + 1]->tag == PPToken::Tag::IDENTIFIER)
            {
                bool isDefined = MACRO_MAP.find(ptvec[i + 1]->uni.identifier->str) != MACRO_MAP.end();
                ptvec.erase(ptvec.begin() + i,
                            ptvec.begin() + i + 2);
                ptvec.insert(ptvec.begin() + i,
                             BaseSimbol::numToPt(isDefined ? "1" : "0"));
            }
            // defined ( identifier )
            else if(i + 3 < ptvec.size())
            {
                if(((ptvec[i + 1]->tag == PPToken::Tag::PUNCTUATOR) ? (ptvec[i + 1]->uni.punctuator->tag == Punctuator::Tag::L_PAREN) : false) &&
                   (ptvec[i + 2]->tag == PPToken::Tag::IDENTIFIER) &&
                   ((ptvec[i + 3]->tag == PPToken::Tag::PUNCTUATOR) ? (ptvec[i + 3]->uni.punctuator->tag == Punctuator::Tag::R_PAREN) : false))
                {
                    bool isDefined = MACRO_MAP.find(ptvec[i + 2]->uni.identifier->str) != MACRO_MAP.end();
                    ptvec.erase(ptvec.begin() + i,
                                ptvec.begin() + i + 4);
                    ptvec.insert(ptvec.begin() + i,
                                 BaseSimbol::numToPt(isDefined ? "1" : "0"));
                }
            }
        }
    }

    // expand macro
    std::vector<PPToken*> exPtvec;
    expand(ptvec, exPtvec);

    // replace remaind identifier
    for(auto&& pt : exPtvec)
    {
        if(pt->tag == PPToken::Tag::IDENTIFIER)
            pt = BaseSimbol::numToPt("0");
    }

    if(!mIsValid)
        return false;

    Integer res;
    mIsValid = Controller::evaluate(exPtvec, res);

    if(res.tag == Integer::Tag::SIGNED)
        return res.uni.i != 0;
    else
        return res.uni.u != 0;
}

bool TP4::Processor::isEvaluated(Identifier *identifier)
{
    return MACRO_MAP.find(identifier->str) != MACRO_MAP.end();
}

void TP4::Processor::expand(std::vector<PPToken*> &src,
                            std::vector<PPToken*> &dst)
{
    std::unordered_map<std::string, bool> usedMap;
    expand(src, dst, usedMap);
}

void TP4::Processor::expand(std::vector<PPToken*> &src,
                            std::vector<PPToken*> &dst,
                            std::unordered_map<std::string, bool> &usedMap)
{
    for(std::size_t i = 0; i < src.size(); i++)
    {
        if(isTarget(src, i, usedMap))
        {
            auto iter = MACRO_MAP.find(src[i]->uni.identifier->str);
            usedMap.emplace(iter->first, true);
            if(!((iter->second.isFunction) ? expandFunction(src, i, dst, usedMap) : expandObject(src, i, dst, usedMap)))
                break;
            usedMap.erase(iter->first);
        }
        else
            dst.push_back(src[i]);
    }
}

bool TP4::Processor::expandObject(std::vector<PPToken*> &src,
                                  std::size_t &idx,
                                  std::vector<PPToken*> &dst,
                                  std::unordered_map<std::string, bool> &usedMap)
{
    auto iter = MACRO_MAP.find(src[idx]->uni.identifier->str);
    expand(iter->second.repVec, dst, usedMap);
    return true;
}

bool TP4::Processor::expandFunction(std::vector<PPToken*> &src,
                                    std::size_t &idx,
                                    std::vector<PPToken*> &dst,
                                    std::unordered_map<std::string, bool> &usedMap)
{
    auto iter = MACRO_MAP.find(src[idx]->uni.identifier->str);

    std::vector<std::vector<PPToken*>> args;
    std::size_t first = idx + 2, numNest = 0;
    // get args
    for(idx++;; idx++)
    {
        // incomplete error
        if(idx >= src.size())
        {
            mIsValid = false;
            std::cout << "TP4 Processor error:\n"
                         "    what: function-like macro invocation not completed.\n"
                         "    filename: " << Global::CURRENT_FILENAME << "\n"
                         "    macro name: " << iter->first
                      << std::endl;
            return false;
        }

        // ( token
        if((src[idx]->tag == PPToken::Tag::PUNCTUATOR) ? (src[idx]->uni.punctuator->tag == Punctuator::Tag::L_PAREN) : false)
            numNest++;
        // ) token
        else if((src[idx]->tag == PPToken::Tag::PUNCTUATOR) ? (src[idx]->uni.punctuator->tag == Punctuator::Tag::R_PAREN) : false)
        {
            numNest--;
            if(numNest == 0)
            {
                // no parameter
                if(first == idx &&
                   iter->second.paramMap.empty())
                    break;
                // otherwise
                else
                {
                    args.emplace_back(src.begin() + first,
                                      src.begin() + idx);
                    break;
                }
            }
        }
        // , token
        else if(((src[idx]->tag == PPToken::Tag::PUNCTUATOR) ? (src[idx]->uni.punctuator->tag == Punctuator::Tag::COMMA) : false) &&
                numNest == 1)
        {
            // variable arguments
            if(args.size() != iter->second.paramMap.size())
            {
                args.emplace_back(src.begin() + first,
                                src.begin() + idx);
                first = idx + 1;
            }
        }
    }

    // check num of args
    if(iter->second.isVariable ? (args.size() < iter->second.paramMap.size()) : (args.size() != iter->second.paramMap.size()))
    {
        mIsValid = false;
        std::cout << "TP4 Processor error:\n"
                     "    what: number of arguments of fuinction-like macro is not match.\n"
                     "    filename: " << Global::CURRENT_FILENAME << "\n"
                     "    macro name: " << iter->first
                  << std::endl;
        return false;
    }

    // expand args
    std::vector<std::vector<PPToken*>> exArgs(args.size());
    for(std::size_t i = 0; i < args.size(); i++)
        expand(args[i], exArgs[i]);
    
    std::vector<PPToken*> repvec = iter->second.repVec;
    // # operator
    for(std::size_t i = 0; i + 1 < repvec.size(); i++)
    {
        if((repvec[i]->tag == PPToken::Tag::PUNCTUATOR) ? (repvec[i]->uni.punctuator->tag == Punctuator::Tag::HASH) : false)
        {
            auto piter = (repvec[i + 1]->tag == PPToken::Tag::IDENTIFIER) ? (iter->second.paramMap.find(repvec[i + 1]->uni.identifier->str)) : (iter->second.paramMap.end());
            std::string str;
            // parameter
            if(piter != iter->second.paramMap.end())
            {
                for(auto&& pt : args[piter->second])
                    str += pt->string();
            }
            // variable parameter (__VA_ARGS__)
            else if(((repvec[i + 1]->tag == PPToken::Tag::IDENTIFIER) ? (repvec[i + 1]->uni.identifier->str == "__VA_ARGS__") : false) &&
                    iter->second.isVariable)
            {                
                if(args.size() == iter->second.paramMap.size() + 1)
                {
                    for(auto&& pt : args.back())
                        str += pt->string();
                }
            }
            // other
            else
                str = repvec[i + 1]->string();
            
            repvec[i] = BaseSimbol::strToPt(std::move(str));
            repvec.erase(repvec.begin() + i + 1);
            i--;
        }
    }

    // replace parameter
    for(std::size_t i = 0; i < repvec.size(); i++)
    {
        if(repvec[i]->tag == PPToken::Tag::IDENTIFIER)
        {
            auto piter = iter->second.paramMap.find(repvec[i]->uni.identifier->str);
            // parameter
            if(piter != iter->second.paramMap.end())
            {
                repvec.erase(repvec.begin() + i);
                repvec.insert(repvec.begin() + i,
                              args[piter->second].begin(),
                              args[piter->second].end());
                i += static_cast<int>(args[piter->second].size()) - 1;
            }
            // variable parameter (__VA_ARGS__)
            else if(repvec[i]->uni.identifier->str == "__VA_ARGS__")
            {
                repvec.erase(repvec.begin() + i);
                if(args.size() == iter->second.paramMap.size() + 1)
                {
                    repvec.insert(repvec.begin() + i,
                                  args.back().begin(),
                                  args.back().end());
                    i += static_cast<int>(args.back().size()) - 1;
                }
                else
                    i--;
            }
        }
    }

    // ## operator
    for(std::size_t i = 0; i < repvec.size(); i++)
    {
        if((repvec[i]->tag == PPToken::Tag::PUNCTUATOR) ? (repvec[i]->uni.punctuator->tag == Punctuator::Tag::HSHS) : false)
        {
            PPToken *bef = (i != 0) ? repvec[i - 1] : nullptr;
            PPToken *aft = (i + 1 < repvec.size()) ? repvec[i + 1] : nullptr;

            std::string src;
            if(bef != nullptr)
            {
                bef->isGlued = true;
                src += bef->string();
            }
            if(aft != nullptr)
            {
                aft->isGlued = true;
                src += aft->string();
            }
        
            std::vector<PPToken*> res;
            if(!(mIsValid = Controller::retokenize(src, res)))
            {
                std::cout << "TP4 Processor error:\n"
                             "    what: failed to process ## operator.\n"
                             "    filename: " << Global::CURRENT_FILENAME << "\n"
                             "    macro name: " << iter->first
                          << std::endl;
                return false;
            }

            auto first = (i != 0) ? (repvec.begin() + i - 1) : repvec.begin();
            auto last = (i + 1 < repvec.size()) ? (repvec.begin() + i + 2) : repvec.end();
            repvec.erase(first, last);
            repvec.insert(((i != 0) ? (repvec.begin() + i - 1) : repvec.begin()),
                          res.begin(),
                          res.end());
            i += static_cast<int>(res.size()) - 1;    
        }
    }

    dst.insert(dst.end(),
               repvec.begin(),
               repvec.end());
    return true;
}

bool TP4::Processor::isTarget(std::vector<PPToken*> &src,
                              std::size_t idx,
                              std::unordered_map<std::string, bool> &usedMap) const
{
    if((idx < src.size()) ? (src[idx]->tag == PPToken::Tag::IDENTIFIER) : false)
    {
        auto iter = MACRO_MAP.end();
        if((iter = MACRO_MAP.find(src[idx]->uni.identifier->str)) != MACRO_MAP.end() &&
           usedMap.find(src[idx]->uni.identifier->str) == usedMap.end())
        {
            if(!iter->second.isFunction)
                return true;
            else
            {
                if(idx + 1 < src.size())
                    return (src[idx + 1]->tag == PPToken::Tag::PUNCTUATOR) ? (src[idx + 1]->uni.punctuator->tag == Punctuator::Tag::L_PAREN) : false;
                else
                    return false;
            }
        }
        else
            return false;
    }
    else
        return false;
}