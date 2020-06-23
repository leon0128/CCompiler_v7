#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <boost/optional.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "tp1/tp1.hpp"
#include "tp2/tp2.hpp"
#include "tp3/tp3.hpp"
#include "tp4/tp4.hpp"
#include "global.hpp"
#include "simbol.hpp"
#include "controller.hpp"

bool Controller::execute(const char *filename)
{
    bool isValid = true;

    isValid = readIncludeSystemPaths();

    std::string src;
    if(isValid)
        isValid = TP1::TP1::execute(filename, src);

    if(isValid)
        isValid = TP2::TP2::execute(src);

    std::vector<PPToken*> ptvec;
    if(isValid)
        isValid = TP3::TP3::execute(src, ptvec);

    if(isValid)
        isValid = TP4::TP4::execute(ptvec);

    for(const auto &pt : ptvec)
        std::cout << pt->string() << std::endl;

    BaseSimbol::destroy();
    return true;
}

bool Controller::retokenize(std::string &src,
                            std::vector<PPToken*> &dst)
{
    return TP3::TP3::execute(src, dst);
}

bool Controller::include(const std::string &filename,
                         std::vector<PPToken*> &dst)
{
    bool isValid = true;

    std::string src;
    isValid = TP1::TP1::execute(filename.c_str(), src);

    if(isValid)
        isValid = TP2::TP2::execute(src);
    
    std::vector<PPToken*> ptvec;
    if(isValid)
        isValid = TP3::TP3::execute(src, ptvec);
    
    if(isValid)
        isValid = TP4::TP4::execute(ptvec);
    
    dst = std::move(ptvec);
    return isValid;
}

bool Controller::readIncludeSystemPaths()
{
    std::ifstream stream(Global::JSON_FILENAME);
    if(!stream.is_open())
    {
        std::cout << "Controller error:\n"
                     "    what: failed to open config file.\n"
                     "    filename: " << Global::JSON_FILENAME
                  << std::endl;
        return false;
    }

    boost::property_tree::ptree ptree;
    boost::property_tree::json_parser::read_json(stream, ptree);

    BOOST_FOREACH(const decltype(ptree)::value_type &element, ptree.get_child("include_system_paths"))
    {
        boost::optional<std::string> ostr = element.second.get_optional<std::string>("");
        if(ostr)
            Global::INCLUDE_SYSTEM_PATHS.push_back(std::move(*ostr));
    }

    return true;
}