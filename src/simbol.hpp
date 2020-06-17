#ifndef SIMBOL_HPP
#define SIMBOL_HPP

#include <string>
#include <vector>
#include <unordered_map>

class BaseSimbol;

class CharacterConstant;
class HCharSequence;
class HeaderName;
class Identifier;
class PPNumber;
class PPToken;
class Punctuator;
class QCharSequence;
class StringLiteral;

class BaseSimbol
{
public:
    static bool unexceptTag(const char *className) noexcept;

    void *operator new[](std::size_t) = delete;
    virtual ~BaseSimbol() noexcept = 0;

    virtual std::string string() = 0;
};

class CharacterConstant : public BaseSimbol
{
public:
    std::string str;
    
    CharacterConstant():
        BaseSimbol(),
        str(){}
    
    std::string string() override;
};

class HCharSequence : public BaseSimbol
{
public:
    std::string str;

    HCharSequence():
        BaseSimbol(),
        str(){}

    std::string string() override;
};

class HeaderName : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        H,
        Q
    } tag;

    union Uni
    {
        HCharSequence *h;
        QCharSequence *q;

        Uni():
            h(nullptr){}
    } uni;

    HeaderName():
        BaseSimbol(),
        tag(Tag::NONE),
        uni(){}

    std::string string() override;
};

class Identifier : public BaseSimbol
{
public:
    std::string str;

    Identifier():
        BaseSimbol(),
        str(){}

    std::string string() override;
};

class PPNumber : public BaseSimbol
{
public:
    std::string str;

    PPNumber():
        BaseSimbol(),
        str(){}
    
    std::string string() override;
};

class PPToken : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        HEADER_NAME,
        IDENTIFIER,
        PP_NUMBER,
        CHARACTER_CONSTANT,
        STRING_LITERAL,
        PUNCTUATOR,
        OTHER
    } tag;

    union Uni
    {
        HeaderName *headerName;
        Identifier *identifier;
        PPNumber *ppNumber;
        CharacterConstant *characterConstant;
        StringLiteral *stringLiteral;
        Punctuator *punctuator;
        char other;

        Uni():
            headerName(nullptr){}
    } uni;

    bool isGlued;

    PPToken():
        BaseSimbol(),
        tag(Tag::NONE),
        uni(),
        isGlued(false){}
    
    std::string string() override;
};

class Punctuator : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        SL_PAREN, // [
        SR_PAREN, // ]
        L_PAREN, // (
        R_PAREN, // )
        BL_PAREN, // {
        BR_PAREN, // }
        DOT, // .
        ARROR, // ->
        INC, // ++
        DEC, // --
        AMP, // &
        AST, // *
        PLUS, // +
        MINUS, // -
        TILDE, // ~
        EXC, // !
        SLASH, // /
        PER, // %
        L_SHIFT, // <<
        R_SHIFT, // >>
        LESS, // <
        GREATER, // >
        LESS_EQ, // <=
        GREATER_EQ, // >=
        EQUAL, // ==
        NOT_EQ, // !=
        EX_OR, // ^
        IN_OR, // |
        AND, // &&
        OR, // ||
        QUE, // ?
        COLON, // :
        SEMI_COL, // ;
        DDD, // ...
        ASS, // =
        MUL_ASS, // *=
        DIV_ASS, // /=
        REM_ASS, // %=
        ADD_ASS, // +=
        SUB_ASS, // -=
        LSFT_ASS, // <<=
        RSFT_ASS, // >>=
        BW_AND_ASS, // &=
        BW_EXOR_ASS, // ^=
        BW_INOR_ASS, // |=
        COMMA, // ,
        HASH, // #
        HSHS // ##
    } tag;

    Punctuator():
        BaseSimbol(),
        tag(Tag::NONE){}
    
    std::string string() override;

    static const std::unordered_map<std::string, Tag> PUNCTUATOR_MAP;
};

class QCharSequence : public BaseSimbol
{
public:
    std::string str;

    QCharSequence():
        BaseSimbol(),
        str(){}

    std::string string() override;
};

class StringLiteral : public BaseSimbol
{
public:
    std::string str;

    StringLiteral():
        BaseSimbol(),
        str(){}

    std::string string() override;
};

#endif