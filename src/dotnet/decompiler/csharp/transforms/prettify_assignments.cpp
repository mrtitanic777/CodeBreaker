// dotnet/decompiler/csharp/transforms/prettify_assignments.cpp -- see prettify_assignments.hpp.
#include "dotnet/decompiler/csharp/transforms/prettify_assignments.hpp"

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_conversions.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"
#include "dotnet/decompiler/util/csharp_primitive_cast.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;
using typesystem::IType;
using typesystem::KnownTypeCode;

void PrettifyAssignments::visit_assignment_expression(AssignmentExpression& assignment_ref) {
    AssignmentExpression* assignment = &assignment_ref;
    DepthFirstAstVisitor::visit_assignment_expression(assignment_ref);

    // Combine "x = x op y" into "x op= y"
    // Also supports "x = (T)(x op y)" -> "x op= y", if x.GetType() == T and y is implicitly convertible to T.
    Expression* rhs = assignment->right();
    const IType* expected_type = nullptr;
    if (auto* cast = as<CastExpression>(assignment->right())) {
        rhs = cast->expression();
        expected_type = get_resolve_result(cast->type())->type();
    }

    auto is_implicitly_convertible = [this](Expression* expression, const IType* type) {
        if (type == nullptr) return true;
        auto& conversions = csharp::resolver::CSharpConversions::get(context_->type_system);
        return conversions.implicit_conversion(get_resolve_result(expression), type)->is_implicit();
    };

    if (auto* binary = as<BinaryOperatorExpression>(rhs); binary != nullptr && assignment->operator_() == AssignmentOperatorType::Assign) {
        if (can_convert_to_compound_assignment(assignment->left()) && pattern_matching::is_match(assignment->left(), binary->left()) &&
            binary->right() != nullptr && is_implicitly_convertible(binary->right(), expected_type)) {
            AssignmentOperatorType new_operator = get_assignment_operator_for_binary_operator(binary->operator_());
            if (new_operator != AssignmentOperatorType::Assign) {
                context_->step("Convert assignment to compound assignment", assignment);
                assignment->set_operator_(new_operator);
                // If we found a shorter operator, get rid of the BinaryOperatorExpression:
                copy_annotations_from(assignment, binary);
                assignment->set_right(binary->right());
            }
        }
    }
    if (context_->settings.introduce_increment_and_decrement &&
        (assignment->operator_() == AssignmentOperatorType::Add || assignment->operator_() == AssignmentOperatorType::Subtract)) {
        // detect increment/decrement; ++/-- on float/double compiles to the same IL as adding/subtracting a constant 1 (which both types
        // represent exactly), so a constant 1 right-hand side qualifies there just like for integers
        semantics::ResolveResult* rr = get_resolve_result(assignment->right());
        if (rr->is_compile_time_constant() &&
            (typesystem::is_csharp_primitive_integer_type(*rr->type()) || typesystem::is_known_type(rr->type(), KnownTypeCode::Single) ||
             typesystem::is_known_type(rr->type(), KnownTypeCode::Double)) &&
            util::csharp_primitive_cast(csharp::resolver::get_type_code(rr->type()), typesystem::ConstantValue(std::int32_t{1}), false) ==
                rr->constant_value()) {
            // only if it's not a custom operator
            if (assignment->annotation<il::CallInstruction>() == nullptr && assignment->annotation<il::UserDefinedCompoundAssign>() == nullptr &&
                assignment->annotation<il::DynamicCompoundAssign>() == nullptr) {
                UnaryOperatorType type;
                // When the parent is an expression statement, pre- or post-increment doesn't matter; so we can pick post-increment which is
                // more commonly used (for (int i = 0; i < x; i++))
                if (is<ExpressionStatement>(assignment->parent()))
                    type = (assignment->operator_() == AssignmentOperatorType::Add) ? UnaryOperatorType::PostIncrement : UnaryOperatorType::PostDecrement;
                else
                    type = (assignment->operator_() == AssignmentOperatorType::Add) ? UnaryOperatorType::Increment : UnaryOperatorType::Decrement;
                context_->step(type == UnaryOperatorType::Increment || type == UnaryOperatorType::PostIncrement ? "Convert assignment to increment"
                                                                                                                  : "Convert assignment to decrement",
                               assignment);
                auto* unary_operator = copy_annotations_from(make<UnaryOperatorExpression>(type, detach(assignment->left())), assignment);
                assignment->replace_with(unary_operator);
                context_->end_step(unary_operator);
            }
        }
    }
}

AssignmentOperatorType PrettifyAssignments::get_assignment_operator_for_binary_operator(BinaryOperatorType bop) {
    switch (bop) {
        case BinaryOperatorType::Add:
            return AssignmentOperatorType::Add;
        case BinaryOperatorType::Subtract:
            return AssignmentOperatorType::Subtract;
        case BinaryOperatorType::Multiply:
            return AssignmentOperatorType::Multiply;
        case BinaryOperatorType::Divide:
            return AssignmentOperatorType::Divide;
        case BinaryOperatorType::Modulus:
            return AssignmentOperatorType::Modulus;
        case BinaryOperatorType::ShiftLeft:
            return AssignmentOperatorType::ShiftLeft;
        case BinaryOperatorType::ShiftRight:
            return AssignmentOperatorType::ShiftRight;
        case BinaryOperatorType::UnsignedShiftRight:
            return AssignmentOperatorType::UnsignedShiftRight;
        case BinaryOperatorType::BitwiseAnd:
            return AssignmentOperatorType::BitwiseAnd;
        case BinaryOperatorType::BitwiseOr:
            return AssignmentOperatorType::BitwiseOr;
        case BinaryOperatorType::ExclusiveOr:
            return AssignmentOperatorType::ExclusiveOr;
        default:
            return AssignmentOperatorType::Assign;
    }
}

bool PrettifyAssignments::can_convert_to_compound_assignment(Expression* left) {
    if (auto* mre = as<MemberReferenceExpression>(left)) return is_without_side_effects(mre->target());
    if (auto* ie = as<IndexerExpression>(left)) {
        if (!is_without_side_effects(ie->target())) return false;
        for (Expression* argument : ie->arguments())
            if (!is_without_side_effects(argument)) return false;
        return true;
    }
    if (auto* uoe = as<UnaryOperatorExpression>(left); uoe != nullptr && uoe->operator_() == UnaryOperatorType::Dereference)
        return is_without_side_effects(uoe->expression());
    return is_without_side_effects(left);
}

bool PrettifyAssignments::is_without_side_effects(Expression* left) {
    return is<ThisReferenceExpression>(left) || is<IdentifierExpression>(left) || is<TypeReferenceExpression>(left) || is<BaseReferenceExpression>(left);
}

void PrettifyAssignments::run(AstNode* node, TransformContext& context) {
    context_ = &context;
    try {
        node->accept_visitor(static_cast<IAstVisitor&>(*this));
    } catch (...) {
        context_ = nullptr;
        throw;
    }
    context_ = nullptr;
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
