#ifndef TP3_TP3_HPP
#define TP3_TP3_HPP

#include <string>
#include <vector>

class CharacterConstant;
class HCharSequence;
class HeaderName;
class Identifier;
class PPNumber;
class PPToken;
class Punctuator;
class QCharSequence;
class StringLiteral;

namespace TP3
{

class TP3
{
private:
    static const std::unordered_map<char, char> SIMPLE_ESCAPE_SEQUENCE_MAP;

public:
    static bool execute(std::string &src,
                        std::vector<PPToken*> &ptvec);

private:
    TP3(std::string&,
        std::vector<PPToken*>&) noexcept;

    void decompose();

    // decompose helper
    // extract each simbol
    CharacterConstant *etrCharacterConstant();
    Identifier *etrIdentifier();
    PPNumber *etrPPNumber();
    PPToken *etrPPToken();
    Punctuator *etrPunctuator();
    StringLiteral *etrStringLiteral();

    std::pair<bool, char> procEscapeSequence();
    bool procComment();

    std::string &mSrc;
    std::size_t mIdx;
    std::vector<PPToken*> &mPtvec;

    bool mIsValid;
};

}

#endif