#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <string>

namespace TP1{class TP1;}
namespace TP2{class TP2;}

class Controller
{
public:
    friend class TP1::TP1;
    friend class TP2::TP2;

    static bool execute(const char *filename);

private:
    static std::string CURRENT_FILENAME;

    Controller() noexcept;


    bool mIsValid;
};

#endif