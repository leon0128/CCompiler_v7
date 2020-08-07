#include <fstream>
#include <algorithm>
#include <iostream>

#include <boost/optional.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../global.hpp"
#include "type.hpp"

namespace TP7
{

const std::string Type::NAME_KEY = "type";
const std::string Type::SIZE_KEY = "size";
const std::string Type::ALIGN_KEY = "align";

const std::unordered_set<Keyword::Tag> Type::TYPE_SPECIFIER_SET
    = {Keyword::Tag::VOID,
       Keyword::Tag::CHAR,
       Keyword::Tag::SHORT,
       Keyword::Tag::INT,
       Keyword::Tag::LONG,
       Keyword::Tag::SIGNED,
       Keyword::Tag::UNSIGNED,
       Keyword::Tag::FLOAT,
       Keyword::Tag::DOUBLE,
       Keyword::Tag::BOOL};

const std::unordered_map<std::string, Type::Tag> Type::TYPE_NAME_MAP
    = {{"void", Tag::VOID},
       {"char", Tag::CHAR},
       {"s_char", Tag::S_CHAR},
       {"u_char", Tag::U_CHAR},
       {"s_short", Tag::S_SHORT},
       {"u_short", Tag::U_SHORT},
       {"s_int", Tag::S_INT},
       {"u_int", Tag::U_INT},
       {"s_long", Tag::S_LONG},
       {"u_long", Tag::U_LONG},
       {"s_long_long", Tag::S_LONG_LONG},
       {"u_long_long", Tag::U_LONG_LONG},
       {"float", Tag::FLOAT},
       {"double", Tag::DOUBLE},
       {"long_double", Tag::LONG_DOUBLE},
       {"bool", Tag::BOOL}};

std::unordered_map<Type::Tag, TypeAttribute> Type::TYPE_ATTRIBUTE_MAP;

std::unordered_map<Type::Tag, std::vector<std::vector<Keyword::Tag>>> Type::TYPE_SPECIFIER_MAP
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

bool Type::initialize()
{
    if(!read())
        return false;
    
    sort();
    return true;
}

Type::Tag Type::convert(std::vector<Keyword::Tag> &src)
{
    // sort
    std::sort(src.begin(), src.end());    

    // compare
    for(const auto &p : TYPE_SPECIFIER_MAP)
    {
        for(const auto &v : p.second)
        {
            if(src == v)
                return p.first;
        }
    }

    return Tag::NONE;
}

bool Type::isTypeSpecifier(Keyword::Tag tag) noexcept
{
    return TYPE_SPECIFIER_SET.find(tag) != TYPE_SPECIFIER_SET.end();
}

bool Type::isIntegerType(Type::Tag tag) noexcept
{
    if(tag == Tag::CHAR ||
        tag == Tag::S_CHAR ||
        tag == Tag::U_CHAR ||
        tag == Tag::S_SHORT ||
        tag == Tag::U_SHORT ||
        tag == Tag::S_INT ||
        tag == Tag::U_INT ||
        tag == Tag::S_LONG ||
        tag == Tag::U_LONG ||
        tag == Tag::S_LONG_LONG ||
        tag == Tag::U_LONG_LONG ||
        tag == Tag::BOOL)
        return true;
    else
        return false;
}

bool Type::isFloatingType(Type::Tag tag) noexcept
{
    if(tag == Tag::FLOAT ||
        tag == Tag::DOUBLE ||
        tag == Tag::LONG_DOUBLE)
        return true;
    else
        return false;
}

void Type::sort()
{
    for(auto &&p : TYPE_SPECIFIER_MAP)
    {
        for(auto &&v : p.second)
            std::sort(v.begin(), v.end());
    }
}

bool Type::read()
{
    std::ifstream stream(Global::Config::FILENAME);
    if(!stream.is_open())
    {
        std::cout << "TP7 TypeSpecifier error:\n"
                     "    what: failed to open config file.\n"
                     "    filename: " << Global::Config::FILENAME
                  << std::endl;
        return false;
    }

    bool isValid = true;

    boost::property_tree::ptree ptree;
    boost::property_tree::json_parser::read_json(stream, ptree);

    BOOST_FOREACH(const decltype(ptree)::value_type &child, ptree.get_child(Global::Config::TYPE_ATTRIBUTE_KEY))
    {
        boost::optional<std::string> ostr
            = child.second.get_optional<std::string>(NAME_KEY);
        boost::optional<int> osize
            = child.second.get_optional<int>(SIZE_KEY);
        boost::optional<int> oalign
            = child.second.get_optional<int>(ALIGN_KEY);
        
        if(ostr && osize && oalign)
        {
            auto nameIter = TYPE_NAME_MAP.find(*ostr);
            if(nameIter != TYPE_NAME_MAP.end())
            {
                TYPE_ATTRIBUTE_MAP
                    .emplace(nameIter->second, TypeAttribute{*osize, *oalign});
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }

    if(isValid)
        return true;
    else
    {
        std::cout << "TP7 TypeSpecifier error:\n"
            "    what: failed to get type information.\n"
            "    config filename: " << Global::Config::FILENAME
            << std::endl;
        return false;
    }
}

}