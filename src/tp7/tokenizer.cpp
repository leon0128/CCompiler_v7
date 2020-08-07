#include "../simbol.hpp"
#include "expression_simbol.hpp"
#include "static_assert_simbol.hpp"
#include "tokenizer.hpp"

// namespace TP7
// {

// ConstantExpression *Tokenizer::tokenizeConstantExpression(const std::vector<Token*> &vec,
//     std::size_t &idx)
// {
//     Tokenizer tokenizer(vec, idx);
//     return tokenizer.tokConstantExpression();
// }

// StaticAssertDeclaration *Tokenizer::tokenizeStaticAssert(const std::vector<Token*> &vec
//     , std::size_t &idx)
// {
//     Tokenizer tokenizer(vec, idx);
//     return tokenizer.tokStaticAssert();
// }

// Tokenizer::Tokenizer(const std::vector<Token*> &vec,
//     std::size_t &idx):
//     mTvec(vec),
//     mIdx(idx)
// {
// }

// Expression *Tokenizer::tokExpression()
// {
//     std::vector<AssignmentExpression*> vec;

//     AssignmentExpression *assign = tokAssignmentExpression();
//     if(assign != nullptr)
//     {
//         vec.push_back(assign);

//         while(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::COMMA))
//         {
//             mIdx++;

//             if((assign = tokAssignmentExpression()) != nullptr)
//                 vec.push_back(assign);
//             else
//             {
//                 mIdx--;
//                 break;
//             }
//         }
//     }

//     if(!vec.empty())
//     {
//         Expression *retval = new Expression();
//         retval->vec = std::move(vec);
//         return retval;
//     }
//     else
//         return nullptr;
// }

// AssignmentExpression *Tokenizer::tokAssignmentExpression()
// {
//     AssignmentExpression retval;
//     std::size_t befidx = mIdx;

//     if((retval.uni.sAssignment.unary = tokUnaryExpression()) != nullptr)
//     {
//         if(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::ASS) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::MUL_ASS) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::DIV_ASS) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::REM_ASS) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::ADD_ASS) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::SUB_ASS) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::LSFT_ASS) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::RSFT_ASS) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::BW_AND_ASS) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::BW_INOR_ASS) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::BW_EXOR_ASS))
//         {
//             mIdx++;
//             retval.uni.sAssignment.punctuator = mTvec[mIdx - 1]->uni.punctuator;
            
//             if((retval.uni.sAssignment.assignment) != nullptr)
//                 retval.tag = AssignmentExpression::Tag::ASSIGNMENT;
//         }
//     }

//     if(retval.tag == AssignmentExpression::Tag::NONE)
//     {
//         mIdx = befidx;

//         if((retval.uni.conditional = tokConditionalExpression()) != nullptr)
//             retval.tag = AssignmentExpression::Tag::CONDITIONAL;
//     }

//     if(retval.tag != AssignmentExpression::Tag::NONE)
//         return new AssignmentExpression(retval);
//     else
//     {
//         mIdx = befidx;
//         return nullptr;
//     }
// }

// ConstantExpression *Tokenizer::tokConstantExpression()
// {
//     ConstantExpression retval;
//     if((retval.conditional = tokConditionalExpression()) != nullptr)
//         return new ConstantExpression(retval);
//     else
//         return nullptr;
// }

// ConditionalExpression *Tokenizer::tokConditionalExpression()
// {
//     ConditionalExpression retval;
//     std::size_t befidx = mIdx, mididx = mIdx;

//     if((retval.logicalOR = tokLogicalORExpression()) != nullptr)
//     {
//         mididx = mIdx;

//         if(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::QUE))
//         {
//             mIdx++;

//             if((retval.expression = tokExpression()) != nullptr)
//             {
//                 if(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::COLON))
//                 {
//                     mIdx++;
//                     retval.conditional = tokConditionalExpression();
//                 }
//             }
//         }
//     }

//     if(retval.logicalOR != nullptr)
//     {
//         if(retval.conditional == nullptr)
//             mIdx = mididx;
        
//         return new ConditionalExpression(retval);
//     }
//     else
//     {
//         mIdx = befidx;
//         return nullptr;
//     }
// }

// LogicalORExpression *Tokenizer::tokLogicalORExpression()
// {
//     std::vector<LogicalANDExpression*> vec;
//     LogicalANDExpression *logical = tokLogicalANDExpression();

//     if(logical != nullptr)
//     {
//         vec.push_back(logical);

//         while(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::OR))
//         {
//             mIdx++;

//             if((logical = tokLogicalANDExpression()) != nullptr)
//                 vec.push_back(logical);
//             else
//             {
//                 mIdx--;
//                 break;
//             }
//         }
//     }

//     if(!vec.empty())
//     {
//         LogicalORExpression *retval = new LogicalORExpression();
//         retval->vec = std::move(vec);
//         return retval;
//     }
//     else
//         return nullptr;
// }

// LogicalANDExpression *Tokenizer::tokLogicalANDExpression()
// {
//     std::vector<InclusiveORExpression*> vec;
//     InclusiveORExpression *inclusive = tokInclusiveORExpression();

//     if(inclusive != nullptr)
//     {
//         vec.push_back(inclusive);

//         while(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::AND))
//         {
//             mIdx++;
    
//             if((inclusive = tokInclusiveORExpression()) != nullptr)
//                 vec.push_back(inclusive);
//             else
//             {
//                 mIdx--;
//                 break;
//             }
//         }
//     }

//     if(!vec.empty())
//     {
//         LogicalANDExpression *retval = new LogicalANDExpression();
//         retval->vec = std::move(vec);
//         return retval;
//     }
//     else
//         return nullptr;
// }

// InclusiveORExpression *Tokenizer::tokInclusiveORExpression()
// {
//     std::vector<ExclusiveORExpression*> vec;
//     ExclusiveORExpression *exclusive = tokExclusiveORExpression();

//     if(exclusive != nullptr)
//     {
//         vec.push_back(exclusive);

//         while(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::IN_OR))
//         {
//             mIdx++;

//             if((exclusive = tokExclusiveORExpression()) != nullptr)
//                 vec.push_back(exclusive);
//             else
//             {
//                 mIdx--;
//                 break;
//             }
//         }
//     }

//     if(!vec.empty())
//     {
//         InclusiveORExpression *retval = new InclusiveORExpression();
//         retval->vec = std::move(vec);
//         return retval;
//     }
//     else
//         return nullptr;
// }

// ExclusiveORExpression *Tokenizer::tokExclusiveORExpression()
// {
//     std::vector<ANDExpression*> vec;
//     ANDExpression *expression = tokANDExpression();

//     if(expression != nullptr)
//     {
//         vec.push_back(expression);

//         while(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::EX_OR))
//         {
//             mIdx++;

//             if((expression = tokANDExpression()) != nullptr)
//                 vec.push_back(expression);
//             else
//             {
//                 mIdx--;
//                 break;
//             }
//         }
//     }

//     if(!vec.empty())
//     {
//         ExclusiveORExpression *retval = new ExclusiveORExpression();
//         retval->vec = std::move(vec);
//         return retval;
//     }
//     else
//         return nullptr;
// }

// ANDExpression *Tokenizer::tokANDExpression()
// {
//     std::vector<EqualityExpression*> vec;
//     EqualityExpression *expression = tokEqualityExpression();

//     if(expression != nullptr)
//     {
//         vec.push_back(expression);

//         while(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::AMP))
//         {
//             mIdx++;

//             if((expression = tokEqualityExpression()) != nullptr)
//                 vec.push_back(expression);
//             else
//             {
//                 mIdx--;
//                 break;
//             }
//         }
//     }

//     if(!vec.empty())
//     {
//         ANDExpression *retval = new ANDExpression();
//         retval->vec = std::move(vec);
//         return retval;
//     }
//     else
//         return nullptr;
// }

// EqualityExpression *Tokenizer::tokEqualityExpression()
// {
//     std::vector<RelationalExpression*> vec;
//     std::vector<Punctuator*> tagvec;
//     RelationalExpression *expression = tokRelationalExpression();

//     if(expression != nullptr)
//     {
//         vec.push_back(expression);

//         while(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::EQUAL) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::NOT_EQ))
//         {
//             mIdx++;
//             Punctuator *tag = mTvec[mIdx - 1]->uni.punctuator;

//             if((expression = tokRelationalExpression()) != nullptr)
//             {
//                 vec.push_back(expression);
//                 tagvec.push_back(tag);
//             }
//             else
//             {
//                 mIdx--;
//                 break;
//             }
//         }
//     }

//     if(!vec.empty())
//     {
//         EqualityExpression *retval = new EqualityExpression();
//         retval->relvec = std::move(vec);
//         retval->tagvec = std::move(tagvec);
//         return retval;
//     }
//     else
//         return nullptr;
// }

// RelationalExpression *Tokenizer::tokRelationalExpression()
// {
//     std::vector<ShiftExpression*> vec;
//     std::vector<Punctuator*> tagvec;
//     ShiftExpression *expression = tokShiftExpression();

//     if(expression != nullptr)
//     {
//         vec.push_back(expression);

//         while(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::LESS) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::GREATER) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::LESS_EQ) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::GREATER_EQ))
//         {
//             mIdx++;
//             Punctuator *tag = mTvec[mIdx - 1]->uni.punctuator;

//             if((expression = tokShiftExpression()) != nullptr)
//             {
//                 vec.push_back(expression);
//                 tagvec.push_back(tag);
//             }
//             else
//             {
//                 mIdx--;
//                 break;
//             }
//         }
//     }

//     if(!vec.empty())
//     {
//         RelationalExpression *retval = new RelationalExpression();
//         retval->sftvec = std::move(vec);
//         retval->tagvec = std::move(tagvec);
//         return retval;
//     }
//     else
//         return nullptr;
// }

// ShiftExpression *Tokenizer::tokShiftExpression()
// {
//     std::vector<AdditiveExpression*> vec;
//     std::vector<Punctuator*> tagvec;
//     AdditiveExpression *expression = tokAdditiveExpression();

//     if(expression != nullptr)
//     {
//         vec.push_back(expression);

//         while(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::L_SHIFT) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::R_SHIFT))
//         {
//             mIdx++;
//             Punctuator *tag = mTvec[mIdx - 1]->uni.punctuator;

//             if((expression = tokAdditiveExpression()) != nullptr)
//             {
//                 vec.push_back(expression);
//                 tagvec.push_back(tag);
//             }
//             else
//             {
//                 mIdx--;
//                 break;
//             }
//         }
//     }

//     if(!vec.empty())
//     {
//         ShiftExpression *retval = new ShiftExpression();
//         retval->addvec = std::move(vec);
//         retval->tagvec = std::move(tagvec);
//         return retval;
//     }
//     else
//         return nullptr;
// }

// AdditiveExpression *Tokenizer::tokAdditiveExpression()
// {
//     std::vector<MultiplicativeExpression*> vec;
//     std::vector<Punctuator*> tagvec;
//     MultiplicativeExpression *expression = tokMultiplicativeExpression();

//     if(expression != nullptr)
//     {
//         vec.push_back(expression);

//         while(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::PLUS) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::MINUS))
//         {
//             mIdx++;
//             Punctuator *tag = mTvec[mIdx - 1]->uni.punctuator;

//             if((expression = tokMultiplicativeExpression()) != nullptr)
//             {
//                 vec.push_back(expression);
//                 tagvec.push_back(tag);
//             }
//             else
//             {
//                 mIdx--;
//                 break;
//             }
//         }
//     }

//     if(!vec.empty())
//     {
//         AdditiveExpression *retval = new AdditiveExpression();
//         retval->mulvec = std::move(vec);
//         retval->tagvec = std::move(tagvec);
//         return retval;
//     }
//     else
//         return nullptr;
// }

// MultiplicativeExpression *Tokenizer::tokMultiplicativeExpression()
// {
//     std::vector<CastExpression*> vec;
//     std::vector<Punctuator*> tagvec;
//     CastExpression *expression = tokCastExpression();

//     if(expression != nullptr)
//     {
//         vec.push_back(expression);

//         while(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::AST) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::SLASH) ||
//             Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::PER))
//         {
//             mIdx++;
//             Punctuator *tag = mTvec[mIdx - 1]->uni.punctuator;

//             if((expression = tokCastExpression()) != nullptr)
//             {
//                 vec.push_back(expression);
//                 tagvec.push_back(tag);
//             }
//             else
//             {
//                 mIdx--;
//                 break;
//             }
//         }
//     }

//     if(!vec.empty())
//     {
//         MultiplicativeExpression *retval = new MultiplicativeExpression();
//         retval->castvec = std::move(vec);
//         retval->tagvec = std::move(tagvec);
//         return retval;
//     }
//     else
//         return nullptr;
// }

// CastExpression *Tokenizer::tokCastExpression()
// {
//     CastExpression retval;

//     if((retval.uni.unary = tokUnaryExpression()) != nullptr)
//         retval.tag = CastExpression::Tag::UNARY;

//     if(retval.tag != CastExpression::Tag::NONE)
//         return new CastExpression(retval);
//     else
//         return nullptr;
// }

// UnaryExpression *Tokenizer::tokUnaryExpression()
// {
//     UnaryExpression retval;
//     std::size_t befidx = mIdx;

//     if(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::INC))
//     {
//         mIdx++;
//         if((retval.uni.inc = tokUnaryExpression()) != nullptr)
//             retval.tag = UnaryExpression::Tag::INC;
//     }
//     else if(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::DEC))
//     {
//         mIdx++;
//         if((retval.uni.dec = tokUnaryExpression()) != nullptr)
//             retval.tag = UnaryExpression::Tag::DEC;
//     }
//     else if(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::AMP) ||
//         Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::AST) ||
//         Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::PLUS) ||
//         Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::MINUS) ||
//         Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::TILDE) ||
//         Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::EXC))
//     {
//         mIdx++;
//         retval.uni.sUnary.tag = mTvec[mIdx - 1]->uni.punctuator;
//         if((retval.uni.sUnary.cast = tokCastExpression()) != nullptr)
//             retval.tag = UnaryExpression::Tag::UNARY;
//     }
//     else if(Simbol::isMatch(mTvec, mIdx, Keyword::Tag::SIZEOF))
//     {
//         mIdx++;
//         if((retval.uni.sizeofUnary = tokUnaryExpression()) != nullptr)
//             retval.tag = UnaryExpression::Tag::SIZEOF_UNARY;
//     }
//     // sizeof (type-name)
//     // alignof (type-name)
//     else if((retval.uni.postfix = tokPostfixExpression()) != nullptr)
//         retval.tag = UnaryExpression::Tag::POSTFIX;

//     if(retval.tag != UnaryExpression::Tag::NONE)
//         return new UnaryExpression(retval);
//     else
//     {
//         mIdx = befidx;
//         return nullptr;
//     }
// }

// PostfixExpression *Tokenizer::tokPostfixExpression()
// {
//     PostfixExpression retval;

//     if((retval.uni.primary = tokPrimaryExpression()) != nullptr)
//         retval.tag = PostfixExpression::Tag::PRIMARY;
    
//     if(retval.tag != PostfixExpression::Tag::NONE)
//         return new PostfixExpression(retval);
//     else
//         return nullptr;
// }

// PrimaryExpression *Tokenizer::tokPrimaryExpression()
// {
//     PrimaryExpression retval;
//     std::size_t befidx = mIdx;

//     if(Simbol::isMatch(mTvec, mIdx, Token::Tag::IDENTIFIER))
//     {
//         mIdx++;
//         retval.uni.identifier = mTvec[mIdx - 1]->uni.identifier;
//         retval.tag = PrimaryExpression::Tag::IDENTIFIER;
//     }
//     else if(Simbol::isMatch(mTvec, mIdx, Token::Tag::CONSTANT))
//     {
//         mIdx++;
//         retval.uni.constant = mTvec[mIdx - 1]->uni.constant;
//         retval.tag = PrimaryExpression::Tag::CONSTANT;
//     }
//     else if(Simbol::isMatch(mTvec, mIdx, Token::Tag::STRING_LITERAL))
//     {
//         mIdx++;
//         retval.uni.stringLiteral = mTvec[mIdx - 1]->uni.stringLiteral;
//         retval.tag = PrimaryExpression::Tag::STRING_LITERAL;
//     }
//     else if(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::L_PAREN))
//     {
//         mIdx++;
//         if((retval.uni.expression = tokExpression()) != nullptr)
//         {
//             if(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::R_PAREN))
//             {
//                 mIdx++;
//                 retval.tag = PrimaryExpression::Tag::EXPRESSION;
//             }
//         }
//     }
//     // generic-selection

//     if(retval.tag != PrimaryExpression::Tag::NONE)
//         return new PrimaryExpression(retval);
//     else
//     {
//         mIdx = befidx;
//         return nullptr;
//     }
// }

// StaticAssertDeclaration *Tokenizer::tokStaticAssert()
// {
//     StaticAssertDeclaration retval;
//     std::size_t befidx = mIdx;
//     bool isValid = false;

//     if(Simbol::isMatch(mTvec, mIdx, Keyword::Tag::STATIC_ASSERT)
//         && Simbol::isMatch(mTvec, mIdx + 1, Punctuator::Tag::L_PAREN))
//     {
//         mIdx += 2;

//         if((retval.constant = tokConstantExpression()) != nullptr)
//         {
//             if(Simbol::isMatch(mTvec, mIdx, Punctuator::Tag::COMMA))
//             {
//                 mIdx++;

//                 if(Simbol::isMatch(mTvec, mIdx, Token::Tag::STRING_LITERAL)
//                     && Simbol::isMatch(mTvec, mIdx + 1, Punctuator::Tag::R_PAREN)
//                     && Simbol::isMatch(mTvec, mIdx + 2, Punctuator::Tag::SEMI_COL))
//                 {
//                     retval.str = mTvec[mIdx]->uni.stringLiteral;
//                     mIdx += 3;
//                     isValid = true;
//                 }
//             }
//         }
//     }

//     if(isValid)
//         return new StaticAssertDeclaration(retval);
//     else
//     {
//         mIdx = befidx;
//         return nullptr;
//     }
// }

// }