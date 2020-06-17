#ifndef TP4_PROCESSOR_HPP
#define TP4_PROCESSOR_HPP

#include <vector>

class PPFile;
class PPToken;

namespace TP4
{

class Processor
{
public:
    static bool execute(PPFile*&,
                        std::vector<PPToken*>&);

private:
    
};

}

#endif