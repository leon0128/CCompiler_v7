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
#include "tp6/tp6.hpp"
#include "tp7/tp7.hpp"
#include "global.hpp"
#include "simbol.hpp"
#include "resource_controller.hpp"
#include "controller.hpp"

bool Controller::execute(const char *filename)
{
    bool isValid = true;
    std::string src, dst;
    std::vector<PPToken*> ptvec;

    isValid = readConfig();

    if(isValid)
        isValid = include(Global::Config::PREDEFINED_MACRO_FILENAME.c_str(), ptvec);    
    ptvec.clear();

    if(isValid)
        isValid = TP1::TP1::execute(filename, src);

    if(isValid)
        isValid = TP2::TP2::execute(src);

    if(isValid)
        isValid = TP3::TP3::execute(src, ptvec);

    if(isValid)
        isValid = TP4::TP4::execute(ptvec);

    if(isValid)
        isValid = TP6::TP6::execute(ptvec);

    if(isValid)
        isValid = TP7::TP7::execute(ptvec, dst);

    if(isValid)
    {
        // output(dst);
        std::cout << dst << std::flush;
    }

    // Simbol::destroy();
    ResourceController::release();
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

bool Controller::readConfig()
{
    std::ifstream stream(Global::Config::FILENAME);
    if(!stream.is_open())
    {
        std::cout << "Controller error:\n"
                     "    what: failed to open config file.\n"
                     "    filename: " << Global::Config::FILENAME
                  << std::endl;
        return false;
    }

    boost::property_tree::ptree ptree;
    boost::property_tree::json_parser::read_json(stream, ptree);
    
    // predefined macro
    boost::optional<std::string> opf = ptree.get_optional<std::string>(Global::Config::PREDEFINED_MACRO_FILENAME_KEY);
    Global::Config::PREDEFINED_MACRO_FILENAME = opf ? *opf : "";

    BOOST_FOREACH(const decltype(ptree)::value_type &element, ptree.get_child(Global::Config::SYSTEM_INCLUDE_PATHS_KEY))
    {
        boost::optional<std::string> ostr = element.second.get_optional<std::string>("");
        if(ostr)
            Global::Config::INCLUDE_SYSTEM_PATHS.push_back(std::move(*ostr));
    }

    return true;
}

bool Controller::output(std::string &dst)
{
    std::string filename = Global::CURRENT_FILENAME.substr(0, Global::CURRENT_FILENAME.size() - 2) + ".o";
    std::ofstream stream(filename);
    if(stream.is_open())
    {
        stream << dst << std::endl;
        return true;
    }
    else
    {
        std::cerr << "Controller error:\n"
                     "    what: failed to open output file.\n"
                     "    filename: " << filename
                  << std::endl;
        return false;
    }
}