#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <string>
#include <vector>

class Global
{
public:
    Global() = delete;

    static std::string CURRENT_FILENAME;

    static std::string JSON_FILENAME;
    static std::vector<std::string> INCLUDE_SYSTEM_PATHS;
};

#endif