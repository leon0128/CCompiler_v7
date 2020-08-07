#include <iostream>
#include <limits>

#include "calculator.hpp"

// namespace TP7
// {

// ArithmeticType Calculator::calculateConstantExpression(ConstantExpression *constantExpression)
// {
//     return calc(constantExpression);
// }

// ArithmeticType Calculator::calc(Expression *expression)
// {
//     if(expression->vec.size() == 1)
//         return calc(expression->vec.front());
//     else
//     {
//         std::cout << "TP7 Calculator error:\n"
//             "    what: constant-expression cannot include comma operators.\n"
//             << std::flush;
//         return ArithmeticType();
//     }
// }

// ArithmeticType Calculator::calc(AssignmentExpression *assignmentExpression)
// {
//     if(assignmentExpression->tag == AssignmentExpression::Tag::CONDITIONAL)
//         return calc(assignmentExpression->uni.conditional);
//     else
//     {
//         std::cout << "TP7 Calculator error:\n"
//             "    what: constant-expression cannot include assignment-expression.\n"
//             << std::endl;
//         return ArithmeticType();
//     }
// }

// ArithmeticType Calculator::calc(ConstantExpression *constantExpression)
// {
//     return calc(constantExpression->conditional);
// }

// ArithmeticType Calculator::calc(ConditionalExpression *conditionalExpression)
// {
//     ArithmeticType logicalOR = calc(conditionalExpression->logicalOR);

//     if(conditionalExpression->conditional != nullptr)
//         return logicalOR.tag != ArithmeticType::Tag::NONE
//             ? logicalOR.isNonzero()
//                 ? calc(conditionalExpression->expression)
//                     : calc(conditionalExpression->conditional)
//                         : logicalOR;
//     else
//         return logicalOR;
// }

// ArithmeticType Calculator::calc(LogicalORExpression *logicalORExpression)
// {
//     if(logicalORExpression->vec.size() == 1)
//         return calc(logicalORExpression->vec.front());
//     else
//     {
//         for(auto &&l : logicalORExpression->vec)
//         {
//             ArithmeticType res = calc(l);
//             if(res.tag != ArithmeticType::Tag::NONE)
//             {
//                 if(res.isNonzero())
//                 {
//                     ArithmeticType ret = ArithmeticType();
//                     ret.tag = ArithmeticType::Tag::S_INT;
//                     ret.uni.si = 1;
//                     return ret;
//                 }
//             }
//             else
//                 return res;
//         }

//         ArithmeticType ret = ArithmeticType();
//         ret.tag = ArithmeticType::Tag::S_INT;
//         ret.uni.si = 0;
//         return ret;
//     }
// }

// ArithmeticType Calculator::calc(LogicalANDExpression *logicalANDExpression)
// {
//     if(logicalANDExpression->vec.size() == 1)
//         return calc(logicalANDExpression->vec.front());
//     else
//     {
//         for(auto &&e : logicalANDExpression->vec)
//         {
//             ArithmeticType res = calc(e);
//             if(res.tag != ArithmeticType::Tag::NONE)
//             {
//                 if(!res.isNonzero())
//                 {
//                     ArithmeticType ret = ArithmeticType();
//                     ret.tag = ArithmeticType::Tag::S_INT;
//                     ret.uni.si = 0;
//                     return ret;
//                 }
//             }
//             else
//                 return res;
//         }

//         ArithmeticType ret = ArithmeticType();
//         ret.tag = ArithmeticType::Tag::S_INT;
//         ret.uni.si = 1;
//         return ret;
//     }
// }

// ArithmeticType Calculator::calc(InclusiveORExpression *inclusiveORExpression)
// {
//     if(inclusiveORExpression->vec.size() == 1)
//         return calc(inclusiveORExpression->vec.front());
//     else
//     {
//         ArithmeticType ret = calc(inclusiveORExpression->vec.front());
//         if(!ret.isInteger())
//         {
//             std::cout << "TP7 Calculator error:\n"
//                 "    what: inclusive-OR-expression cannot include type other than integer type.\n"
//                 << std::flush;
//             return ArithmeticType();
//         }

//         for(std::size_t i = 1; i < inclusiveORExpression->vec.size(); i++)
//         {
//             ArithmeticType rhs = calc(inclusiveORExpression->vec[i]);
//             if(!ret.isInteger())
//             {
//                 std::cout << "TP7 Calculator error:\n"
//                     "    what: inclusive-OR-expression cannot include type other than integer type.\n"
//                     << std::flush;
//                 return ArithmeticType();
//             }
//             ret = ret | rhs;
//         }

//         return ret;
//     }
// }

// ArithmeticType Calculator::calc(ExclusiveORExpression *exclusiveORExpression)
// {
//     if(exclusiveORExpression->vec.size() == 1)
//         return calc(exclusiveORExpression->vec.front());
//     else
//     {
//         ArithmeticType ret = calc(exclusiveORExpression->vec.front());
//         if(!ret.isInteger())
//         {
//             std::cout << "TP7 Calculator error:\n"
//                 "    what: exclusive-OR-expression cannot include type other than integer type.\n"
//                 << std::flush;
//             return ArithmeticType();
//         }

//         for(std::size_t i = 1; i < exclusiveORExpression->vec.size(); i++)
//         {
//             ArithmeticType rhs = calc(exclusiveORExpression->vec[i]);
//             if(!ret.isInteger())
//             {
//                 std::cout << "TP7 Calculator error:\n"
//                     "    what: exclusive-OR-expression cannot include type other than integer type.\n"
//                     << std::flush;
//                 return ArithmeticType();
//             }
//             ret = ret ^ rhs;
//         }

//         return ret;
//     }
// }

// ArithmeticType Calculator::calc(ANDExpression *andExpression)
// {
//     if(andExpression->vec.size() == 1)
//         return calc(andExpression->vec.front());
//     else
//     {
//         ArithmeticType ret = calc(andExpression->vec.front());
//         if(!ret.isInteger())
//         {
//             std::cout << "TP7 Calculator error:\n"
//                 "    what: AND-expression cannot include type other than integer type.\n"
//                 << std::flush;
//             return ArithmeticType();
//         }

//         for(std::size_t i = 1; i < andExpression->vec.size(); i++)
//         {
//             ArithmeticType rhs = calc(andExpression->vec[i]);
//             if(!ret.isInteger())
//             {
//                 std::cout << "TP7 Calculator error:\n"
//                     "    what: AND-expression cannot include type other than integer type.\n"
//                     << std::flush;
//                 return ArithmeticType();
//             }
//             ret = ret & rhs;
//         }

//         return ret;
//     }
// }

// ArithmeticType Calculator::calc(EqualityExpression *equalityExpression)
// {
//     if(equalityExpression->relvec.size() == 1)
//         return calc(equalityExpression->relvec.front());
//     else
//     {
//         ArithmeticType ret = calc(equalityExpression->relvec.front());
//         if(!ret.isArithmetic())
//             return ret;

//         for(std::size_t i = 1; i < equalityExpression->relvec.size(); i++)
//         {
//             ArithmeticType rhs = calc(equalityExpression->relvec[i]);
//             if(!rhs.isArithmetic())
//                 return ArithmeticType();
            
//             if(equalityExpression->tagvec[i - 1]->tag == Punctuator::Tag::EQUAL)
//                 ret = ret == rhs;
//             else if(equalityExpression->tagvec[i - 1]->tag == Punctuator::Tag::NOT_EQ)
//                 ret = ret != rhs;
//             else
//             {
//                 std::cerr << "TP7 Calculator implementation error:\n"
//                     "    what: equality-expression cannot include operator othre than equal and not-equal.\n"
//                     << std::flush;
//                 return ArithmeticType();
//             }

//             ret.cast(ArithmeticType::Tag::S_INT);
//         }

//         return ret;
//     }
// }

// ArithmeticType Calculator::calc(RelationalExpression *relationalExpression)
// {
//     if(relationalExpression->sftvec.size() == 1)
//         return calc(relationalExpression->sftvec.front());
//     else
//     {
//         ArithmeticType ret = calc(relationalExpression->sftvec.front());
//         if(!ret.isArithmetic())
//             return ret;

//         for(std::size_t i = 1; i < relationalExpression->sftvec.size(); i++)
//         {
//             ArithmeticType rhs = calc(relationalExpression->sftvec[i]);
//             if(!rhs.isArithmetic())
//                 return ArithmeticType();
            
//             if(relationalExpression->tagvec[i - 1]->tag == Punctuator::Tag::LESS)
//                 ret = ret < rhs;
//             else if(relationalExpression->tagvec[i - 1]->tag == Punctuator::Tag::GREATER)
//                 ret = ret > rhs;
//             else if(relationalExpression->tagvec[i - 1]->tag == Punctuator::Tag::LESS_EQ)
//                 ret = ret <= rhs;
//             else if(relationalExpression->tagvec[i - 1]->tag == Punctuator::Tag::GREATER_EQ)
//                 ret = ret >= rhs;
//             else
//             {
//                 std::cerr << "TP7 Calculator implementation error:\n"
//                     "    what: relational-expression cannot include operator othre than less, greater, less-eq, greater-eq operator.\n"
//                     << std::flush;
//                 return ArithmeticType();
//             }

//             ret.cast(ArithmeticType::Tag::S_INT);
//         }

//         return ret;
//     }
// }

// ArithmeticType Calculator::calc(ShiftExpression *expression)
// {
//     if(expression->addvec.size() == 1)
//         return calc(expression->addvec.front());
//     else
//     {
//         ArithmeticType ret = calc(expression->addvec.front());
//         if(!ret.isInteger())
//         {
//             std::cout << "TP7 Calculator error:\n"
//                 "    what: shift-expression cannot include type other than integer type.\n"
//                 << std::flush;
//             return ArithmeticType();
//         }

//         for(std::size_t i = 1; i < expression->addvec.size(); i++)
//         {
//             ArithmeticType rhs = calc(expression->addvec[i]);
//             ArithmeticType::Tag resTag = ret.tag;
//             if(!ret.isInteger())
//             {
//                 std::cout << "TP7 Calculator error:\n"
//                     "    what: shift-expression cannot include type other than integer type.\n"
//                     << std::flush;
//                 return ArithmeticType();
//             }
            
//             if(expression->tagvec[i - 1]->tag == Punctuator::Tag::L_SHIFT)
//                 ret = ret << rhs;
//             else if(expression->tagvec[i - 1]->tag == Punctuator::Tag::R_SHIFT)
//                 ret = ret >> rhs;
//             else
//             {
//                 std::cerr << "TP7 Calculator implementation error:\n"
//                     "    what: shift-expression cannot include operator othre than l-shift or r-shift.\n"
//                     << std::flush;
//                 return ArithmeticType();
//             }

//             ret.cast(resTag);
//         }

//         return ret;
//     }
// }

// ArithmeticType Calculator::calc(AdditiveExpression *expression)
// {
//     if(expression->mulvec.size() == 1)
//         return calc(expression->mulvec.front());
//     else
//     {
//         ArithmeticType ret = calc(expression->mulvec.front());
//         if(!ret.isArithmetic())
//             return ArithmeticType();

//         for(std::size_t i = 1; i < expression->mulvec.size(); i++)
//         {
//             ArithmeticType rhs = calc(expression->mulvec[i]);
//             if(!ret.isArithmetic())
//                 return ArithmeticType();
            
//             if(expression->tagvec[i - 1]->tag == Punctuator::Tag::PLUS)
//                 ret = ret + rhs;
//             else if(expression->tagvec[i - 1]->tag == Punctuator::Tag::MINUS)
//                 ret = ret - rhs;
//             else
//             {
//                 std::cerr << "TP7 Calculator implementation error:\n"
//                     "    what: additive-expression cannot include operator othre than plus and minus operator.\n"
//                     << std::flush;
//                 return ArithmeticType();
//             }
//         }

//         return ret;
//     }
// }

// ArithmeticType Calculator::calc(MultiplicativeExpression *expression)
// {
//     if(expression->castvec.size() == 1)
//         return calc(expression->castvec.front());
//     else
//     {
//         ArithmeticType ret = calc(expression->castvec.front());
//         if(!ret.isArithmetic())
//             return ArithmeticType();

//         for(std::size_t i = 1; i < expression->castvec.size(); i++)
//         {
//             ArithmeticType rhs = calc(expression->castvec[i]);
//             if(!ret.isArithmetic())
//                 return ArithmeticType();
            
//             if(expression->tagvec[i - 1]->tag == Punctuator::Tag::AST)
//                 ret = ret * rhs;
//             else if(expression->tagvec[i - 1]->tag == Punctuator::Tag::SLASH)
//                 ret = ret / rhs;
//             else if(expression->tagvec[i - 1]->tag == Punctuator::Tag::PER)
//             {
//                 if(ret.isInteger() && rhs.isInteger())
//                     ret = ret % rhs;
//                 if(!ret.isInteger() || !rhs.isInteger())
//                 {
//                     std::cout << "TP7 Calculator error:\n"
//                         "    what: % operator cannot have type other than integer.\n"
//                         << std::flush;
//                     return ArithmeticType();
//                 }
//             }
//             else
//             {
//                 std::cerr << "TP7 Calculator implementation error:\n"
//                     "    what: multiplicative-expression cannot include operator othre than asterisk, slash or percent operator.\n"
//                     << std::flush;
//                 return ArithmeticType();
//             }
//         }

//         return ret;
//     }
// }

// ArithmeticType Calculator::calc(CastExpression *expression)
// {
//     ArithmeticType ret = ArithmeticType();

//     switch(expression->tag)
//     {
//         case(CastExpression::Tag::UNARY):
//             ret = calc(expression->uni.unary);
//             break;
        
//         default:
//             std::cerr << "TP7 Calculator implementation error:\n"
//                 "    what: cast-expression.\n"
//             << std::flush;
//     }

//     return ret;
// }

// ArithmeticType Calculator::calc(UnaryExpression *expression)
// {
//     ArithmeticType ret = ArithmeticType();

//     switch(expression->tag)
//     {
//         case(UnaryExpression::Tag::POSTFIX):
//             ret = calc(expression->uni.postfix);
//             break;
//         case(UnaryExpression::Tag::INC):
//         case(UnaryExpression::Tag::DEC):
//             std::cout << "TP7 Calculator error:\n"
//                 "    what: constant-expression cannnot include increment and decrement\n"
//                 << std::endl;
//             break;
//         case(UnaryExpression::Tag::UNARY):
//         {
//             ArithmeticType rhs = calc(expression->uni.sUnary.cast);
//             if(!rhs.isArithmetic())
//                 return ArithmeticType();

//             switch(expression->uni.sUnary.tag->tag)
//             {
//                 case(Punctuator::Tag::PLUS):
//                     ret = +rhs; break;
//                 case(Punctuator::Tag::MINUS):
//                     ret = -rhs; break;
//                 case(Punctuator::Tag::TILDE):
//                     if(rhs.isInteger())
//                         ret = ~rhs;
//                     else
//                     {
//                         std::cout << "TP7 Calculator error:\n"
//                             "    what: tolde-operator cannot include type other than integer.\n"
//                             << std::flush;
//                         return ArithmeticType();
//                     }
//                     break;
//                 case(Punctuator::Tag::EXC):
//                     ret = !rhs;
//                     ret.cast(ArithmeticType::Tag::S_INT);
//                     break;
                
//                 default:
//                     Simbol::unexpectTag("UnaryExpression");
//             }
//             break;
//         }

//         default:
//             Simbol::unexpectTag("UnaryExpression");
//     }

//     return ret;
// }

// ArithmeticType Calculator::calc(PostfixExpression *expression)
// {
//     ArithmeticType ret = ArithmeticType();

//     switch(expression->tag)
//     {
//         case(PostfixExpression::Tag::PRIMARY):
//             ret = calc(expression->uni.primary);
//             break;
        
//         default:
//             Simbol::unexpectTag("PostfixExpression");
//     }

//     return ret;
// }

// ArithmeticType Calculator::calc(PrimaryExpression *expression)
// {
//     ArithmeticType ret = ArithmeticType();

//     switch(expression->tag)
//     {
//         case(PrimaryExpression::Tag::CONSTANT):
//             ret = convertConstant(expression->uni.constant);
//             break;
//         case(PrimaryExpression::Tag::EXPRESSION):
//             ret = calc(expression->uni.expression);
//             break;

//         default:
//             Simbol::unexpectTag("PrimaryExpression");
//     }

//     return ret;
// }

// ArithmeticType Calculator::convertConstant(Constant *constant)
// {
//     ArithmeticType ret = ArithmeticType();

//     switch(constant->tag)
//     {
//         case(Constant::Tag::INTEGER):
//             ret = convertIntegerConstant(constant->uni.integer);
//             break;
//         case(Constant::Tag::FLOATING):
//             ret = convertFloatingConstant(constant->uni.floating);
//             break;
//         case(Constant::Tag::CHARACTER):
//             ret = convertCharacterConstant(constant->uni.character);
//             break;
        
//         default:
//             Simbol::unexpectTag("Constant");
//     }

//     return ret;
// }

// ArithmeticType Calculator::convertIntegerConstant(IntegerConstant *integer)
// {
//     unsigned long long base = 0;
//     bool isContainedUnsigned = false;
//     switch(integer->prefixTag)
//     {
//         case(IntegerConstant::PrefixTag::DECIMAL):
//             base = 10;
//             break;
//         case(IntegerConstant::PrefixTag::OCTAL):
//             base = 8;
//             isContainedUnsigned = true;
//             break;
//         case(IntegerConstant::PrefixTag::HEXADECIMAL):
//             base = 16;
//             isContainedUnsigned = true;
//             break;
        
//         default:
//             Simbol::unexpectTag("IntegerConstant");
//             return ArithmeticType();
//     }

//     unsigned long long value = 0;
//     try
//     {
//         value = integer->str.empty() ? 0 : std::stoull(integer->str, nullptr, base);
//     }
//     catch(const std::out_of_range &error)
//     {
//         std::cout << "TP7 Calculator error:\n"
//             "    what: IntegerConstant is out of range.\n"
//             "    value: " << integer->string()
//             << std::endl;
//         return ArithmeticType();
//     }

//     ArithmeticType retval;
//     switch(integer->suffixTag)
//     {
//         case(IntegerConstant::SuffixTag::NONE):
//             if(value <= static_cast<unsigned long long>(std::numeric_limits<int>::max()))
//             {
//                 retval.tag = ArithmeticType::Tag::S_INT;
//                 retval.uni.si = static_cast<int>(value);
//             }
//             else if(value <= static_cast<unsigned long long>(std::numeric_limits<unsigned int>::max())
//                 && isContainedUnsigned)
//             {
//                 retval.tag = ArithmeticType::Tag::U_INT;
//                 retval.uni.ui = static_cast<unsigned int>(value);
//             }
//             else if(value <= static_cast<unsigned long long>(std::numeric_limits<long int>::max()))
//             {
//                 retval.tag = ArithmeticType::Tag::S_LONG;
//                 retval.uni.sl = static_cast<long int>(value);
//             }
//             else if(value <= static_cast<unsigned long long>(std::numeric_limits<unsigned long int>::max())
//                 && isContainedUnsigned)
//             {
//                 retval.tag = ArithmeticType::Tag::U_LONG;
//                 retval.uni.ul = static_cast<unsigned long int>(value);
//             }
//             else if(value <= static_cast<unsigned long long>(std::numeric_limits<long long int>::max()))
//             {
//                 retval.tag = ArithmeticType::Tag::S_LONG_LONG;
//                 retval.uni.sll = static_cast<long long int>(value);
//             }
//             else if(value <= static_cast<unsigned long long>(std::numeric_limits<unsigned long long int>::max())
//                 && isContainedUnsigned)
//             {
//                 retval.tag = ArithmeticType::Tag::U_LONG_LONG;
//                 retval.uni.ull = static_cast<unsigned long long>(value);
//             }
//             else
//             {
//                 std::cout << "TP7 Calculator error:\n"
//                     "    what: IntegerConstant is out of range.\n"
//                     "    value: " << integer->string()
//                     << std::endl;
//                 return ArithmeticType();
//             }
//             break;
//         case(IntegerConstant::SuffixTag::UNSIGNED):
//             if(value <= static_cast<unsigned long long>(std::numeric_limits<unsigned int>::max()))
//             {
//                 retval.tag = ArithmeticType::Tag::U_INT;
//                 retval.uni.ui = static_cast<unsigned int>(value);
//             }
//             else if(value <= static_cast<unsigned long long>(std::numeric_limits<unsigned long int>::max()))
//             {
//                 retval.tag = ArithmeticType::Tag::U_LONG;
//                 retval.uni.ul = static_cast<unsigned long>(value);
//             }
//             else if(value <= static_cast<unsigned long long>(std::numeric_limits<unsigned long long int>::max()))
//             {
//                 retval.tag = ArithmeticType::Tag::U_LONG_LONG;
//                 retval.uni.ull = static_cast<unsigned long long>(value);
//             }
//             else
//             {
//                 std::cout << "TP7 Calculator error:\n"
//                     "    what: IntegerConstant is out of range.\n"
//                     "    value: " << integer->string()
//                     << std::endl;
//                 return ArithmeticType();
//             }
//             break;
//         case(IntegerConstant::SuffixTag::LONG):
//             if(value <= static_cast<unsigned long long>(std::numeric_limits<long int>::max()))
//             {
//                 retval.tag = ArithmeticType::Tag::S_LONG;
//                 retval.uni.sl = static_cast<long int>(value);
//             }
//             else if(value <= static_cast<unsigned long long>(std::numeric_limits<unsigned long long>::max())
//                 && isContainedUnsigned)
//             {
//                 retval.tag = ArithmeticType::Tag::U_LONG;
//                 retval.uni.ul = static_cast<unsigned long int>(value);
//             }
//             else if(value <= static_cast<unsigned long long>(std::numeric_limits<long long int>::max()))
//             {
//                 retval.tag = ArithmeticType::Tag::S_LONG_LONG;
//                 retval.uni.sll = static_cast<long long int>(value);
//             }
//             else if(value <= static_cast<unsigned long long>(std::numeric_limits<unsigned long long int>::max())
//                 && isContainedUnsigned)
//             {
//                 retval.tag = ArithmeticType::Tag::U_LONG_LONG;
//                 retval.uni.ull = static_cast<unsigned long long int>(value);
//             }
//             else
//             {
//                 std::cout << "TP7 Calculator error:\n"
//                     "    what: IntegerConstant is out of range.\n"
//                     "    value: " << integer->string()
//                     << std::endl;
//                 return ArithmeticType();
//             }
//             break;
//         case(IntegerConstant::SuffixTag::UNSIGNED_LONG):
//             if(value <= static_cast<unsigned long long>(std::numeric_limits<unsigned long int>::max()))
//             {
//                 retval.tag = ArithmeticType::Tag::U_LONG;
//                 retval.uni.ul = static_cast<unsigned long int>(value);
//             }
//             else if(value <= static_cast<unsigned long long>(std::numeric_limits<unsigned long long int>::max()))
//             {
//                 retval.tag = ArithmeticType::Tag::U_LONG_LONG;
//                 retval.uni.ull = static_cast<unsigned long long int>(value);
//             }
//             else
//             {
//                 std::cout << "TP7 Calculator error:\n"
//                     "    what: IntegerConstant is out of range.\n"
//                     "    value: " << integer->string()
//                     << std::endl;
//                 return ArithmeticType();
//             }
//             break;
//         case(IntegerConstant::SuffixTag::LONG_LONG):
//             if(value <= static_cast<unsigned long long>(std::numeric_limits<long long int>::max()))
//             {
//                 retval.tag = ArithmeticType::Tag::S_LONG_LONG;
//                 retval.uni.sll = static_cast<long long int>(value);
//             }
//             else if(value <= static_cast<unsigned long long>(std::numeric_limits<unsigned long long int>::max())
//                 && isContainedUnsigned)
//             {
//                 retval.tag = ArithmeticType::Tag::U_LONG_LONG;
//                 retval.uni.ull = static_cast<unsigned long long int>(value);
//             }
//             else
//             {
//                 std::cout << "TP7 Calculator error:\n"
//                     "    what: IntegerConstant is out of range.\n"
//                     "    value: " << integer->string()
//                     << std::endl;
//                 return ArithmeticType();
//             }
//             break;
//         case(IntegerConstant::SuffixTag::UNSIGNED_LONG_LONG):
//             if(value <= static_cast<unsigned long long>(std::numeric_limits<unsigned long long int>::max()))
//             {
//                 retval.tag = ArithmeticType::Tag::U_LONG_LONG;
//                 retval.uni.ull = static_cast<unsigned long long int>(value);
//             }
//             else
//             {
//                 std::cout << "TP7 Calculator error:\n"
//                     "    what: IntegerConstant is out of range.\n"
//                     "    value: " << integer->string()
//                     << std::endl;
//                 return ArithmeticType();
//             }
//             break;
//     }

//     return retval;
// }

// ArithmeticType Calculator::convertFloatingConstant(FloatingConstant *floating)
// {
//     long double value = 0.0l;
//     int radix = 0;

//     switch(floating->radixTag)
//     {
//         case(FloatingConstant::RadixTag::DECIMAL):
//             radix = 10;
//             break;
//         case(FloatingConstant::RadixTag::HEXADECIMAL):
//             radix = 16;
//             break;
        
//         default:
//             Simbol::unexpectTag("FloatingConstant");
//             return ArithmeticType();
//     }

//     for(auto &&c : floating->integer)
//         value = value * 10 + std::stoi(std::string(1, c), nullptr, radix);
//     long double dec = 0.0l;
//     for(auto iter = floating->decimal.crbegin(); iter != floating->decimal.crend(); iter++)
//         dec = (dec + std::stoi(std::string(1, *iter), nullptr, radix)) / 10;
    
//     unsigned long long exp = 0;
//     try
//     {
//         exp = floating->exponent.empty() ? 0 : std::stoull(floating->exponent);
//     }
//     catch(const std::out_of_range &error)
//     {
//         std::cout << "TP7 Calculator error:\n"
//             "    what: exponent part of FloatingConstant is out of range.\n"
//             "    value: " << floating->string()
//             << std::endl;
//         return ArithmeticType();
//     }

//     if(floating->exponentTag == FloatingConstant::ExponentTag::NONE
//         || floating->exponentTag == FloatingConstant::ExponentTag::PLUS)
//         for(unsigned long long i = 0; i < exp; i++)
//             value *= 10;
//     else
//         for(unsigned long long i = 0; i < exp; i++)
//             value /= 10;

//     ArithmeticType retval;
//     switch(floating->suffixTag)
//     {
//         case(FloatingConstant::SuffixTag::FLOAT):
//             retval.uni.f = static_cast<float>(value);
//             retval.tag = ArithmeticType::Tag::FLOAT;
//             break;
//         case(FloatingConstant::SuffixTag::LONG):
//             retval.uni.ld = value;
//             retval.tag = ArithmeticType::Tag::LONG_DOUBLE;
//             break;
//         case(FloatingConstant::SuffixTag::NONE):
//             retval.uni.d = value;
//             retval.tag = ArithmeticType::Tag::DOUBLE;
//             break;
//     }

//     return retval;
// }

// ArithmeticType Calculator::convertCharacterConstant(CharacterConstant *character)
// {
//     int value = 0;
//     for(auto &&e : character->str)
//         value = value * std::numeric_limits<char>::max() + e;
    
//     ArithmeticType retval;
//     retval.tag = ArithmeticType::Tag::S_INT;
//     retval.uni.si = value;

//     return retval;
// }

// }