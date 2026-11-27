// dotnet/decompiler/csharp/syntax/assignment_expression.cpp -- see assignment_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/assignment_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

AssignmentExpression::AssignmentExpression(Expression* left, Expression* right) {
    set_left(left);
    set_right(right);
}

AssignmentExpression::AssignmentExpression(Expression* left, AssignmentOperatorType op, Expression* right) {
    set_left(left);
    set_operator_(op);
    set_right(right);
}

void AssignmentExpression::set_left(Expression* value) { set_child_node(left_, value, 0); }
void AssignmentExpression::set_right(Expression* value) { set_child_node(right_, value, 1); }

bool AssignmentExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<AssignmentExpression>(other);
    return o != nullptr &&
           match_required(left_, o->left_, match) &&
           (operator_value_ == AssignmentOperatorType::Any || operator_value_ == o->operator_value_) &&
           match_required(right_, o->right_, match);
}

int AssignmentExpression::get_child_count() const { return 2; }

AstNode* AssignmentExpression::get_child_by_index(int index) const {
    if (index == 0) return left_;
    if (index == 1) return right_;
    throw std::out_of_range("index");
}

void AssignmentExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(left_, cast<Expression>(value), index); return; }
    if (index == 1) { set_child_node(right_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& AssignmentExpression::get_child_slot_info(int index) const {
    if (index == 0) return left_slot;
    if (index == 1) return right_slot;
    throw std::out_of_range("index");
}

void AssignmentExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<AssignmentExpression*>(copy_node);
    if (left_ != nullptr) copy->set_left(clone_as(left_.get()));
    if (right_ != nullptr) copy->set_right(clone_as(right_.get()));
}

std::string_view AssignmentExpression::get_operator_token(AssignmentOperatorType op) {
    switch (op) {
        case AssignmentOperatorType::Assign: return AssignmentExpression::assign_token;
        case AssignmentOperatorType::Add: return AssignmentExpression::add_token;
        case AssignmentOperatorType::Subtract: return AssignmentExpression::subtract_token;
        case AssignmentOperatorType::Multiply: return AssignmentExpression::multiply_token;
        case AssignmentOperatorType::Divide: return AssignmentExpression::divide_token;
        case AssignmentOperatorType::Modulus: return AssignmentExpression::modulus_token;
        case AssignmentOperatorType::ShiftLeft: return AssignmentExpression::shift_left_token;
        case AssignmentOperatorType::ShiftRight: return AssignmentExpression::shift_right_token;
        case AssignmentOperatorType::UnsignedShiftRight: return AssignmentExpression::unsigned_shift_right_token;
        case AssignmentOperatorType::BitwiseAnd: return AssignmentExpression::bitwise_and_token;
        case AssignmentOperatorType::BitwiseOr: return AssignmentExpression::bitwise_or_token;
        case AssignmentOperatorType::ExclusiveOr: return AssignmentExpression::exclusive_or_token;
        default: throw std::invalid_argument("Invalid value for AssignmentOperatorType");
    }
}

std::optional<AssignmentOperatorType> AssignmentExpression::get_assignment_operator_type_from_expression_type(il::ExpressionType expression_type) {
    using il::ExpressionType;
    switch (expression_type) {
        case ExpressionType::AddAssign:
        case ExpressionType::AddAssignChecked: return AssignmentOperatorType::Add;
        case ExpressionType::AndAssign: return AssignmentOperatorType::BitwiseAnd;
        case ExpressionType::DivideAssign: return AssignmentOperatorType::Divide;
        case ExpressionType::ExclusiveOrAssign: return AssignmentOperatorType::ExclusiveOr;
        case ExpressionType::LeftShiftAssign: return AssignmentOperatorType::ShiftLeft;
        case ExpressionType::ModuloAssign: return AssignmentOperatorType::Modulus;
        case ExpressionType::MultiplyAssign:
        case ExpressionType::MultiplyAssignChecked: return AssignmentOperatorType::Multiply;
        case ExpressionType::OrAssign: return AssignmentOperatorType::BitwiseOr;
        case ExpressionType::RightShiftAssign: return AssignmentOperatorType::ShiftRight;
        case ExpressionType::SubtractAssign:
        case ExpressionType::SubtractAssignChecked: return AssignmentOperatorType::Subtract;
        default: return std::nullopt;
    }
}


}  // namespace cb::dotnet::decompiler::csharp::syntax
