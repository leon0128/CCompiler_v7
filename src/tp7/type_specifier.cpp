#include "type_specifier.hpp"

namespace TP7
{

const std::unordered_map<TypeSpecifier::Tag, std::vector<std::vector<Keyword::Tag>>> TypeSpecifier::TYPE_SPECIFIER_MAP
    = {{Tag::VOID,
        {{Keyword::Tag::VOID}}},
       {Tag::CHAR,
        {{Keyword::Tag::CHAR}}},
       {Tag::S_CHAR,
        {{Keyword::Tag::SIGNED, Keyword::Tag::CHAR}}},
       {Tag::U_CHAR,
        {{Keyword::Tag::UNSIGNED, Keyword::Tag::CHAR}}},
       {Tag::S_SHORT,
        {{Keyword::Tag::SHORT},
         {Keyword::Tag::SIGNED, Keyword::Tag::SHORT},
         {Keyword::Tag::SHORT, Keyword::Tag::INT},
         {Keyword::Tag::SIGNED, Keyword::Tag::SHORT, Keyword::Tag::INT}}},
       {Tag::U_SHORT,
        {{Keyword::Tag::UNSIGNED, Keyword::Tag::SHORT},
         {Keyword::Tag::UNSIGNED, Keyword::Tag::SHORT, Keyword::Tag::INT}}},
       {Tag::S_INT,
        {{Keyword::Tag::INT},
         {Keyword::Tag::SIGNED},
         {Keyword::Tag::SIGNED, Keyword::Tag::INT}}},
       {Tag::U_INT,
        {{Keyword::Tag::UNSIGNED},
         {Keyword::Tag::UNSIGNED, Keyword::Tag::INT}}},
       {Tag::S_LONG,
        {{Keyword::Tag::LONG},
         {Keyword::Tag::SIGNED, Keyword::Tag::LONG},
         {Keyword::Tag::LONG, Keyword::Tag::INT},
         {Keyword::Tag::SIGNED, Keyword::Tag::LONG, Keyword::Tag::INT}}},
       {Tag::U_LONG,
        {{Keyword::Tag::UNSIGNED, Keyword::Tag::LONG},
         {Keyword::Tag::UNSIGNED, Keyword::Tag::LONG, Keyword::Tag::INT}}},
       {Tag::S_LONG_LONG,
        {{Keyword::Tag::LONG, Keyword::Tag::LONG},
         {Keyword::Tag::SIGNED, Keyword::Tag::LONG, Keyword::Tag::LONG},
         {Keyword::Tag::LONG, Keyword::Tag::LONG, Keyword::Tag::INT},
         {Keyword::Tag::SIGNED, Keyword::Tag::LONG, Keyword::Tag::LONG, Keyword::Tag::INT}}},
       {Tag::U_LONG_LONG,
        {{Keyword::Tag::UNSIGNED, Keyword::Tag::LONG, Keyword::Tag::LONG},
         {Keyword::Tag::UNSIGNED, Keyword::Tag::LONG, Keyword::Tag::LONG, Keyword::Tag::INT}}},
       {Tag::FLOAT,
        {{Keyword::Tag::FLOAT}}},
       {Tag::DOUBLE,
        {{Keyword::Tag::DOUBLE}}},
       {Tag::LONG_DOUBLE,
        {{Keyword::Tag::LONG, Keyword::Tag::DOUBLE}}},
       {Tag::BOOL,
        {{Keyword::Tag::BOOL}}}};

TypeSpecifier::Tag TypeSpecifier::convert(const std::vector<Keyword::Tag> &src)
{

}

}