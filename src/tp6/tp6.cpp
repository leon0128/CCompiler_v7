#include "../simbol.hpp"
#include "tp6.hpp"

bool TP6::TP6::execute(std::vector<PPToken*> &ptvec)
{
    for(std::size_t i = 0; i + 1 < ptvec.size(); i++)
    {
        if(ptvec[i]->tag == PPToken::Tag::STRING_LITERAL &&
           ptvec[i + 1]->tag == PPToken::Tag::STRING_LITERAL)
        {
            ptvec[i]->uni.stringLiteral->str += ptvec[i + 1]->uni.stringLiteral->str;
            ptvec.erase(ptvec.begin() + i + 1);
            i--;
        }
    }

    return true;
}