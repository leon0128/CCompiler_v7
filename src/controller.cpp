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

    BaseSimbol::destroy();
    return true;
}

bool Controller::retokenize(std::string &src,
                            std::vector<PPToken*> &dst)
{
    return TP3::TP3::execute(src, dst);
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
        Global::INCLUDE_SYSTEM_PATHS.push_back(element.first);
    }

    return true;
}