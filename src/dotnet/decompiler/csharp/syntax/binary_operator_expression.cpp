// dotnet/decompiler/csharp/syntax/binary_operator_expression.cpp -- see binary_operator_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/binary_operator_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

BinaryOperatorExpression::BinaryOperatorExpression(Expression* left, BinaryOperatorType op) {
    set_left(left);
    set_operator_(op);
}

BinaryOperatorExpression::BinaryOperatorExpression(Expression* left, BinaryOperatorType op, Expression* right) : BinaryOperatorExpression(left, op) {
    set_right(right);
}

void BinaryOperatorExpression::set_left(Expression* value) { set_child_node(left_, value, 0); }
void BinaryOperatorExpression::set_right(Expression* value) { set_child_node(right_, value, 1); }

bool BinaryOperatorExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<BinaryOperatorExpression>(other);
    return o != nullptr &&
           match_optional(left_, o->left_, match) &&
           (operator_value_ == BinaryOperatorType::Any || operator_value_ == o->operator_value_) &&
           match_optional(right_, o->right_, match);
}

int BinaryOperatorExpression::get_child_count() const { return 2; }

AstNode* BinaryOperatorExpression::get_child_by_index(int index) const {
    if (index == 0) return left_;
    if (index == 1) return right_;
    throw std::out_of_range("index");
}

void BinaryOperatorExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(left_, cast<Expression>(value), index); return; }
    if (index == 1) { set_child_node(right_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& BinaryOperatorExpression::get_child_slot_info(int index) const {
    if (index == 0) return left_slot;
    if (index == 1) return right_slot;
    throw std::out_of_range("index");
}

void BinaryOperatorExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<BinaryOperatorExpression*>(copy_node);
    if (left_ != nullptr) copy->set_left(clone_as(left_.get()));
    if (right_ != nullptr) copy->set_right(clone_as(right_.get()));
}

std::string_view BinaryOperatorExpression::get_operator_token(BinaryOperatorType op) {
    switch (op) {
        case BinaryOperatorType::BitwiseAnd: return BinaryOperatorExpression::bitwise_and_token;
        case BinaryOperatorType::BitwiseOr: return BinaryOperatorExpression::bitwise_or_token;
        case BinaryOperatorType::ConditionalAnd: return BinaryOperatorExpression::conditional_and_token;
        case BinaryOperatorType::ConditionalOr: return BinaryOperatorExpression::conditional_or_token;
        case BinaryOperatorType::ExclusiveOr: return BinaryOperatorExpression::exclusive_or_token;
        case BinaryOperatorType::GreaterThan: return BinaryOperatorExpression::greater_than_token;
        case BinaryOperatorType::GreaterThanOrEqual: return BinaryOperatorExpression::greater_than_or_equal_token;
        case BinaryOperatorType::Equality: return BinaryOperatorExpression::equality_token;
        case BinaryOperatorType::InEquality: return BinaryOperatorExpression::in_equality_token;
        case BinaryOperatorType::LessThan: return BinaryOperatorExpression::less_than_token;
        case BinaryOperatorType::LessThanOrEqual: return BinaryOperatorExpression::less_than_or_equal_token;
        case BinaryOperatorType::Add: return BinaryOperatorExpression::add_token;
        case BinaryOperatorType::Subtract: return BinaryOperatorExpression::subtract_token;
        case BinaryOperatorType::Multiply: return BinaryOperatorExpression::multiply_token;
        case BinaryOperatorType::Divide: return BinaryOperatorExpression::divide_token;
        case BinaryOperatorType::Modulus: return BinaryOperatorExpression::modulus_token;
        case BinaryOperatorType::ShiftLeft: return BinaryOperatorExpression::shift_left_token;
        case BinaryOperatorType::ShiftRight: return BinaryOperatorExpression::shift_right_token;
        case BinaryOperatorType::UnsignedShiftRight: return BinaryOperatorExpression::unsigned_shift_right_token;
        case BinaryOperatorType::NullCoalescing: return BinaryOperatorExpression::null_coalescing_token;
        case BinaryOperatorType::Range: return BinaryOperatorExpression::range_token;
        case BinaryOperatorType::IsPattern: return BinaryOperatorExpression::is_keyword;
        case BinaryOperatorType::PatternOr: return BinaryOperatorExpression::pattern_or_keyword;
        default: throw std::invalid_argument("Invalid value for BinaryOperatorType");
    }
}


}  // namespace cb::dotnet::decompiler::csharp::syntax
