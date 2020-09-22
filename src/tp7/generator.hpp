#ifndef TP7_GENERATOR_HPP
#define TP7_GENERATOR_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <string>

class StringLiteral;
class Identifier;
class Constant;
class IntegerConstant;
class FloatingConstant;
class EnumerationConstant;
class CharacterConstant;

namespace TP7
{

class TranslationUnit;
class FunctionDefinition;
class ExternalDeclaration;
class Declarator;
class Declaration;
class InitDeclaratorList;
class InitDeclarator;
class Initializer;
class DirectDeclarator;
class StaticAssertDeclaration;
class CompoundStatement;
class BlockItemList;
class BlockItem;
class Statement;
class LabeledStatement;
class ExpressionStatement;
class SelectionStatement;
class IterationStatement;
class JumpStatement;
class Expression;
class AssignmentExpression;
class AssignmentOperator;
class ConditionalExpression;
class LogicalORExpression;
class LogicalANDExpression;
class InclusiveORExpression;
class ExclusiveORExpression;
class ANDExpression;
class EqualityExpression;
class RelationalExpression;
class ShiftExpression;
class AdditiveExpression;
class MultiplicativeExpression;
class CastExpression;
class UnaryExpression;
class PostfixExpression;
class PrimaryExpression;
class GenericSelection;
class ArgumentExpressionList;

class Generator
{
private:
    static const std::vector<std::string> ARGUMENT_REGISTERS;

public:
    Generator() noexcept;
    Generator(const Generator&) = delete;
    Generator(Generator&&) = delete;

    std::string generate(const TranslationUnit*);
    std::string generate(const FunctionDefinition*);
    std::string generate(const ExternalDeclaration*);
    std::string generate(const Declaration*); // for external-declaration
    std::string generate(const CompoundStatement*);
    std::string generate(const BlockItemList*);
    std::string generate(const BlockItem*);
    std::string generate(const Statement*);
    std::string generate(const Declaration*, int); // for block-item(overload)
    std::string generate(const InitDeclaratorList*, int); // for block-item(overload)
    std::string generate(const InitDeclarator*, int); // for block-item(overload)
    std::string generate(const Declarator*, int); // for block-item(overload)
    std::string generate(const Initializer*, int); // for block-item(overload)
    std::string generate(const DirectDeclarator*, int); // for block-item(overload)
    std::string generate(const StaticAssertDeclaration*);
    std::string generate(const LabeledStatement*);
    std::string generate(const ExpressionStatement*);
    std::string generate(const SelectionStatement*);
    std::string generate(const IterationStatement*);
    std::string generate(const JumpStatement*);
    std::string generate(const Expression*);
    std::string generate(const AssignmentExpression*);
    std::string generate(const ConditionalExpression*);
    std::string generate(const LogicalORExpression*);
    std::string generate(const LogicalANDExpression*);
    std::string generate(const InclusiveORExpression*);
    std::string generate(const ExclusiveORExpression*);
    std::string generate(const ANDExpression*);
    std::string generate(const EqualityExpression*);
    std::string generate(const RelationalExpression*);
    std::string generate(const ShiftExpression*);
    std::string generate(const AdditiveExpression*);
    std::string generate(const MultiplicativeExpression*);
    std::string generate(const CastExpression*);
    std::string generate(const UnaryExpression*);
    std::string generate(const PostfixExpression*);
    std::string generate(const PrimaryExpression*);
    std::string generate(const ::Identifier*);
    std::string generate(const ::Constant*);
    std::string generate(const ::StringLiteral*);
    std::string generate(const GenericSelection*);
    std::string generate(const ::IntegerConstant*);
    std::string generate(const ::FloatingConstant*);
    std::string generate(const ::EnumerationConstant*);
    std::string generate(const ::CharacterConstant*);
    std::string generate(const ArgumentExpressionList*);

private:
    std::string getIdentifier(const Declarator*) const;
    std::string getIdentifier(const UnaryExpression*) const;
    std::string getIdentifier(const PostfixExpression*) const;
    std::string getIdentifier(const PrimaryExpression*) const;
    std::string getIdentifier(const Expression*) const;
    std::string getIdentifier(const AssignmentExpression*) const;
    std::string getIdentifier(const ConditionalExpression*) const;
    std::string getIdentifier(const LogicalORExpression*) const;
    std::string getIdentifier(const LogicalANDExpression*) const;
    std::string getIdentifier(const InclusiveORExpression*) const;
    std::string getIdentifier(const ExclusiveORExpression*) const;
    std::string getIdentifier(const ANDExpression*) const;
    std::string getIdentifier(const EqualityExpression*) const;
    std::string getIdentifier(const RelationalExpression*) const;
    std::string getIdentifier(const ShiftExpression*) const;
    std::string getIdentifier(const AdditiveExpression*) const;
    std::string getIdentifier(const MultiplicativeExpression*) const;
    std::string getIdentifier(const CastExpression*) const;

    std::string leadIdentifier(const Expression*);
    std::string leadIdentifier(const AssignmentExpression*);
    std::string leadIdentifier(const ConditionalExpression*);
    std::string leadIdentifier(const LogicalORExpression*);
    std::string leadIdentifier(const LogicalANDExpression*);
    std::string leadIdentifier(const InclusiveORExpression*);
    std::string leadIdentifier(const ExclusiveORExpression*);
    std::string leadIdentifier(const ANDExpression*);
    std::string leadIdentifier(const EqualityExpression*);
    std::string leadIdentifier(const RelationalExpression*);
    std::string leadIdentifier(const ShiftExpression*);
    std::string leadIdentifier(const AdditiveExpression*);
    std::string leadIdentifier(const MultiplicativeExpression*);
    std::string leadIdentifier(const CastExpression*);
    std::string leadIdentifier(const UnaryExpression*);
    std::string leadIdentifier(const PostfixExpression*);
    std::string leadIdentifier(const PrimaryExpression*);

    std::string stringnizeLabel(std::size_t) const noexcept;
    std::string stringnizeIdentifier(const std::string&) const;

    std::string getIdentifierPosition(const std::string&) const;

    std::string generateInstruction(const std::string &inst
        , const std::string &ope1 = ""
        , const std::string &ope2 = ""
        , const std::string &ope3 = ""
        , const std::string &ope4 = "") const;

    std::size_t mLabelIdx;
    std::unordered_map<std::string, std::size_t> mLabelMap;
    std::size_t mStackIdx;
    std::vector<std::unordered_map<std::string, std::ptrdiff_t>> mStackMapVec;
    enum class GlobalTag{FUNCTION, OBJECT};
    std::unordered_map<std::string, GlobalTag> mGlobalMap;
};

}

#endif