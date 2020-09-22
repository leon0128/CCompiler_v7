#ifndef TP7_IDENTIFIER_HPP
#define TP7_IDENTIFIER_HPP

#include <string>
#include <utility>

namespace TP7
{

class Identifier
{
public:
    template<typename String>
    explicit Identifier(String &&i)
        : mIdent(std::forward(i)){}

    const std::string &identifier() const noexcept
        {return mIdent;}

private:
    std::string mIdent;
};

}

#endif