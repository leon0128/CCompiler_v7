#ifndef SIMBOL_HPP
#define SIMBOL_HPP

#include <new>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

class BaseSimbol;

class Token;
class Keyword;
class Constant;
class IntegerConstant;
class FloatingConstant;
class EnumerationConstant;

class CharacterConstant;
class HCharSequence;
class HeaderName;
class Identifier;
class PPNumber;
class PPToken;
class Punctuator;
class QCharSequence;
class StringLiteral;

class IdentifierList;

class ControlLine;
class ElifGroup;
class ElifGroups;
class ElseGroup;
class Group;
class GroupPart;
class IfGroup;
class IfSection;
class NonDirective;
class PPFile;
class PPTokens;
class ReplacementList;
class TextLine;

namespace Simbol
{
    extern void destroy() noexcept;
    extern bool unexpectTag(const char *className) noexcept;

    template<class String>
    extern PPToken *strToPt(String&&);
    template<class String>
    extern PPToken *numToPt(String&&);
    template<class Tag>
    extern PPToken *puncToPt(Tag tag);
}

class BaseSimbol
{
public:
    friend void Simbol::destroy() noexcept;

    BaseSimbol() noexcept {}
    void *operator new(std::size_t);
    void *operator new[](std::size_t) = delete;
    virtual ~BaseSimbol() noexcept = 0;

    virtual std::string string() const = 0;

private:
    static std::vector<BaseSimbol*> ALLOCATED_SIMBOLS;
};

class Token : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        KEYWORD,
        IDENTIFIER,
        CONSTANT,
        STRING_LITERAL,
        PUNCTUATOR
    } tag;

    union Uni
    {
        Keyword *keyword;
        Identifier *identifier;
        Constant *constant;
        StringLiteral *stringLiteral;
        Punctuator *punctuator;
    } uni;

    Token() noexcept:
        BaseSimbol(),
        tag(Tag::NONE),
        uni{nullptr}{}

    std::string string() const override;
};

class Keyword : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        AUTO,
        BREAK,
        CASE,
        CHAR,
        CONST,
        CONTINUE,
        DEFAULT,
        DO,
        DOUBLE,
        ELSE,
        ENUM,
        EXTERN,
        FLOAT,
        FOR,
        GOTO,
        IF,
        INLINE,
        INT,
        LONG,
        REGISTER,
        RESTRICT,
        RETURN,
        SHORT,
        SIGNED,
        SIZEOF,
        STATIC,
        STRUCT,
        SWITCH,
        TYPEDEF,
        UNION,
        UNSIGNED,
        VOID,
        VOLATILE,
        WHILE,
        ALIGNAS,
        ALIGNOF,
        ATOMIC,
        BOOL,
        COMPLEX,
        GENERIC,
        IMAGINARY,
        NORETURN,
        STATIC_ASSERT,
        THREAD_LOCAL
    } tag;

    Keyword() noexcept:
        BaseSimbol(),
        tag(Tag::NONE){}
    
    std::string string() const;

    static const std::unordered_map<std::string, Tag> KEYWORD_MAP;
};

class Constant : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        INTEGER,
        FLOATING,
        ENUMERATION,
        CHARACTER
    } tag;

    union Uni
    {
        IntegerConstant *integer;
        FloatingConstant *floating;
        EnumerationConstant *enumeration;
        CharacterConstant *character;
    } uni;

    Constant() noexcept:
        BaseSimbol(),
        tag(Tag::NONE),
        uni{nullptr}{}

    std::string string() const override;
};

class IntegerConstant : public BaseSimbol
{
public:
    enum class PrefixTag
    {
        NONE,
        DECIMAL,
        OCTAL,
        HEXADECIMAL
    } prefixTag;
    enum class SuffixTag
    {
        NONE,
        UNSIGNED,
        UNSIGNED_LONG,
        UNSIGNED_LONG_LONG,
        LONG,
        LONG_LONG
    } suffixTag;

    std::string str;

    IntegerConstant() noexcept:
        BaseSimbol(),
        prefixTag(PrefixTag::NONE),
        suffixTag(SuffixTag::NONE),
        str(){}

    std::string string() const override;
};

class FloatingConstant : public BaseSimbol
{
public:
    enum class RadixTag
    {
        NONE,
        DECIMAL,
        HEXADECIMAL
    } radixTag;
    enum class ExponentTag
    {
        NONE,
        PLUS,
        MINUS
    } exponentTag;
    enum class SuffixTag
    {
        NONE,
        FLOAT,
        LONG
    } suffixTag;

    std::string integer;
    std::string decimal;
    std::string exponent;

    FloatingConstant() noexcept:
        BaseSimbol(),
        radixTag(RadixTag::NONE),
        exponentTag(ExponentTag::NONE),
        suffixTag(SuffixTag::NONE),
        integer(),
        decimal(),
        exponent(){}

    std::string string() const override;
};

class EnumerationConstant : public BaseSimbol
{
public:
    Identifier *identifier;

    EnumerationConstant() noexcept:
        BaseSimbol(),
        identifier(nullptr){}

    std::string string() const override;
};

class CharacterConstant : public BaseSimbol
{
public:
    std::string str;
    
    CharacterConstant() noexcept:
        BaseSimbol(),
        str(){}
    
    std::string string() const  override;
};

class HCharSequence : public BaseSimbol
{
public:
    std::string str;

    HCharSequence() noexcept:
        BaseSimbol(),
        str(){}

    std::string string() const override;
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

    HeaderName() noexcept:
        BaseSimbol(),
        tag(Tag::NONE),
        uni(){}

    std::string string() const override;
};

class Identifier : public BaseSimbol
{
public:
    std::string str;

    Identifier() noexcept:
        BaseSimbol(),
        str(){}

    std::string string() const override;
};

class PPNumber : public BaseSimbol
{
public:
    std::string str;

    PPNumber() noexcept:
        BaseSimbol(),
        str(){}
    
    std::string string() const override;
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

    PPToken() noexcept:
        BaseSimbol(),
        tag(Tag::NONE),
        uni(),
        isGlued(false){}
    
    std::string string() const override;
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

    Punctuator() noexcept:
        BaseSimbol(),
        tag(Tag::NONE){}
    
    std::string string() const override;

    static const std::unordered_map<std::string, Tag> PUNCTUATOR_MAP;
};

class QCharSequence : public BaseSimbol
{
public:
    std::string str;

    QCharSequence() noexcept:
        BaseSimbol(),
        str(){}

    std::string string() const override;
};

class StringLiteral : public BaseSimbol
{
public:
    std::string str;

    StringLiteral() noexcept:
        BaseSimbol(),
        str(){}

    std::string string() const override;
};

class IdentifierList : public BaseSimbol
{
public:
    std::vector<Identifier*> ivec;

    IdentifierList() noexcept:
        BaseSimbol(),
        ivec(){}
    
    std::string string() const override;
};

class ControlLine : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        INCLUDE,
        DEFINE,
        DEFINE_IL, // identifier-list
        DEFINE_V, // variable arguments
        DEFINE_ILV, // identifier-list and variable arguments
        UNDEF,
        LINE,
        ERROR,
        PRAGMA,
        NEW_LINE
    } tag;

    union Uni
    {
        struct SInclude
        {
            PPTokens *ppTokens;
        } sInclude;
        struct SDefine
        {
            Identifier *identifier;
            ReplacementList *replacementList;
        } sDefine;
        struct SDefineIL
        {
            Identifier *identifier;
            IdentifierList *identifierList;
            ReplacementList *replacementList;
        } sDefineIL;
        struct SDefineV
        {
            Identifier *identifier;
            ReplacementList *replacementList;
        } sDefineV;
        struct SDefineILV
        {
            Identifier *identifier;
            IdentifierList *identifierList;
            ReplacementList *replacementList;
        } sDefineILV;
        struct SUndef
        {
            Identifier *identifier;
        } sUndef;
        struct SLine
        {
            PPTokens *ppTokens;
        } sLine;
        struct SError
        {
            PPTokens *ppTokens;
        } sError;
        struct SPragma
        {
            PPTokens *ppTokens;
        } sPragma;
        
        Uni() noexcept:
            sInclude{nullptr}{}
    } uni;

    ControlLine() noexcept:
        BaseSimbol(),
        tag(Tag::NONE),
        uni(){}
    
    std::string string() const override;
};

class ElifGroup : public BaseSimbol
{
public:
    PPTokens *ppTokens;
    Group *group;

    ElifGroup() noexcept:
        BaseSimbol(),
        ppTokens(nullptr),
        group(nullptr){}
    
    std::string string() const override;
};

class ElifGroups : public BaseSimbol
{
public:
    std::vector<ElifGroup*> egvec;

    ElifGroups() noexcept:
        BaseSimbol(),
        egvec(){}

    std::string string() const override;
};

class ElseGroup : public BaseSimbol
{
public:
    Group *group;

    ElseGroup() noexcept:
        BaseSimbol(),
        group(nullptr){}
    
    std::string string() const override;
};

class Group : public BaseSimbol
{
public:
    std::vector<GroupPart*> gpvec;

    Group() noexcept:
        BaseSimbol(),
        gpvec(){}

    std::string string() const override;
};

class GroupPart : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        IF_SECTION,
        CONTROL_LINE,
        TEXT_LINE,
        NON_DIRECTIVE
    } tag;

    union Uni
    {
        IfSection *ifSection;
        ControlLine *controlLine;
        TextLine *textLine;
        NonDirective *nonDirective;

        Uni() noexcept:
            ifSection(nullptr){}
    } uni;

    GroupPart() noexcept:
        BaseSimbol(),
        tag(Tag::NONE),
        uni(){}
    
    std::string string() const override;
};

class IfGroup : public BaseSimbol
{
public:
    enum class Tag
    {
        NONE,
        IF,
        IFDEF,
        IFNDEF
    } tag;

    union Uni
    {
        struct SIf
        {
            PPTokens *ppTokens;
            Group *group;
        } sIf;
        struct SIfdef
        {
            Identifier *identifier;
            Group *group;
        } sIfdef;
        struct SIfndef
        {
            Identifier *identifier;
            Group *group;
        } sIfndef;

        Uni() noexcept:
            sIf{nullptr, nullptr}{}
    } uni;

    IfGroup() noexcept:
        BaseSimbol(),
        tag(Tag::NONE),
        uni(){}

    std::string string() const override;
};

class IfSection : public BaseSimbol
{
public:
    IfGroup *ifGroup;
    ElifGroups *elifGroups;
    ElseGroup *elseGroup;

    IfSection() noexcept:
        BaseSimbol(),
        ifGroup(nullptr),
        elifGroups(nullptr),
        elseGroup(nullptr){}
    
    std::string string() const override;
};

class NonDirective : public BaseSimbol
{
public:
    PPTokens *ppTokens;

    NonDirective() noexcept:
        BaseSimbol(),
        ppTokens(nullptr){}
    
    std::string string() const override;
};

class PPFile : public BaseSimbol
{
public:
    Group *group;

    PPFile() noexcept:
        BaseSimbol(),
        group(nullptr){}

    std::string string() const override;
};

class PPTokens : public BaseSimbol
{
public:
    std::vector<PPToken*> ptvec;

    PPTokens() noexcept:
        BaseSimbol(),
        ptvec(){}
    
    std::string string() const override;
};

class ReplacementList : public BaseSimbol
{
public:
    PPTokens *ppTokens;

    ReplacementList() noexcept:
        BaseSimbol(),
        ppTokens(nullptr){}
    
    std::string string() const override;
};

class TextLine : public BaseSimbol
{
public:
    PPTokens *ppTokens;

    TextLine() noexcept:
        BaseSimbol(),
        ppTokens(nullptr){}
    
    std::string string() const override;
};

template<class String>
PPToken *Simbol::strToPt(String &&str)
{
    PPToken *retval = new PPToken();
    retval->tag = PPToken::Tag::STRING_LITERAL;
    retval->uni.stringLiteral = new StringLiteral();
    retval->uni.stringLiteral->str = std::forward<String>(str);
    return retval;
}

template<class String>
PPToken *Simbol::numToPt(String &&str)
{
    PPToken *retval = new PPToken();
    retval->tag = PPToken::Tag::PP_NUMBER;
    retval->uni.ppNumber = new PPNumber();
    retval->uni.ppNumber->str = std::forward<String>(str);
    return retval;
}

template<class Tag>
PPToken *Simbol::puncToPt(Tag tag)
{
    PPToken *retval = new PPToken();
    retval->tag = PPToken::Tag::PUNCTUATOR;
    retval->uni.punctuator = new Punctuator();
    retval->uni.punctuator->tag = tag;
    return retval;
}

namespace Simbol
{
    extern bool isMatch(const std::vector<Token*> &vec,
        std::size_t idx,
        Punctuator::Tag tag);
    extern bool isMatch(const std::vector<Token*> &vec,
        std::size_t idx,
        Keyword::Tag tag);
    extern bool isMatch(const std::vector<Token*> &vec,
        std::size_t idx,
        Token::Tag tag);
    template<class String>
    extern bool isMatch(const std::vector<Token*> &vec,
        std::size_t idx,
        String &&str)
    {
        return idx < vec.size()
            ? vec[idx]->tag == Token::Tag::IDENTIFIER
                ? vec[idx]->uni.identifier->str == std::forward(str)
                    : false
                        : false;
    }
}

#endif