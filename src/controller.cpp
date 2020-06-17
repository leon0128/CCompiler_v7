#include <string>
#include <vector>
#include <iostream>

#include "tp1/tp1.hpp"
#include "tp2/tp2.hpp"
#include "tp3/tp3.hpp"
#include "simbol.hpp"
#include "controller.hpp"

bool Controller::execute(const char *filename)
{
    bool isValid = true;

    std::string src;
    isValid = TP1::TP1::execute(filename, src);

    if(isValid)
        isValid = TP2::TP2::execute(src);

    std::vector<PPToken*> ptvec;
    if(isValid)
        isValid = TP3::TP3::execute(src, ptvec);

    BaseSimbol::destroy();
    return true;
}