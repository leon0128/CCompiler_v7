#ifndef TP7_CONVERTER_HPP
#define TP7_CONVERTER_HPP

#include <vector>

class PPToken;
class Token;

namespace TP7
{

class Converter
{
public:
    static bool execute(std::vector<PPToken*>&,
                        std::vector<Token*>&);

private:
    static Token *convert(PPToken*);
};

}

#endif