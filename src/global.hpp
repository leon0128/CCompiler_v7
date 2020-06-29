#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <string>
#include <vector>

namespace Global
{
    extern std::string CURRENT_FILENAME;

    namespace Config
    {
        extern const std::string FILENAME;
        
        extern const std::string PREDEFINED_MACRO_FILENAME_KEY;
        extern const std::string SYSTEM_INCLUDE_PATHS_KEY;
        extern const std::string TYPE_ATTRIBUTE_KEY;

        extern std::string PREDEFINED_MACRO_FILENAME;
        extern std::vector<std::string> INCLUDE_SYSTEM_PATHS;
    }
}

#endif