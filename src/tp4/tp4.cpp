#include "tokenizer.hpp"
#include "processor.hpp"
#include "tp4.hpp"

bool TP4::TP4::execute(std::vector<PPToken*> &ptvec)
{
    bool isValid = true;

    PPFile *ppFile;
    isValid = Tokenizer::execute(ptvec, ppFile);

    if(isValid)
        isValid = Processor::execute(ppFile, ptvec);
    
    return isValid;
}