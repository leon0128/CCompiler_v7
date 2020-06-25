#include "../simbol.hpp"
#include "converter.hpp"

bool TP7::Converter::execute(std::vector<PPToken*> &src,
                             std::vector<Token*> &dst)
{
    bool isValid = true;
    for(std::size_t i = 0; i < src.size(); i++)
    {
        Token *token = convert(src[i]);
        if(token != nullptr)
            dst.push_back(token);
        else
        {
            isValid = false;
            break;
        }
    }

    return isValid;
}

Token *TP7::Converter::convert(PPToken *ppToken)
{
    return nullptr;
}