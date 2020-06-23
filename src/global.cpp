#include "global.hpp"

namespace Global
{
    std::string CURRENT_FILENAME;

    namespace Config
    {
        const std::string FILENAME = "config.json";
        
        const std::string PREDEFINED_MACRO_FILENAME_KEY = "predefined_macro_filename";
        const std::string SYSTEM_INCLUDE_PATHS_KEY = "system_include_paths";

        std::string PREDEFINED_MACRO_FILENAME;
        std::vector<std::string> INCLUDE_SYSTEM_PATHS;
    }
}