// dotnet/decompiler/csharp/syntax/unary_operator_expression.cpp -- see unary_operator_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/unary_operator_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

UnaryOperatorExpression::UnaryOperatorExpression(UnaryOperatorType op, Expression* expression) {
    set_operator_(op);
    set_expression(expression);
}

void UnaryOperatorExpression::set_expression(Expression* value) { set_child_node(expression_, value, 0); }

bool UnaryOperatorExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<UnaryOperatorExpression>(other);
    return o != nullptr &&
           (operator_value_ == UnaryOperatorType::Any || operator_value_ == o->operator_value_) &&
           match_required(expression_, o->expression_, match);
}

int UnaryOperatorExpression::get_child_count() const { return 1; }

AstNode* UnaryOperatorExpression::get_child_by_index(int index) const {
    if (index == 0) return expression_;
    throw std::out_of_range("index");
}

void UnaryOperatorExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(expression_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& UnaryOperatorExpression::get_child_slot_info(int index) const {
    if (index == 0) return expression_slot;
    throw std::out_of_range("index");
}

void UnaryOperatorExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<UnaryOperatorExpression*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

std::optional<std::string_view> UnaryOperatorExpression::get_operator_token(UnaryOperatorType op) {
    switch (op) {
        case UnaryOperatorType::Not: return UnaryOperatorExpression::not_token;
        case UnaryOperatorType::BitNot: return UnaryOperatorExpression::bit_not_token;
        case UnaryOperatorType::Minus: return UnaryOperatorExpression::minus_token;
        case UnaryOperatorType::Plus: return UnaryOperatorExpression::plus_token;
        case UnaryOperatorType::Increment:
        case UnaryOperatorType::PostIncrement: return UnaryOperatorExpression::increment_token;
        case UnaryOperatorType::PostDecrement:
        case UnaryOperatorType::Decrement: return UnaryOperatorExpression::decrement_token;
        case UnaryOperatorType::Dereference: return UnaryOperatorExpression::dereference_token;
        case UnaryOperatorType::AddressOf: return UnaryOperatorExpression::address_of_token;
        case UnaryOperatorType::Await: return UnaryOperatorExpression::await_keyword;
        case UnaryOperatorType::NullConditional: return UnaryOperatorExpression::null_conditional_token;
        case UnaryOperatorType::NullConditionalRewrap:
        case UnaryOperatorType::IsTrue: return std::nullopt;
        case UnaryOperatorType::SuppressNullableWarning: return UnaryOperatorExpression::suppress_nullable_warning_token;
        case UnaryOperatorType::IndexFromEnd: return UnaryOperatorExpression::index_from_end_token;
        case UnaryOperatorType::PatternNot: return UnaryOperatorExpression::pattern_not_keyword;
        case UnaryOperatorType::PatternRelationalLessThan: return BinaryOperatorExpression::less_than_token;
        case UnaryOperatorType::PatternRelationalLessThanOrEqual: return BinaryOperatorExpression::less_than_or_equal_token;
        case UnaryOperatorType::PatternRelationalGreaterThan: return BinaryOperatorExpression::greater_than_token;
        case UnaryOperatorType::PatternRelationalGreaterThanOrEqual: return BinaryOperatorExpression::greater_than_or_equal_token;
        default: throw std::invalid_argument("Invalid value for UnaryOperatorType");
    }
}


}  // namespace cb::dotnet::decompiler::csharp::syntax
