#ifndef TP4_PROCESSOR_HPP
#define TP4_PROCESSOR_HPP

#include <vector>
#include <unordered_map>

#include "../simbol.hpp"
#include "macro.hpp"

namespace TP4
{

class Processor
{
private:
    static std::unordered_map<std::string, Macro> MACRO_MAP;    

public:
    static bool execute(PPFile*,
                        std::vector<PPToken*>&);

private:
    Processor(PPFile*,
              std::vector<PPToken*>&) noexcept;

    void process();

    // helper function for process
    void proc(ControlLine*);
    void proc(Group*);
    void proc(GroupPart*);
    void proc(IfSection*);
    void proc(NonDirective*);
    void proc(PPFile*);
    void proc(TextLine*);

    // control-line helper
    void ctrlInclude(ControlLine*);
    void ctrlDefine(ControlLine*);
    void ctrlDefineIL(ControlLine*);
    void ctrlDefineV(ControlLine*);
    void ctrlDefineILV(ControlLine*);
    void ctrlUndef(ControlLine*);
    void ctrlLine(ControlLine*);
    void ctrlError(ControlLine*);
    void ctrlPragma(ControlLine*);
    void ctrl(ControlLine*);

    // helper for define
    void emplaceMacro(Macro&&);

    // if-section helper
    bool isEvaluated(PPTokens*);
    bool isEvaluated(Identifier*);

    // expand macro
    void expand(std::vector<PPToken*> &src,
                std::vector<PPToken*> &dst);
    void expand(std::vector<PPToken*> &src,
                std::vector<PPToken*> &dst,
                std::unordered_map<std::string, bool> &usedMap);
    // helper for macro
    bool isTarget(std::vector<PPToken*> &src,
                  std::size_t idx,
                  std::unordered_map<std::string, bool> &usedMap) const;
    bool expandObject(std::vector<PPToken*> &src,
                      std::size_t &idx,
                      std::vector<PPToken*> &dst,
                      std::unordered_map<std::string, bool> &usedMap);
    bool expandFunction(std::vector<PPToken*> &src,
                        std::size_t &idx,
                        std::vector<PPToken*> &dst,
                        std::unordered_map<std::string, bool> &usedMap);

    PPFile *mPPFile;
    std::vector<PPToken*> &mPtvec;
    bool mIsValid;
};

}

#endif