#ifndef TP4_TOKENIZER_HPP
#define TP4_TOKENIZER_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include "../simbol.hpp"

namespace TP4
{

class Tokenizer
{
private:
    static const std::unordered_map<std::string, bool> DIRECTIVE_MAP;

public:
    static bool execute(std::vector<PPToken*>&,
                        PPFile*&);

private:
    Tokenizer(std::vector<PPToken*>&,
              PPFile*&) noexcept;
    
    void tokenize();

    // tokenize helper function
    // tokenize each elements
    ControlLine *tokControlLine();
    ElifGroup *tokElifGroup();
    ElifGroups *tokElifGroups();
    ElseGroup *tokElseGroup();
    Group *tokGroup();
    GroupPart *tokGroupPart();
    IdentifierList *tokIdentifierList();
    IfGroup *tokIfGroup();
    IfSection *tokIfSection();
    NonDirective *tokNonDirective();
    PPFile *tokPPFile();
    PPTokens *tokPPTokens();
    ReplacementList *tokReplacementList();
    TextLine *tokTextLine();

    // compare value
    bool isMatch(std::size_t idx, Punctuator::Tag tag) const; // for puncautator
    bool isMatch(std::size_t idx, std::string str = std::string()) const; // for identifier
    bool isMatch(std::size_t idx, char c) const; // for other

    std::vector<PPToken*> &mPtvec;
    PPFile *&mPPFile;
    std::size_t mIdx;

    bool mIsValid;
};

}

#endif