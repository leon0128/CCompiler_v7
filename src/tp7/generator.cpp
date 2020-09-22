#include <iostream>

#include "../simbol.hpp"
#include "declaration_simbol.hpp"
#include "expression_simbol.hpp"
#include "function_definition_simbol.hpp"
#include "generator.hpp"

namespace TP7
{

const std::vector<std::string> Generator::ARGUMENT_REGISTERS
    = {"rdi"
        , "rsi"
        , "rdx"
        , "rcx"
        , "r8"
        , "r9"};

Generator::Generator() noexcept
    : mLabelIdx(0)
    , mLabelMap()
    , mStackIdx(0)
    , mStackMapVec()
    , mGlobalMap()
{
}

std::string Generator::generate(const TranslationUnit *tu)
{
    std::string ret;
    ret += "    .intel_syntax noprefix\n\n";

    for(auto &&ed : tu->edvec)
    {
        ret += generate(ed);
        ret.push_back('\n');
    }

    return ret;
}

std::string Generator::generate(const ExternalDeclaration *ed)
{
    std::string ret;

    switch(ed->tag)
    {
        case(ExternalDeclaration::Tag::FUNCTION_DEFINITION):
            ret += generate(ed->uni.functionDefinition);
            break;
        case(ExternalDeclaration::Tag::DECLARATION):
            ret += generate(ed->uni.declaration);
            break;
        
        default:
            Simbol::unexpectTag("ExternalDeclaration");
            break;
    }

    return ret;
}

std::string Generator::generate(const FunctionDefinition *fd)
{
    std::string prologue("        /* function prologue */\n");
    std::string contents("        /* function contents */\n");
    std::string epilogue("        /* function epilogue */\n");
    std::string functionName;

    functionName = getIdentifier(fd->declarator);
    mGlobalMap.emplace(functionName, GlobalTag::FUNCTION);

    // contents
    contents += generate(fd->compoundStatement);

    // prologue
    prologue += "    .text\n"
        "    .global ";
    prologue += functionName;
    prologue += "\n"
        "    .type ";
    prologue += functionName;
    prologue += " STT_FUNC\n";
    prologue += functionName;
    prologue += ":\n";
    prologue += generateInstruction("endbr64");
    prologue += generateInstruction("push", "rbp");
    prologue += generateInstruction("mov", "rbp", "rsp");
    prologue += generateInstruction("sub", "rsp", std::to_string(mStackIdx));

    // epilogue
    epilogue += generateInstruction("leave");
    epilogue += generateInstruction("ret");
    epilogue += "    .size ";
    epilogue += functionName;
    epilogue += ", .-";
    epilogue +=functionName;
    epilogue.push_back('\n');

    mLabelMap.clear();
    mStackIdx = 0;

    return prologue + contents + epilogue;
}

std::string Generator::generate(const Declaration *d)
{
    std::string ret;

    ret += "=== not implements (Declaration) ===\n";

    return ret;
}

std::string Generator::generate(const CompoundStatement *cs)
{
    std::string ret;

    mStackMapVec.emplace_back();

    if(cs->blockItemList != nullptr)
        ret += generate(cs->blockItemList);

    mStackMapVec.pop_back();

    return ret;
}

std::string Generator::generate(const BlockItemList *bil)
{
    std::string ret;

    for(auto &&bi : bil->bivec)
    {
        switch(bi->tag)
        {
            case(BlockItem::Tag::DECLARATION):
                ret += generate(bi->uni.declaration, 0);
                break;
            case(BlockItem::Tag::STATEMENT):
                ret += generate(bi->uni.statement);
                break;
            
            default:
                Simbol::unexpectTag("BlockItem");
                break;
        }
    }

    return ret;
}

std::string Generator::generate(const Statement *s)
{
    std::string ret;

    switch(s->tag)
    {
        case(Statement::Tag::LABELED):
            ret += generate(s->uni.labeled);
            break;
        case(Statement::Tag::COMPOUND):
            ret += generate(s->uni.compound);
            break;
        case(Statement::Tag::EXPRESSION):
            ret += generate(s->uni.expression);
            break;
        case(Statement::Tag::SELECTION):
            ret += generate(s->uni.selection);
            break;
        case(Statement::Tag::ITERATION):
            ret += generate(s->uni.iteration);
            break;
        case(Statement::Tag::JUMP):
            ret += generate(s->uni.jump);
            break;
        
        default:
            Simbol::unexpectTag("Statement");
            break;
    }

    return ret;
}

std::string Generator::generate(const LabeledStatement *ls)
{
    std::string ret;

    switch(ls->tag)
    {
        case(LabeledStatement::Tag::IDENTIFIER):
        {
            auto iter = mLabelMap.find(ls->uni.sIdentifier.identifier->str);
            if(iter == mLabelMap.end())
                iter = (mLabelMap.emplace(ls->uni.sIdentifier.identifier->str, mLabelIdx++)).first;
            
            ret += stringnizeLabel(iter->second);
            ret += ":\n";

            ret += generate(ls->uni.sIdentifier.statement);

            break;
        }

        default:
            Simbol::unexpectTag("LabeledStatement");
            break;
    }

    return ret;
}

std::string Generator::generate(const ExpressionStatement *es)
{
    std::string ret;

    if(es->expression != nullptr)
        ret += generate(es->expression);

    return ret;
}

std::string Generator::generate(const Expression *e)
{
    std::string ret;

    for(auto &&ae : e->vec)
        ret += generate(ae);
    
    return ret;
}

std::string Generator::generate(const AssignmentExpression *ae)
{
    std::string ret;

    switch(ae->tag)
    {
        case(AssignmentExpression::Tag::CONDITIONAL):
            ret += generate(ae->uni.conditional);
            break;
        case(AssignmentExpression::Tag::ASSIGNMENT):
        {
            ret += generate(ae->uni.sAssignment.assignment);

            switch(ae->uni.sAssignment.assignmentOperator->tag)
            {
                case(AssignmentOperator::Tag::ASS):
                    ret += generateInstruction("push", "rax");
                    ret += leadIdentifier(ae->uni.sAssignment.unary);
                    ret += generateInstruction("pop", "rdx");
                    ret += generateInstruction("mov", "QWORD PTR [rax]", "rdx");
                    ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
                    break;
                case(AssignmentOperator::Tag::MUL):
                    ret += generateInstruction("push", "rax");
                    ret += leadIdentifier(ae->uni.sAssignment.unary);
                    ret += generateInstruction("pop", "rdx");
                    ret += generateInstruction("imul", "rdx", "QWORD PTR [rax]");
                    ret += generateInstruction("mov", "QWORD PTR [rax]", "rdx");
                    ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
                    break;
                case(AssignmentOperator::Tag::DIV):
                    ret += generateInstruction("push", "rax");
                    ret += leadIdentifier(ae->uni.sAssignment.unary);
                    ret += generateInstruction("mov", "rdx", "rax");
                    ret += generateInstruction("mov", "rax", "QWORD PTR [rdx]");
                    ret += generateInstruction("pop", "rcx");
                    ret += generateInstruction("push", "rdx");
                    ret += generateInstruction("cqo");
                    ret += generateInstruction("idiv", "rcx");
                    ret += generateInstruction("pop", "rdx");
                    ret += generateInstruction("mov", "QWORD PTR [rdx]", "rax");
                    break;
                case(AssignmentOperator::Tag::REM):
                    ret += generateInstruction("push", "rax");
                    ret += leadIdentifier(ae->uni.sAssignment.unary);
                    ret += generateInstruction("mov", "rdx", "rax");
                    ret += generateInstruction("mov", "rax", "QWORD PTR [rdx]");
                    ret += generateInstruction("pop", "rcx");
                    ret += generateInstruction("push", "rdx");
                    ret += generateInstruction("cqo");
                    ret += generateInstruction("idiv", "rcx");
                    ret += generateInstruction("pop", "rax");
                    ret += generateInstruction("mov", "QWORD PTR [rax]", "rdx");
                    ret += generateInstruction("mov", "rax", "rdx");
                    break;
                case(AssignmentOperator::Tag::PLUS):
                    ret += generateInstruction("push", "rax");
                    ret += leadIdentifier(ae->uni.sAssignment.unary);
                    ret += generateInstruction("pop", "rdx");
                    ret += generateInstruction("add", "QWORD PTR [rax]", "rdx");
                    ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
                    break;
                case(AssignmentOperator::Tag::MINUS):
                    ret += generateInstruction("push", "rax");
                    ret += leadIdentifier(ae->uni.sAssignment.unary);
                    ret += generateInstruction("pop", "rdx");
                    ret += generateInstruction("sub", "QWORD PTR [rax]", "rdx");
                    ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
                    break;
                case(AssignmentOperator::Tag::LSFT):
                    ret += generateInstruction("push", "rax");
                    ret += leadIdentifier(ae->uni.sAssignment.unary);
                    ret += generateInstruction("pop", "rcx");
                    ret += generateInstruction("sal", "QWORD PTR [rax]", "cl");
                    ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
                    break;
                case(AssignmentOperator::Tag::RSFT):
                    ret += generateInstruction("push", "rax");
                    ret += leadIdentifier(ae->uni.sAssignment.unary);
                    ret += generateInstruction("pop", "rcx");
                    ret += generateInstruction("sar", "QWORD PTR [rax]", "cl");
                    ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
                    break;
                case(AssignmentOperator::Tag::AND):
                    ret += generateInstruction("push", "rax");
                    ret += leadIdentifier(ae->uni.sAssignment.unary);
                    ret += generateInstruction("pop", "rdx");
                    ret += generateInstruction("and", "QWORD PTR [rax]", "rdx");
                    ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
                    break;
                case(AssignmentOperator::Tag::EXOR):
                    ret += generateInstruction("push", "rax");
                    ret += leadIdentifier(ae->uni.sAssignment.unary);
                    ret += generateInstruction("pop", "rdx");
                    ret += generateInstruction("xor", "QWORD PTR [rax]", "rdx");
                    ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
                    break;
                case(AssignmentOperator::Tag::INOR):
                    ret += generateInstruction("push", "rax");
                    ret += leadIdentifier(ae->uni.sAssignment.unary);
                    ret += generateInstruction("pop", "rdx");
                    ret += generateInstruction("or", "QWORD PTR [rax]", "rdx");
                    ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
                    break;
                
                default:
                    Simbol::unexpectTag("AssignmentOperator");
                    break;
            }

            break;
        }

        default:
            Simbol::unexpectTag("AssignmentExpression");
            break;
    }

    return ret;
}

std::string Generator::generate(const ConditionalExpression *ce)
{
    std::string ret;

    if(ce->conditional == nullptr)
        ret += generate(ce->logicalOR);
    if(ce->conditional != nullptr)
    {
        std::size_t falseL = mLabelIdx++, endL = mLabelIdx++;

        ret += generate(ce->logicalOR);
        ret += generateInstruction("cmp", "rax", "0");
        ret += generateInstruction("je", stringnizeLabel(falseL));
        ret += generate(ce->expression);
        ret += generateInstruction("jmp", stringnizeLabel(endL));
        ret += stringnizeLabel(falseL);
        ret += ":\n";
        ret += generate(ce->conditional);
        ret += stringnizeLabel(endL);
        ret += ":\n";
    }
    
    return ret;
}

std::string Generator::generate(const LogicalORExpression *lore)
{
    std::string ret;

    if(lore->vec.size() == 1)
        ret += generate(lore->vec.front());
    else
    {
        std::size_t trueL = mLabelIdx++, falseL = mLabelIdx++, endL = mLabelIdx++;

        for(std::size_t i = 0; i < lore->vec.size(); i++)
        {
            ret += generate(lore->vec[i]);
            ret += generateInstruction("cmp", "rax", "0");

            if(i + 1 != lore->vec.size())
                ret += generateInstruction("jne", stringnizeLabel(trueL));
            else
                ret += generateInstruction("je", stringnizeLabel(falseL));
        }

        ret += stringnizeLabel(trueL);
        ret += ":\n";
        ret += generateInstruction("mov", "rax", "1");
        ret += generateInstruction("jmp", stringnizeLabel(endL));
        ret += stringnizeLabel(falseL);
        ret += ":\n";
        ret += generateInstruction("mov", "rax", "0");
        ret += stringnizeLabel(endL);
        ret += ":\n";
    }
    
    return ret;
}

std::string Generator::generate(const LogicalANDExpression *lande)
{
    std::string ret;

    if(lande->vec.size() == 1)
        ret += generate(lande->vec.front());
    else
    {
        std::size_t falseL = mLabelIdx++, endL = mLabelIdx++;

        for(std::size_t i = 0; i < lande->vec.size(); i++)
        {
            ret += generate(lande->vec[i]);
            ret += generateInstruction("cmp", "rax", "0");
            ret += generateInstruction("je", stringnizeLabel(falseL));
        }

        ret += generateInstruction("mov", "rax", "1");
        ret += generateInstruction("jmp", stringnizeLabel(endL));
        ret += stringnizeLabel(falseL);
        ret += ":\n";
        ret += generateInstruction("mov", "rax", "0");
        ret += stringnizeLabel(endL);
        ret += ":\n";
    }

    return ret;
}

std::string Generator::generate(const InclusiveORExpression *iore)
{
    std::string ret;

    if(iore->vec.size() == 1)
        ret += generate(iore->vec.front());
    else
    {
        ret += generate(iore->vec.front());

        for(std::size_t i = 1; i < iore->vec.size(); i++)
        {
            ret += generateInstruction("push", "rax");
            ret += generate(iore->vec[i]);
            ret += generateInstruction("pop", "rdx");
            ret += generateInstruction("or", "rax", "rdx");
        }
    }

    return ret;
}

std::string Generator::generate(const ExclusiveORExpression *eore)
{
    std::string ret;

    if(eore->vec.size() == 1)
        ret += generate(eore->vec.front());
    else
    {
        ret += generate(eore->vec.front());

        for(std::size_t i = 1; i < eore->vec.size(); i++)
        {
            ret += generateInstruction("push", "rax");
            ret += generate(eore->vec[i]);
            ret += generateInstruction("pop", "rdx");
            ret += generateInstruction("xor", "rax", "rdx");
        }
    }
    
    return ret;
}

std::string Generator::generate(const ANDExpression *ande)
{
    std::string ret;

    if(ande->vec.size() == 1)
        ret += generate(ande->vec.front());
    else
    {
        ret += generate(ande->vec.front());

        for(std::size_t i = 1; i < ande->vec.size(); i++)
        {
            ret += generateInstruction("push", "rax");
            ret += generate(ande->vec[i]);
            ret += generateInstruction("pop", "rdx");
            ret += generateInstruction("and", "rax", "rdx");
        }
    }

    return ret;
}

std::string Generator::generate(const EqualityExpression *ee)
{
    std::string ret;

    if(ee->seevec.size() == 1)
        ret += generate(ee->seevec.front().re);
    else
    {
        ret += generate(ee->seevec.front().re);

        for(std::size_t i = 1; i < ee->seevec.size(); i++)
        {
            ret += generateInstruction("push", "rax");
            ret += generate(ee->seevec[i].re);
            ret += generateInstruction("pop", "rdx");
            ret += generateInstruction("cmp", "rax", "rdx");
            
            switch(ee->seevec[i].tag)
            {
                case(EqualityExpression::Tag::EQUAL):
                    ret += generateInstruction("setl", "al");
                    break;
                case(EqualityExpression::Tag::NOT_EQUAL):
                    ret += generateInstruction("setne", "al");
                    break;
                
                default:
                    Simbol::unexpectTag("EqualityExpression");
                    break;
            }

            ret += generateInstruction("movzx", "rax", "al");
        }
    }
    
    return ret;
}

std::string Generator::generate(const RelationalExpression *re)
{
    std::string ret;

    if(re->srevec.size() == 1)
        ret += generate(re->srevec.front().se);
    else
    {
        ret += generate(re->srevec.front().se);

        for(std::size_t i = 1; i < re->srevec.size(); i++)
        {
            ret += generateInstruction("push", "rax");
            ret += generate(re->srevec[i].se);
            ret += generateInstruction("pop", "rdx");
            ret += generateInstruction("cmp", "rdx", "rax");
            
            switch(re->srevec[i].tag)
            {
                case(RelationalExpression::Tag::LESS):
                    ret += generateInstruction("setl", "al");
                    break;
                case(RelationalExpression::Tag::GREATER):
                    ret += generateInstruction("setg", "al");
                    break;
                case(RelationalExpression::Tag::LESS_EQ):
                    ret += generateInstruction("setle", "al");
                    break;
                case(RelationalExpression::Tag::GREATER_EQ):
                    ret += generateInstruction("setge", "al");
                    break;

                default:
                    Simbol::unexpectTag("RelationalExpression");
                    break;
            }

            ret += generateInstruction("movzx", "rax", "al");
        }
    }

    return ret;
}

std::string Generator::generate(const ShiftExpression *se)
{
    std::string ret;

    if(se->ssevec.size() == 1)
        ret += generate(se->ssevec.front().ae);
    else
    {
        ret += generate(se->ssevec.front().ae);

        for(std::size_t i = 1; i < se->ssevec.size(); i++)
        {
            ret += generateInstruction("push", "rax");
            ret += generate(se->ssevec[i].ae);
            ret += generateInstruction("mov", "rcx", "rax");
            ret += generateInstruction("pop", "rax");
            
            switch(se->ssevec[i].tag)
            {
                case(ShiftExpression::Tag::L_SHIFT):
                    ret += generateInstruction("sal", "rax", "cl");
                    break;
                case(ShiftExpression::Tag::R_SHIFT):
                    ret += generateInstruction("sar", "rax", "cl");
                    break;

                default:
                    Simbol::unexpectTag("ShiftExpression");
                    break;
            }
        }
    }
    
    return ret;
}

std::string Generator::generate(const AdditiveExpression *ae)
{
    std::string ret;

    if(ae->saevec.size() == 1)
        ret += generate(ae->saevec.front().me);
    else
    {
        ret += generate(ae->saevec.front().me);

        for(std::size_t i = 1; i < ae->saevec.size(); i++)
        {
            ret += generateInstruction("push", "rax");
            ret += generate(ae->saevec[i].me);
            ret += generateInstruction("mov", "rdx", "rax");
            ret += generateInstruction("pop", "rax");

            switch(ae->saevec[i].tag)
            {
                case(AdditiveExpression::Tag::ADD):
                    ret += generateInstruction("add", "rax", "rdx");
                    break;
                case(AdditiveExpression::Tag::SUB):
                    ret += generateInstruction("sub", "rax", "rdx");
                    break;
                
                default:
                    Simbol::unexpectTag("AdditiveExpression");
                    break;
            }
        }
    }
    
    return ret;
}

std::string Generator::generate(const MultiplicativeExpression *me)
{
    std::string ret;

    if(me->smevec.size() == 1)
        ret += generate(me->smevec.front().ce);
    else
    {
        ret += generate(me->smevec.front().ce);

        for(std::size_t i = 1; i < me->smevec.size(); i++)
        {
            ret += generateInstruction("push", "rax");
            ret += generate(me->smevec[i].ce);
            ret += generateInstruction("mov", "rcx", "rax");
            ret += generateInstruction("pop", "rax");
            
            switch(me->smevec[i].tag)
            {
                case(MultiplicativeExpression::Tag::MUL):
                    ret += generateInstruction("imul", "rcx");
                    break;
                case(MultiplicativeExpression::Tag::DIV):
                    ret += generateInstruction("cqo");
                    ret += generateInstruction("idiv", "rcx");
                    break;
                case(MultiplicativeExpression::Tag::REM):
                    ret += generateInstruction("cqo");
                    ret += generateInstruction("idiv", "rcx");
                    ret += generateInstruction("mov", "rax", "rdx");
                    break;
                
                default:
                    Simbol::unexpectTag("MultiplicativeExpression");
                    break;
            }
        }
    }
    
    return ret;
}

std::string Generator::generate(const CastExpression *ce)
{
    std::string ret;

    switch(ce->tag)
    {
        case(CastExpression::Tag::UNARY):
            ret += generate(ce->uni.unary);
            break;
        case(CastExpression::Tag::CAST):
            ret += generate(ce->uni.sCast.cast);
            break;
        
        default:
            Simbol::unexpectTag("CastExpression");
            break;
    }

    return ret;
}

std::string Generator::generate(const UnaryExpression *ue)
{
    std::string ret;

    switch(ue->tag)
    {
        case(UnaryExpression::Tag::POSTFIX):
            ret += generate(ue->uni.postfix);
            break;
        case(UnaryExpression::Tag::INC):
            ret += leadIdentifier(ue->uni.inc);
            ret += generateInstruction("add", "QWORD PTR [rax]", "1");
            ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
            break;
        case(UnaryExpression::Tag::DEC):
            ret += leadIdentifier(ue->uni.dec);
            ret += generateInstruction("sub", "QWORD PTR [rax]", "1");
            ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
            break;
        case(UnaryExpression::Tag::UNARY):
            switch(ue->uni.sUnary.unaryOperator->tag)
            {
                case(UnaryOperator::Tag::AND):
                    ret += leadIdentifier(ue->uni.sUnary.cast);
                    break;
                case(UnaryOperator::Tag::AST):
                    ret += generate(ue->uni.sUnary.cast);
                    ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
                    break;
                case(UnaryOperator::Tag::PLUS):
                    ret += generate(ue->uni.sUnary.cast);
                    break;
                case(UnaryOperator::Tag::MINUS):
                    ret += generate(ue->uni.sUnary.cast);
                    ret += generateInstruction("neg", "rax");
                    break;
                case(UnaryOperator::Tag::TILDE):
                    ret += generate(ue->uni.sUnary.cast);
                    ret += generateInstruction("not", "rax");
                    break;
                case(UnaryOperator::Tag::EXC):
                    ret += generate(ue->uni.sUnary.cast);
                    ret += generateInstruction("cmp", "rax", "0");
                    ret += generateInstruction("sete", "al");
                    ret += generateInstruction("movzx", "rax", "al");
                    break;
                
                default:
                    Simbol::unexpectTag("UnaryOperator");
                    break;
            }

            break;
        case(UnaryExpression::Tag::SIZEOF_UNARY):
            ret += "=== not implements (UnaryExpression) ===\n";
            break;
        case(UnaryExpression::Tag::SIZEOF_TYPE):
            ret += "=== not implements (UnaryExpression) ===\n";
            break;
        case(UnaryExpression::Tag::ALIGNOF):
            ret += "=== not implements (UnaryExpression) ===\n";
            break;
        
        default:
            Simbol::unexpectTag("UnaryExpression");
            break;
    }

    return ret;
}

std::string Generator::generate(const PostfixExpression *pe)
{
    std::string ret;

    for(auto &&spe : pe->spevec)
    {
        switch(spe.tag)
        {
            case(PostfixExpression::Tag::PRIMARY):
                ret += generate(spe.uni.primary);
                break;
            case(PostfixExpression::Tag::EXPRESSION):
                ret += generateInstruction("push", "rax");
                ret += generate(spe.uni.expression);
                ret += generateInstruction("lea", "rdx", "[0+rax*8]");
                ret += generateInstruction("pop", "rax");
                ret += generateInstruction("add", "rax", "rdx");
                ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
                break;
            case(PostfixExpression::Tag::ARGUMENT):
            {
                if(spe.uni.argument != nullptr)
                {
                    ret += generateInstruction("push", "rax");
                    ret += generate(spe.uni.argument);

                    for(std::size_t i = 0; i < spe.uni.argument->aevec.size(); i++)
                    {
                        if(i < ARGUMENT_REGISTERS.size())
                            ret += generateInstruction("pop", ARGUMENT_REGISTERS[i]);
                        else
                            break;
                    }

                    std::string tmp;
                    if(spe.uni.argument->aevec.size() > ARGUMENT_REGISTERS.size())
                    {
                        tmp = "QWORD PTR [rsp+";
                        tmp += std::to_string((spe.uni.argument->aevec.size() - ARGUMENT_REGISTERS.size()) * 8);
                        tmp.push_back(']');
                    }
                    else
                        tmp = "QWORD PTR [rsp]";
                    
                    ret += generateInstruction("mov", "rax", tmp);
                }
                
                ret += generateInstruction("call", "rax");

                if(spe.uni.argument != nullptr)
                {
                    std::size_t size = 1;

                    if(spe.uni.argument->aevec.size() > ARGUMENT_REGISTERS.size())
                        size += spe.uni.argument->aevec.size() - ARGUMENT_REGISTERS.size();
                
                    ret += generateInstruction("add", "rsp", std::to_string(size * 8));
                }

                break;
            }

            default:
                Simbol::unexpectTag("PostfixExpression");
                break;
        }
    }

    return ret;
}

std::string Generator::generate(const PrimaryExpression *pe)
{
    std::string ret;

    switch(pe->tag)
    {
        case(PrimaryExpression::Tag::IDENTIFIER):
            ret += generate(pe->uni.identifier);
            break;
        case(PrimaryExpression::Tag::CONSTANT):
            ret += generate(pe->uni.constant);
            break;
        case(PrimaryExpression::Tag::STRING_LITERAL):
            ret += generate(pe->uni.stringLiteral);
            break;
        case(PrimaryExpression::Tag::EXPRESSION):
            ret += generate(pe->uni.expression);
            break;
        case(PrimaryExpression::Tag::GENERIC_SELECTION):
            ret += generate(pe->uni.genericSelection);
            break;
        
        default:
            Simbol::unexpectTag("PrimaryExpression");
            break;
    }

    return ret;
}

std::string Generator::generate(const ::Identifier *i)
{
    std::string ret;

    ret += stringnizeIdentifier(i->str);
    
    bool isFunction = false;
    bool isFinded = false;
    for(auto &&iter = mStackMapVec.rbegin(); iter != mStackMapVec.rend(); iter++)
    {
        auto &&it = iter->find(i->str);
        if((isFinded = it != iter->end()))
            break;
    }
    if(!isFinded)
    {
        auto &&it = mGlobalMap.find(i->str);
        if(it != mGlobalMap.end())
        {
            switch(it->second)
            {
                case(GlobalTag::FUNCTION):
                    isFunction = true;
                    break;
                case(GlobalTag::OBJECT):
                    isFunction = false;
                    break;
            }
        }
    }

    if(!isFunction)
        ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");

    return ret;
}

std::string Generator::generate(const ::Constant *c)
{
    std::string ret;

    switch(c->tag)
    {
        case(::Constant::Tag::INTEGER):
            ret += generate(c->uni.integer);
            break;
        case(::Constant::Tag::FLOATING):
            ret += generate(c->uni.floating);
            break;
        case(::Constant::Tag::ENUMERATION):
            ret += generate(c->uni.enumeration);
            break;
        case(::Constant::Tag::CHARACTER):
            ret += generate(c->uni.character);
            break;
        
        default:
            Simbol::unexpectTag("Constant");
            break;
    }
    
    return ret;
}

std::string Generator::generate(const ::IntegerConstant *ic)
{
    std::string ret;

    std::string prefix;
    switch(ic->prefixTag)
    {
        case(::IntegerConstant::PrefixTag::DECIMAL):
            break;
        case(::IntegerConstant::PrefixTag::OCTAL):
            prefix.push_back('0');
            break;
        case(::IntegerConstant::PrefixTag::HEXADECIMAL):
            prefix += "0x";
            break;
        
        default:
            Simbol::unexpectTag("IntegerConstant");
            break;
    }

    ret += generateInstruction("mov", "rax", prefix + ic->str);

    return ret;
}

std::string Generator::generate(const ::FloatingConstant *fc)
{
    std::string ret;

    ret += "=== not implements (FloatingConstant)===\n";

    return ret;
}

std::string Generator::generate(const ::EnumerationConstant *ec)
{
    std::string ret;

    ret += "=== not implements (EnumerationConstant)===\n";

    return ret;
}

std::string Generator::generate(const ::CharacterConstant *cc)
{
    std::string ret;

    ret += "=== not implements (CharacterConstant)===\n";

    return ret;
}

std::string Generator::generate(const StringLiteral *sl)
{
    std::string ret;

    ret += "=== not implements (StringLiteral)===\n";
    
    return ret;
}

std::string Generator::generate(const GenericSelection *gs)
{
    std::string ret;

    ret += "=== not implements (GenericSelection)===\n";

    return ret;
}

std::string Generator::generate(const SelectionStatement *ss)
{
    std::string ret;

    switch(ss->tag)
    {
        case(SelectionStatement::Tag::IF):
        {
            std::size_t endL = mLabelIdx++;

            ret += generate(ss->uni.sIf.expression);
            ret += generateInstruction("cmp", "rax", "0");
            ret += generateInstruction("je", stringnizeLabel(endL));
            ret += generate(ss->uni.sIf.statement);
            ret += stringnizeLabel(endL);
            ret += ":\n";

            break;
        }
        case(SelectionStatement::Tag::IF_ELSE):
        {
            std::size_t falseL = mLabelIdx++, endL = mLabelIdx++;

            ret += generate(ss->uni.sIfElse.expression);
            ret += generateInstruction("cmp", "rax", "0");
            ret += generateInstruction("je", stringnizeLabel(falseL));
            ret += generate(ss->uni.sIfElse.ifStatement);
            ret += generateInstruction("jmp", stringnizeLabel(endL));
            ret += stringnizeLabel(falseL);
            ret += ":\n";
            ret += generate(ss->uni.sIfElse.elseStatement);
            ret += stringnizeLabel(endL);
            ret += ":\n";

            break;
        }

        default:
            Simbol::unexpectTag("SeletionStatement");
            break;
    }

    return ret;
}

std::string Generator::generate(const IterationStatement *is)
{
    std::string ret;

    switch(is->tag)
    {
        case(IterationStatement::Tag::WHILE):
        {
            std::size_t procL = mLabelIdx++, compL = mLabelIdx++;

            ret += generateInstruction("jmp", stringnizeLabel(compL));
            ret += stringnizeLabel(procL);
            ret += ":\n";
            ret += generate(is->uni.sWhile.statement);
            ret += stringnizeLabel(compL);
            ret += ":\n";
            ret += generate(is->uni.sWhile.expression);
            ret += generateInstruction("cmp", "rax", "0");
            ret += generateInstruction("jne", stringnizeLabel(procL));

            break;
        }
        case(IterationStatement::Tag::DO_WHILE):
        {
            std::size_t procL = mLabelIdx++;

            ret += stringnizeLabel(procL);
            ret += ":\n";
            ret += generate(is->uni.sDoWhile.statement);
            ret += generate(is->uni.sDoWhile.expression);
            ret += generateInstruction("cmp", "rax", "0");
            ret += generateInstruction("jne", stringnizeLabel(procL));

            break;
        }
        case(IterationStatement::Tag::FOR_EXPRESSION):
        {
            std::size_t procL = mLabelIdx++, compL = mLabelIdx++;

            ret += generate(is->uni.sForExpression.expressionArr[0]);
            ret += generateInstruction("jmp", stringnizeLabel(compL));
            ret += stringnizeLabel(procL);
            ret += ":\n";
            ret += generate(is->uni.sForExpression.statement);
            ret += generate(is->uni.sForExpression.expressionArr[2]);
            ret += stringnizeLabel(compL);
            ret += ":\n";
            ret += generate(is->uni.sForExpression.expressionArr[1]);
            ret += generateInstruction("cmp", "rax", "0");
            ret += generateInstruction("jne", stringnizeLabel(procL));
            
            break;
        }

        default:
            Simbol::unexpectTag("IterationStatement");
            break;
    }

    return ret;
}

std::string Generator::generate(const JumpStatement *js)
{
    std::string ret;

    switch(js->tag)
    {
        case(JumpStatement::Tag::GOTO):
        {
            auto iter = mLabelMap.find(js->uni.gotoIdentifier->str);
            if(iter == mLabelMap.end())
                iter = (mLabelMap.emplace(js->uni.gotoIdentifier->str, mLabelIdx++)).first;

            ret += generateInstruction("jmp", stringnizeLabel(iter->second));
            
            break;
        }
        case(JumpStatement::Tag::RETURN):
            if(js->uni.returnExpression != nullptr)
                ret += generate(js->uni.returnExpression);

            ret += generateInstruction("leave");
            ret += generateInstruction("ret");
            
            break;

        default:
            Simbol::unexpectTag("JumpStatement");
            break;
    }

    return ret;
}

std::string Generator::generate(const Declaration *d, int)
{
    std::string ret;

    switch(d->tag)
    {
        case(Declaration::Tag::DECLARATION):
            // declaration-specifiers
            if(d->uni.sDeclaration.initDeclaratorList != nullptr)
                ret += generate(d->uni.sDeclaration.initDeclaratorList, 0);
            break;
        
        case(Declaration::Tag::STATIC_ASSERT):
            ret += generate(d->uni.staticAssert);
            break;
        
        default:
            Simbol::unexpectTag("Declaration");
            break;
    }

    return ret;
}

std::string Generator::generate(const InitDeclaratorList *idl, int)
{
    std::string ret;

    for(auto &&id : idl->idvec)
        ret += generate(id, 0);

    return ret;
}

std::string Generator::generate(const InitDeclarator *id, int)
{
    std::string ret;

    ret += generate(id->declarator, 0);

    if(id->initializer != nullptr)
    {
        ret += generate(id->initializer, 0);
        ret += generateInstruction("mov", "rdx", "rax");
        ret += stringnizeIdentifier(getIdentifier(id->declarator));
        ret += generateInstruction("mov", "QWORD PTR [rax]", "rdx");
    }

    return ret;
}

std::string Generator::generate(const Declarator *d, int)
{
    std::string ret;

    // pointer

    ret += generate(d->directDeclarator, 0);

    return ret;
}

std::string Generator::generate(const Initializer *i, int)
{
    std::string ret;

    switch(i->tag)
    {
        case(Initializer::Tag::ASSIGNMENT):
            ret += generate(i->uni.assignment);
            break;
        
        default:
            Simbol::unexpectTag("Initializer");
            break;
    }

    return ret;
}

std::string Generator::generate(const DirectDeclarator *dd, int)
{
    std::string ret;

    for(auto &&sdd : dd->sddvec)
    {
        switch(sdd.tag)
        {
            case(DirectDeclarator::Tag::IDENTIFIER):
                mStackMapVec.back().emplace(sdd.uni.identifier->str, mStackIdx += 8);
                break;
            case(DirectDeclarator::Tag::DECLARATOR):
                ret += generate(sdd.uni.declarator, 0);
                break;
            
            default:
                Simbol::unexpectTag("DirectDeclarator");
                break;
        }
    }

    return ret;
}

std::string Generator::generate(const StaticAssertDeclaration *sad)
{
    std::string ret;

    ret += "=== not implements (StaticAssertDeclaration) ===\n";

    return ret;
}

std::string Generator::generate(const ArgumentExpressionList *ael)
{
    std::string ret;

    for(auto iter = ael->aevec.rbegin(); iter != ael->aevec.rend(); iter++)
    {
        ret += generate(*iter);
        generateInstruction("push", "rax");
    }

    return ret;
}

std::string Generator::getIdentifier(const Declarator *d) const
{
    std::string ret;

    switch(d->directDeclarator->sddvec.front().tag)
    {
        case(DirectDeclarator::Tag::IDENTIFIER):
            ret = d->directDeclarator->sddvec.front().uni.identifier->str;
            break;
        case(DirectDeclarator::Tag::DECLARATOR):
            ret = getIdentifier(d->directDeclarator->sddvec.front().uni.declarator);
            break;
        
        default:
            Simbol::unexpectTag("DirectDeclarator");
            break;
    }

    return ret;
}

std::string Generator::getIdentifier(const UnaryExpression *ue) const
{
    std::string ret;

    switch(ue->tag)
    {
        case(UnaryExpression::Tag::POSTFIX):
            ret = getIdentifier(ue->uni.postfix);
            break;
        case(UnaryExpression::Tag::INC):
            ret = getIdentifier(ue->uni.inc);
            break;
        case(UnaryExpression::Tag::DEC):
            ret = getIdentifier(ue->uni.dec);
            break;
        case(UnaryExpression::Tag::UNARY):
            ret = getIdentifier(ue->uni.sUnary.cast);
            break;
        
        default:
            Simbol::unexpectTag("UnaryExpression");
            break;
    }

    return ret;
}

std::string Generator::getIdentifier(const PostfixExpression *pe) const
{
    std::string ret;

    switch(pe->spevec.front().tag)
    {
        case(PostfixExpression::Tag::PRIMARY):
            ret = getIdentifier(pe->spevec.front().uni.primary);
            break;
        
        default:
            Simbol::unexpectTag("PostfixExpression");
            break;
    }

    return ret;
}

std::string Generator::getIdentifier(const PrimaryExpression *pe) const
{
    std::string ret;

    switch(pe->tag)
    {
        case(PrimaryExpression::Tag::IDENTIFIER):
            ret = pe->uni.identifier->str;
            break;
        
        case(PrimaryExpression::Tag::EXPRESSION):
            ret = getIdentifier(pe->uni.expression);
            break;
        
        default:
            Simbol::unexpectTag("PrimaryExpression");
            break;
    }

    return ret;
}

std::string Generator::getIdentifier(const Expression *e) const
{
    return getIdentifier(e->vec.front());
}

std::string Generator::getIdentifier(const AssignmentExpression *ae) const
{
    std::string ret;

    switch(ae->tag)
    {
        case(AssignmentExpression::Tag::CONDITIONAL):
            ret = getIdentifier(ae->uni.conditional);
            break;
        
        default:
            Simbol::unexpectTag("AssignmentExpression");
            break;
    }
    
    return ret;
}

std::string Generator::getIdentifier(const ConditionalExpression* ce) const
{
    return getIdentifier(ce->logicalOR);
}

std::string Generator::getIdentifier(const LogicalORExpression *lore) const
{
    return getIdentifier(lore->vec.front());
}

std::string Generator::getIdentifier(const LogicalANDExpression *lande) const
{
    return getIdentifier(lande->vec.front());
}

std::string Generator::getIdentifier(const InclusiveORExpression *iore) const
{
    return getIdentifier(iore->vec.front());
}

std::string Generator::getIdentifier(const ExclusiveORExpression *eore) const
{
    return getIdentifier(eore->vec.front());
}

std::string Generator::getIdentifier(const ANDExpression *ande) const
{
    return getIdentifier(ande->vec.front());
}

std::string Generator::getIdentifier(const EqualityExpression *ee) const
{
    return getIdentifier(ee->seevec.front().re);
}

std::string Generator::getIdentifier(const RelationalExpression *re) const
{
    return getIdentifier(re->srevec.front().se);
}

std::string Generator::getIdentifier(const ShiftExpression *se) const
{
    return getIdentifier(se->ssevec.front().ae);
}

std::string Generator::getIdentifier(const AdditiveExpression *ae) const
{
    return getIdentifier(ae->saevec.front().me);
}

std::string Generator::getIdentifier(const MultiplicativeExpression *me) const
{
    return getIdentifier(me->smevec.front().ce);
}

std::string Generator::getIdentifier(const CastExpression *ce) const
{
    std::string ret;

    switch(ce->tag)
    {
        case(CastExpression::Tag::UNARY):
            ret = getIdentifier(ce->uni.unary);
            break;
        
        default:
            Simbol::unexpectTag("CastExpression");
            break;
    }

    return ret;
}

std::string Generator::leadIdentifier(const Expression *e)
{
    return leadIdentifier(e->vec.back());
}

std::string Generator::leadIdentifier(const AssignmentExpression *ae)
{
    std::string ret;

    switch(ae->tag)
    {
        case(AssignmentExpression::Tag::CONDITIONAL):
            ret += leadIdentifier(ae->uni.conditional);
            break;

        default:
            Simbol::unexpectTag("AssignmentExpression");
            break;
    }

    return ret;
}

std::string Generator::leadIdentifier(const ConditionalExpression *ce)
{
    return leadIdentifier(ce->logicalOR);
}

std::string Generator::leadIdentifier(const LogicalORExpression *lore)
{
    return leadIdentifier(lore->vec.front());
}

std::string Generator::leadIdentifier(const LogicalANDExpression *lande)
{
    return leadIdentifier(lande->vec.front());
}

std::string Generator::leadIdentifier(const InclusiveORExpression *iore)
{
    return leadIdentifier(iore->vec.front());
}

std::string Generator::leadIdentifier(const ExclusiveORExpression *eore)
{
    return leadIdentifier(eore->vec.front());
}

std::string Generator::leadIdentifier(const ANDExpression *ande)
{
    return leadIdentifier(ande->vec.front());
}

std::string Generator::leadIdentifier(const EqualityExpression *ee)
{
    return leadIdentifier(ee->seevec.front().re);
}

std::string Generator::leadIdentifier(const RelationalExpression *re)
{
    return leadIdentifier(re->srevec.front().se);
}

std::string Generator::leadIdentifier(const ShiftExpression *se)
{
    return leadIdentifier(se->ssevec.front().ae);
}

std::string Generator::leadIdentifier(const AdditiveExpression *ae)
{
    return leadIdentifier(ae->saevec.front().me);
}

std::string Generator::leadIdentifier(const MultiplicativeExpression *me)
{
    return leadIdentifier(me->smevec.front().ce);
}

std::string Generator::leadIdentifier(const CastExpression *ce)
{
    std::string ret;

    switch(ce->tag)
    {
        case(CastExpression::Tag::UNARY):
            ret += leadIdentifier(ce->uni.unary);
            break;
        
        default:
            Simbol::unexpectTag("CastExpression");
            break;
    }

    return ret;
}

std::string Generator::leadIdentifier(const UnaryExpression *ue)
{
    std::string ret;

    switch(ue->tag)
    {
        case(UnaryExpression::Tag::POSTFIX):
            ret += leadIdentifier(ue->uni.postfix);
            break;
        
        case(UnaryExpression::Tag::UNARY):
        {
            switch(ue->uni.sUnary.unaryOperator->tag)
            {
                case(UnaryOperator::Tag::AST):
                    ret += leadIdentifier(ue->uni.sUnary.cast);
                    ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
            
                default:
                    Simbol::unexpectTag("UnaryOperator");
                    break;
            }

            break;
        }

        default:
            Simbol::unexpectTag("UnaryExpression");
            break;
    }

    return ret;
}

std::string Generator::leadIdentifier(const PostfixExpression *pe)
{
    std::string ret;

    for(auto &&spe : pe->spevec)
    {
        switch(spe.tag)
        {
            case(PostfixExpression::Tag::PRIMARY):
                ret += leadIdentifier(spe.uni.primary);
                break;
            case(PostfixExpression::Tag::EXPRESSION):
                ret += generateInstruction("push", "rax");
                ret += generate(spe.uni.expression);
                ret += generateInstruction("lea", "rdx", "[0+rax*8]");
                ret += generateInstruction("pop", "rax");
                ret += generateInstruction("mov", "rax", "QWORD PTR [rax]");
                ret += generateInstruction("add", "rax", "rdx");
                break;
            
            default:
                Simbol::unexpectTag("PostfixExpression");
                break;
        }
    }

    return ret;
}

std::string Generator::leadIdentifier(const PrimaryExpression* pe)
{
    std::string ret;

    switch(pe->tag)
    {
        case(PrimaryExpression::Tag::IDENTIFIER):
            ret += generateInstruction("lea", "rax", getIdentifierPosition(pe->uni.identifier->str));
            break;
        case(PrimaryExpression::Tag::EXPRESSION):
            ret += leadIdentifier(pe->uni.expression);
            break;
        
        default:
            Simbol::unexpectTag("PrimaryExpression");
            break;
    }

    return ret;
}

std::string Generator::stringnizeLabel(std::size_t idx) const noexcept
{
    return ".L_" + std::to_string(idx);
}

std::string Generator::stringnizeIdentifier(const std::string &identifier) const
{
    std::string pos;

    bool isFinded = false;
    for(auto riter = mStackMapVec.rbegin(); riter != mStackMapVec.rend(); riter++)
    {
        auto iter = riter->find(identifier);
        if((isFinded = iter != riter->end()))
        {
            pos += "[rbp-";
            pos += std::to_string(iter->second);
            pos.push_back(']');
            break;
        }
    }
    if(!isFinded)
    {
        pos += identifier;
        pos += "[rip]";
    }

    return generateInstruction("lea", "rax", pos);
}

std::string Generator::getIdentifierPosition(const std::string &ident) const
{
    std::string pos;

    bool isFinded = false;
    for(auto riter = mStackMapVec.rbegin(); riter != mStackMapVec.rend(); riter++)
    {
        auto iter = riter->find(ident);
        if((isFinded = iter != riter->end()))
        {
            pos += "[rbp-";
            pos += std::to_string(iter->second);
            pos.push_back(']');
            break;
        }
    }
    if(!isFinded)
    {
        auto iter = mGlobalMap.find(ident);
        if(iter != mGlobalMap.end())
        {
            switch(iter->second)
            {
                case(GlobalTag::FUNCTION):
                    pos += "OFFSET FLAT:";
                    pos += iter->first;
                    break;
                case(GlobalTag::OBJECT):
                    pos += iter->first;
                    pos += "[rip]";
                    break;
            }
        }
    }

    return pos;
}

std::string Generator::generateInstruction(const std::string &inst
    , const std::string &ope1
    , const std::string &ope2
    , const std::string &ope3
    , const std::string &ope4) const
{
    std::string ret = "    ";
    ret += inst;
    if(inst.size() < 7)
        ret += std::string(7 - inst.size(), ' ');

    if(!ope1.empty())
    {
        ret.push_back(' ');
        ret += ope1;
    }

    if(!ope2.empty())
    {
        ret += ", ";
        ret += ope2;
    }

    if(!ope3.empty())
    {
        ret += ", ";
        ret += ope3;
    }

    if(!ope4.empty())
    {
        ret += ", ";
        ret += ope4;
    }

    ret.push_back('\n');

    return ret;
}

}