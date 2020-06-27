#include "../simbol.hpp"
#include "converter.hpp"
#include "translator.hpp"
#include "tp7.hpp"

bool TP7::TP7::execute(const std::vector<PPToken*> &ptvec,
                       std::string &dst)
{
    bool isValid = true;

    std::vector<Token*> tvec;
    isValid = Converter::execute(ptvec, tvec);

    if(isValid)
        isValid = Translator::execute(tvec, dst);

    return isValid;
}