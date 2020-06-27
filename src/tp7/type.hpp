#ifndef TP7_TYPE_HPP
#define TP7_TYPE_HPP

#include <array>

namespace TP7
{

class StructOrUnion;
class Enum;
class Typedef;

class StorageClassSpecifier
{
public:
    enum class Tag
    {
        TYPEDEF,
        EXTERN,
        STATIC,
        AUTO,
        REGISTER,
        TAG_SIZE
    };

    inline StorageClassSpecifier() noexcept:
        mFlags()
    {
        mFlags.fill(false);
    }

    inline void on(Tag tag) noexcept {mFlags[static_cast<std::size_t>(tag)] = true;}
    inline void off(Tag tag) noexcept {mFlags[static_cast<std::size_t>(tag)] = false;}
    inline bool get(Tag tag) const noexcept {return mFlags[static_cast<std::size_t>(tag)];}

private:
    std::array<bool, static_cast<std::size_t>(Tag::TAG_SIZE)> mFlags;
};

class TypeSpecifier
{
public:
    enum class Tag
    {
        NONE,
        VOID,
        CHAR,
        S_CHAR,
        U_CHAR,
        S_SHORT,
        U_SHORT,
        S_INT,
        U_LONG,
        S_LONG_LONG,
        U_LONG_LONG,
        FLOAT,
        DOUBLE,
        LONG_DOUBLE,
        BOOL,
        STRUCT_OR_UNION,
        ENUM,
        TYPEDEF,
        TAG_SIZE
    };

    union Uni
    {
        StructOrUnion *structOrUnion;
        Enum *enumClass;
        Typedef *typedefClass;
    };

    inline constexpr TypeSpecifier() noexcept:
        mTag(Tag::NONE),
        mUni{nullptr}{}

    inline void setTag(Tag tag) noexcept {mTag = tag;}
    inline constexpr Tag getTag() const noexcept {return mTag;}
    inline constexpr Uni &getUni() noexcept {return mUni;}

private:
    Tag mTag;
    Uni mUni;
};

class TypeQualifier
{
public:
    enum class Tag
    {
        CONST,
        RESTRICT,
        VOLATILE,
        TAG_SIZE
    };

    inline TypeQualifier() noexcept:
        mFlags()
    {
        mFlags.fill(false);
    }

    inline void on(Tag tag) noexcept {mFlags[static_cast<std::size_t>(tag)] = true;}
    inline void off(Tag tag) noexcept {mFlags[static_cast<std::size_t>(tag)] = false;}
private:
    std::array<bool, static_cast<std::size_t>(Tag::TAG_SIZE)> mFlags;
};

class FunctionSpecifier
{
public:
    enum class Tag
    {
        INLINE,
        NORETURN,
        TAG_SIZE
    };

    inline FunctionSpecifier() noexcept:
        mFlags()
    {
        mFlags.fill(false);
    }

    inline void on(Tag tag) noexcept {mFlags[static_cast<std::size_t>(tag)] = true;}
    inline void off(Tag tag) noexcept {mFlags[static_cast<std::size_t>(tag)] = false;}
    inline bool get(Tag tag) const noexcept {return mFlags[static_cast<std::size_t>(tag)];}

private:
    std::array<bool, static_cast<std::size_t>(Tag::TAG_SIZE)> mFlags;
};

class DeclarationSpecifiers
{
public:
    inline DeclarationSpecifiers() noexcept:
        storageClassSpecifier(new StorageClassSpecifier()),
        typeSpecifier(new TypeSpecifier()),
        typeQualifier(new TypeQualifier()),
        functionSpecifier(new FunctionSpecifier()){}
    inline ~DeclarationSpecifiers() noexcept
    {
        delete storageClassSpecifier;
        delete typeSpecifier;
        delete typeQualifier;
        delete functionSpecifier;
    }

    StorageClassSpecifier *storageClassSpecifier;
    TypeSpecifier *typeSpecifier;
    TypeQualifier *typeQualifier;
    FunctionSpecifier *functionSpecifier;
};

}

#endif