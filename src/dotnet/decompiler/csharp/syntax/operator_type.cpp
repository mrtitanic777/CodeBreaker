// dotnet/decompiler/csharp/syntax/operator_type.cpp -- see operator_type.hpp.
#include "dotnet/decompiler/csharp/syntax/operator_type.hpp"

#include <stdexcept>

namespace cb::dotnet::decompiler::csharp::syntax {

using il::ExpressionType;

ExpressionType get_linq_node_type(BinaryOperatorType op, bool check_for_overflow) {
    switch (op) {
        case BinaryOperatorType::BitwiseAnd: return ExpressionType::And;
        case BinaryOperatorType::BitwiseOr: return ExpressionType::Or;
        case BinaryOperatorType::ConditionalAnd: return ExpressionType::AndAlso;
        case BinaryOperatorType::ConditionalOr: return ExpressionType::OrElse;
        case BinaryOperatorType::ExclusiveOr: return ExpressionType::ExclusiveOr;
        case BinaryOperatorType::GreaterThan: return ExpressionType::GreaterThan;
        case BinaryOperatorType::GreaterThanOrEqual: return ExpressionType::GreaterThanOrEqual;
        case BinaryOperatorType::Equality: return ExpressionType::Equal;
        case BinaryOperatorType::InEquality: return ExpressionType::NotEqual;
        case BinaryOperatorType::LessThan: return ExpressionType::LessThan;
        case BinaryOperatorType::LessThanOrEqual: return ExpressionType::LessThanOrEqual;
        case BinaryOperatorType::Add: return check_for_overflow ? ExpressionType::AddChecked : ExpressionType::Add;
        case BinaryOperatorType::Subtract: return check_for_overflow ? ExpressionType::SubtractChecked : ExpressionType::Subtract;
        case BinaryOperatorType::Multiply: return check_for_overflow ? ExpressionType::MultiplyChecked : ExpressionType::Multiply;
        case BinaryOperatorType::Divide: return ExpressionType::Divide;
        case BinaryOperatorType::Modulus: return ExpressionType::Modulo;
        case BinaryOperatorType::ShiftLeft: return ExpressionType::LeftShift;
        case BinaryOperatorType::ShiftRight: return ExpressionType::RightShift;
        case BinaryOperatorType::NullCoalescing: return ExpressionType::Coalesce;
        case BinaryOperatorType::Range:
        case BinaryOperatorType::UnsignedShiftRight: return ExpressionType::Extension;
        default: throw std::invalid_argument("Invalid value for BinaryOperatorType");
    }
}

ExpressionType get_linq_node_type(UnaryOperatorType op, bool check_for_overflow) {
    switch (op) {
        case UnaryOperatorType::Not: return ExpressionType::Not;
        case UnaryOperatorType::BitNot: return ExpressionType::OnesComplement;
        case UnaryOperatorType::Minus: return check_for_overflow ? ExpressionType::NegateChecked : ExpressionType::Negate;
        case UnaryOperatorType::Plus: return ExpressionType::UnaryPlus;
        case UnaryOperatorType::Increment: return ExpressionType::PreIncrementAssign;
        case UnaryOperatorType::Decrement: return ExpressionType::PreDecrementAssign;
        case UnaryOperatorType::PostIncrement: return ExpressionType::PostIncrementAssign;
        case UnaryOperatorType::PostDecrement: return ExpressionType::PostDecrementAssign;
        case UnaryOperatorType::Dereference:
        case UnaryOperatorType::AddressOf:
        case UnaryOperatorType::Await:
        case UnaryOperatorType::SuppressNullableWarning:
        case UnaryOperatorType::IndexFromEnd:
        case UnaryOperatorType::PatternNot:
        case UnaryOperatorType::PatternRelationalLessThan:
        case UnaryOperatorType::PatternRelationalLessThanOrEqual:
        case UnaryOperatorType::PatternRelationalGreaterThan:
        case UnaryOperatorType::PatternRelationalGreaterThanOrEqual: return ExpressionType::Extension;
        default: throw std::invalid_argument("Invalid value for UnaryOperatorType");
    }
}

ExpressionType get_linq_node_type(AssignmentOperatorType op, bool check_for_overflow) {
    switch (op) {
        case AssignmentOperatorType::Assign: return ExpressionType::Assign;
        case AssignmentOperatorType::Add: return check_for_overflow ? ExpressionType::AddAssignChecked : ExpressionType::AddAssign;
        case AssignmentOperatorType::Subtract: return check_for_overflow ? ExpressionType::SubtractAssignChecked : ExpressionType::SubtractAssign;
        case AssignmentOperatorType::Multiply: return check_for_overflow ? ExpressionType::MultiplyAssignChecked : ExpressionType::MultiplyAssign;
        case AssignmentOperatorType::Divide: return ExpressionType::DivideAssign;
        case AssignmentOperatorType::Modulus: return ExpressionType::ModuloAssign;
        case AssignmentOperatorType::ShiftLeft: return ExpressionType::LeftShiftAssign;
        case AssignmentOperatorType::ShiftRight: return ExpressionType::RightShiftAssign;
        case AssignmentOperatorType::UnsignedShiftRight: return ExpressionType::Extension;
        case AssignmentOperatorType::BitwiseAnd: return ExpressionType::AndAssign;
        case AssignmentOperatorType::BitwiseOr: return ExpressionType::OrAssign;
        case AssignmentOperatorType::ExclusiveOr: return ExpressionType::ExclusiveOrAssign;
        default: throw std::invalid_argument("Invalid value for AssignmentOperatorType");
    }
}

std::optional<BinaryOperatorType> get_corresponding_binary_operator(AssignmentOperatorType op) {
    switch (op) {
        case AssignmentOperatorType::Assign: return std::nullopt;
        case AssignmentOperatorType::Add: return BinaryOperatorType::Add;
        case AssignmentOperatorType::Subtract: return BinaryOperatorType::Subtract;
        case AssignmentOperatorType::Multiply: return BinaryOperatorType::Multiply;
        case AssignmentOperatorType::Divide: return BinaryOperatorType::Divide;
        case AssignmentOperatorType::Modulus: return BinaryOperatorType::Modulus;
        case AssignmentOperatorType::ShiftLeft: return BinaryOperatorType::ShiftLeft;
        case AssignmentOperatorType::ShiftRight: return BinaryOperatorType::ShiftRight;
        case AssignmentOperatorType::UnsignedShiftRight: return BinaryOperatorType::UnsignedShiftRight;
        case AssignmentOperatorType::BitwiseAnd: return BinaryOperatorType::BitwiseAnd;
        case AssignmentOperatorType::BitwiseOr: return BinaryOperatorType::BitwiseOr;
        case AssignmentOperatorType::ExclusiveOr: return BinaryOperatorType::ExclusiveOr;
        default: throw std::invalid_argument("Invalid value for AssignmentOperatorType");
    }
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
