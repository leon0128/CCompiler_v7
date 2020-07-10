#ifndef TP7_TOKENIZER_HPP
#define TP7_TOKENIZER_HPP

#include <vector>
#include "expression_simbol.hpp"

namespace TP7
{

class Tokenizer
{
public:
    static ConstantExpression *tokenizeConstantExpression(const std::vector<Token*>&,
        std::size_t&);

private:
    Tokenizer(const std::vector<Token*>&,
        std::size_t&);

    Expression *tokExpression();
    AssignmentExpression *tokAssignmentExpression();
    ConstantExpression *tokConstantExpression();
    ConditionalExpression *tokConditionalExpression();
    LogicalORExpression *tokLogicalORExpression();
    LogicalANDExpression *tokLogicalANDExpression();
    InclusiveORExpression *tokInclusiveORExpression();
    ExclusiveORExpression *tokExclusiveORExpression();
    ANDExpression *tokANDExpression();
    EqualityExpression *tokEqualityExpression();
    RelationalExpression *tokRelationalExpression();
    ShiftExpression *tokShiftExpression();
    AdditiveExpression *tokAdditiveExpression();
    MultiplicativeExpression *tokMultiplicativeExpression();
    CastExpression *tokCastExpression();
    UnaryExpression *tokUnaryExpression();
    PostfixExpression *tokPostfixExpression();
    PrimaryExpression *tokPrimaryExpression();

    const std::vector<Token*> &mTvec;
    std::size_t &mIdx;
};

}

#endif