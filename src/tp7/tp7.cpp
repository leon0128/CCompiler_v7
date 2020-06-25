#include "../simbol.hpp"
#include "converter.hpp"
#include "tp7.hpp"

bool TP7::TP7::execute(std::vector<PPToken*> &ptvec,
                       std::string &dst)
{
    bool isValid = true;

    std::vector<Token*> tvec;
    isValid = Converter::execute(ptvec, tvec);

    return isValid;
}