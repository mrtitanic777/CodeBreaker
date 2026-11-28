// dotnet/decompiler/csharp/output_visitor/insert_parentheses_visitor.cpp -- InsertParenthesesVisitor; see insert_parentheses_visitor.hpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: InsertParenthesesVisitor), Copyright AlphaSierraPapa for the ILSpy team,
// MIT (see NOTICE in dotnet/decompiler).
#include "dotnet/decompiler/csharp/output_visitor/insert_parentheses_visitor.hpp"

#include <functional>
#include <stdexcept>
#include <variant>

namespace cb::dotnet::decompiler::csharp::output_visitor {

using namespace syntax;

namespace {

enum class PrecedenceLevel : int {
    Assignment,
    Conditional,                // ?:
    NullCoalescing,             // ??
    ConditionalOr,              // ||
    ConditionalAnd,             // &&
    BitwiseOr,                  // |
    ExclusiveOr,                // binary ^
    BitwiseAnd,                 // binary &
    Equality,                   // == !=
    RelationalAndTypeTesting,   // < <= > >= is
    Shift,                      // << >>
    Additive,                   // binary + -
    Multiplicative,             // * / %
    Switch,                     // C# 8 switch expression
    Range,                      // ..
    Unary,
    QueryOrLambda,
    NullableRewrap,
    Primary
};

constexpr PrecedenceLevel operator+(PrecedenceLevel level, int n) { return static_cast<PrecedenceLevel>(static_cast<int>(level) + n); }
constexpr bool operator<(PrecedenceLevel a, PrecedenceLevel b) { return static_cast<int>(a) < static_cast<int>(b); }

PrecedenceLevel get_precedence(Expression* expr) {
    // Based on the operator precedence table in the C# language specification.
    if (is<QueryExpression>(expr) || is<LambdaExpression>(expr)) {
        return PrecedenceLevel::QueryOrLambda;
    }
    if (auto* uoe = as<UnaryOperatorExpression>(expr)) {
        switch (uoe->operator_()) {
            case UnaryOperatorType::PostDecrement:
            case UnaryOperatorType::PostIncrement:
            case UnaryOperatorType::NullConditional:
            case UnaryOperatorType::SuppressNullableWarning:
                return PrecedenceLevel::Primary;
            case UnaryOperatorType::NullConditionalRewrap:
                return PrecedenceLevel::NullableRewrap;
            case UnaryOperatorType::IsTrue:
                return PrecedenceLevel::Conditional;
            default:
                return PrecedenceLevel::Unary;
        }
    }
    if (is<CastExpression>(expr)) return PrecedenceLevel::Unary;
    if (auto* primitive = as<PrimitiveExpression>(expr)) {
        const typesystem::ConstantValue& value = primitive->value();
        if (const auto* i = std::get_if<std::int32_t>(&value); i != nullptr && *i < 0) return PrecedenceLevel::Unary;
        if (const auto* l = std::get_if<std::int64_t>(&value); l != nullptr && *l < 0) return PrecedenceLevel::Unary;
        if (const auto* f = std::get_if<float>(&value); f != nullptr && *f < 0) return PrecedenceLevel::Unary;
        if (const auto* d = std::get_if<double>(&value); d != nullptr && *d < 0) return PrecedenceLevel::Unary;
        if (const auto* de = std::get_if<util::Decimal>(&value); de != nullptr && de->negative && !de->is_zero()) return PrecedenceLevel::Unary;
        return PrecedenceLevel::Primary;
    }
    if (auto* boe = as<BinaryOperatorExpression>(expr)) {
        switch (boe->operator_()) {
            case BinaryOperatorType::Range:
                return PrecedenceLevel::Range;
            case BinaryOperatorType::Multiply:
            case BinaryOperatorType::Divide:
            case BinaryOperatorType::Modulus:
                return PrecedenceLevel::Multiplicative;
            case BinaryOperatorType::Add:
            case BinaryOperatorType::Subtract:
                return PrecedenceLevel::Additive;
            case BinaryOperatorType::ShiftLeft:
            case BinaryOperatorType::ShiftRight:
            case BinaryOperatorType::UnsignedShiftRight:
                return PrecedenceLevel::Shift;
            case BinaryOperatorType::GreaterThan:
            case BinaryOperatorType::GreaterThanOrEqual:
            case BinaryOperatorType::LessThan:
            case BinaryOperatorType::LessThanOrEqual:
                return PrecedenceLevel::RelationalAndTypeTesting;
            case BinaryOperatorType::Equality:
            case BinaryOperatorType::InEquality:
                return PrecedenceLevel::Equality;
            case BinaryOperatorType::BitwiseAnd:
                return PrecedenceLevel::BitwiseAnd;
            case BinaryOperatorType::ExclusiveOr:
                return PrecedenceLevel::ExclusiveOr;
            case BinaryOperatorType::BitwiseOr:
                return PrecedenceLevel::BitwiseOr;
            case BinaryOperatorType::ConditionalAnd:
                return PrecedenceLevel::ConditionalAnd;
            case BinaryOperatorType::ConditionalOr:
                return PrecedenceLevel::ConditionalOr;
            case BinaryOperatorType::NullCoalescing:
                return PrecedenceLevel::NullCoalescing;
            case BinaryOperatorType::IsPattern:
                return PrecedenceLevel::RelationalAndTypeTesting;
            case BinaryOperatorType::PatternOr:   // (not upstream; the combinators bind loosest of a pattern's operators)
                return PrecedenceLevel::ConditionalOr;
            default:
                throw std::logic_error("Invalid value for BinaryOperatorType");
        }
    }
    if (is<SwitchExpression>(expr)) return PrecedenceLevel::Switch;
    if (is<IsExpression>(expr) || is<AsExpression>(expr)) return PrecedenceLevel::RelationalAndTypeTesting;
    if (is<ConditionalExpression>(expr) || is<DirectionExpression>(expr)) return PrecedenceLevel::Conditional;
    if (is<AssignmentExpression>(expr)) return PrecedenceLevel::Assignment;
    return PrecedenceLevel::Primary;
}

void parenthesize(Expression* expr) {
    expr->replace_with([](AstNode* e) -> AstNode* {
        auto* parenthesized = make<ParenthesizedExpression>();
        parenthesized->set_expression(cast<Expression>(e));
        return parenthesized;
    });
}

// Parenthesizes the expression if its precedence is lower than the minimum.
void parenthesize_if_required(Expression* expr, PrecedenceLevel minimum_precedence) {
    if (expr == nullptr) return;
    if (get_precedence(expr) < minimum_precedence) {
        parenthesize(expr);
    }
}

bool type_can_be_misinterpreted_as_expression(AstType* type) {
    // This is a heuristic: for a SimpleType or a MemberType that is not a `::` access (a cast to a keyword type is not ambiguous).
    if (auto* mt = as<MemberType>(type)) return !mt->is_double_colon();
    return is<SimpleType>(type);
}

bool is_bitwise_operator(BinaryOperatorType op) {
    return op == BinaryOperatorType::BitwiseAnd || op == BinaryOperatorType::BitwiseOr || op == BinaryOperatorType::ExclusiveOr;
}

// The operator of a binary operator expression; nothing for any other expression.
std::optional<BinaryOperatorType> get_binary_operator_type(Expression* expr) {
    if (auto* boe = as<BinaryOperatorExpression>(expr)) return boe->operator_();
    return std::nullopt;
}

bool interpolation_needs_parenthesis(AstNode* node) {
    if (auto* mt = as<MemberType>(node); mt != nullptr && mt->is_double_colon()) return true;
    if (is<ParenthesizedExpression>(node)) return false;
    if (is<AnonymousMethodExpression>(node)) return false;
    if (auto* lambda = as<LambdaExpression>(node); lambda != nullptr && is<BlockStatement>(lambda->body())) return false;
    if (auto* invocation = as<InvocationExpression>(node)) return interpolation_needs_parenthesis(invocation->target());
    if (auto* cast_expression = as<CastExpression>(node)) return interpolation_needs_parenthesis(cast_expression->expression());
    for (AstNode* child = node->first_child(); child != nullptr; child = child->next_sibling()) {
        if (interpolation_needs_parenthesis(child)) return true;
    }
    return false;
}

}  // namespace

void InsertParenthesesVisitor::visit_member_reference_expression(MemberReferenceExpression& member_reference_expression) {
    // Primary is the highest level: the target of a member access must be primary
    parenthesize_if_required(member_reference_expression.target(), PrecedenceLevel::Primary);
    DepthFirstAstVisitor::visit_member_reference_expression(member_reference_expression);
}

void InsertParenthesesVisitor::visit_pointer_reference_expression(PointerReferenceExpression& pointer_reference_expression) {
    parenthesize_if_required(pointer_reference_expression.target(), PrecedenceLevel::Primary);
    DepthFirstAstVisitor::visit_pointer_reference_expression(pointer_reference_expression);
}

void InsertParenthesesVisitor::visit_invocation_expression(InvocationExpression& invocation_expression) {
    parenthesize_if_required(invocation_expression.target(), PrecedenceLevel::Primary);
    DepthFirstAstVisitor::visit_invocation_expression(invocation_expression);
}

void InsertParenthesesVisitor::visit_indexer_expression(IndexerExpression& indexer_expression) {
    if (indexer_expression.target() != nullptr) {
        parenthesize_if_required(indexer_expression.target(), PrecedenceLevel::Primary);
        if (auto* ace = as<ArrayCreateExpression>(indexer_expression.target())) {
            if (insert_parentheses_for_readability || ace->initializer() == nullptr) parenthesize(indexer_expression.target());
        } else if (auto* sae = as<StackAllocExpression>(indexer_expression.target())) {
            if (insert_parentheses_for_readability || sae->initializer() == nullptr) parenthesize(indexer_expression.target());
        }
    }
    DepthFirstAstVisitor::visit_indexer_expression(indexer_expression);
}

void InsertParenthesesVisitor::visit_unary_operator_expression(UnaryOperatorExpression& unary_operator_expression) {
    // Unary operators bind tighter than their operand needs
    parenthesize_if_required(unary_operator_expression.expression(), get_precedence(&unary_operator_expression));
    // Also parenthesize nested unary operators for readability
    auto* child = as<UnaryOperatorExpression>(unary_operator_expression.expression());
    if (child != nullptr && insert_parentheses_for_readability) parenthesize(child);
    DepthFirstAstVisitor::visit_unary_operator_expression(unary_operator_expression);
}

void InsertParenthesesVisitor::visit_cast_expression(CastExpression& cast_expression) {
    // Cast expression is unary, but allows `(int)(object)x`: no parentheses are needed around a nested cast
    if (!is<CastExpression>(cast_expression.expression())) {
        parenthesize_if_required(cast_expression.expression(),
                                 insert_parentheses_for_readability ? PrecedenceLevel::NullableRewrap : PrecedenceLevel::Unary);
    }
    // "(int)-1" is fine, but "(A)-b" is not: it would be a subtraction.
    auto* uoe = as<UnaryOperatorExpression>(cast_expression.expression());
    if (uoe != nullptr && !(uoe->operator_() == UnaryOperatorType::BitNot || uoe->operator_() == UnaryOperatorType::Not)) {
        if (type_can_be_misinterpreted_as_expression(cast_expression.type())) {
            parenthesize(cast_expression.expression());
        }
    }
    // The same for a negative literal
    auto* pe = as<PrimitiveExpression>(cast_expression.expression());
    if (pe != nullptr && !std::holds_alternative<std::monostate>(pe->value()) && type_can_be_misinterpreted_as_expression(cast_expression.type())) {
        const typesystem::ConstantValue& value = pe->value();
        bool negative = false;
        if (const auto* v = std::get_if<std::int8_t>(&value)) negative = *v < 0;
        else if (const auto* v16 = std::get_if<std::int16_t>(&value)) negative = *v16 < 0;
        else if (const auto* v32 = std::get_if<std::int32_t>(&value)) negative = *v32 < 0;
        else if (const auto* v64 = std::get_if<std::int64_t>(&value)) negative = *v64 < 0;
        else if (const auto* f = std::get_if<float>(&value)) negative = *f < 0;
        else if (const auto* d = std::get_if<double>(&value)) negative = *d < 0;
        else if (const auto* de = std::get_if<util::Decimal>(&value)) negative = de->negative && !de->is_zero();
        if (negative) parenthesize(cast_expression.expression());
    }
    DepthFirstAstVisitor::visit_cast_expression(cast_expression);
}

void InsertParenthesesVisitor::visit_binary_operator_expression(BinaryOperatorExpression& binary_operator_expression) {
    PrecedenceLevel precedence = get_precedence(&binary_operator_expression);
    if (binary_operator_expression.operator_() == BinaryOperatorType::NullCoalescing) {
        if (insert_parentheses_for_readability) {
            // ?? is right-associative
            parenthesize_if_required(binary_operator_expression.left(), PrecedenceLevel::NullableRewrap);
            if (get_binary_operator_type(binary_operator_expression.right()) == BinaryOperatorType::NullCoalescing) {
                parenthesize_if_required(binary_operator_expression.right(), precedence);
            } else {
                parenthesize_if_required(binary_operator_expression.right(), PrecedenceLevel::NullableRewrap);
            }
        } else {
            // ?? is right-associative
            parenthesize_if_required(binary_operator_expression.left(), precedence + 1);
            parenthesize_if_required(binary_operator_expression.right(), precedence);
        }
    } else {
        if (insert_parentheses_for_readability && precedence < PrecedenceLevel::Equality) {
            // Boost the priority of the operands to make the expression more readable
            PrecedenceLevel boost_to = is_bitwise_operator(binary_operator_expression.operator_()) ? PrecedenceLevel::Unary : PrecedenceLevel::Equality;
            if (get_binary_operator_type(binary_operator_expression.left()) == binary_operator_expression.operator_()) {
                parenthesize_if_required(binary_operator_expression.left(), precedence);
            } else {
                parenthesize_if_required(binary_operator_expression.left(), boost_to);
            }
            parenthesize_if_required(binary_operator_expression.right(), boost_to);
        } else {
            // all other binary operators are left-associative
            parenthesize_if_required(binary_operator_expression.left(), precedence);
            parenthesize_if_required(binary_operator_expression.right(), precedence + 1);
        }
    }
    DepthFirstAstVisitor::visit_binary_operator_expression(binary_operator_expression);
}

void InsertParenthesesVisitor::visit_is_expression(IsExpression& is_expression) {
    if (insert_parentheses_for_readability) {
        // few people know the precedence of 'is', so always parenthesize
        parenthesize_if_required(is_expression.expression(), PrecedenceLevel::NullableRewrap);
    } else {
        parenthesize_if_required(is_expression.expression(), PrecedenceLevel::RelationalAndTypeTesting);
    }
    DepthFirstAstVisitor::visit_is_expression(is_expression);
}

void InsertParenthesesVisitor::visit_as_expression(AsExpression& as_expression) {
    if (insert_parentheses_for_readability) {
        // few people know the precedence of 'as', so always parenthesize
        parenthesize_if_required(as_expression.expression(), PrecedenceLevel::NullableRewrap);
    } else {
        parenthesize_if_required(as_expression.expression(), PrecedenceLevel::RelationalAndTypeTesting);
    }
    DepthFirstAstVisitor::visit_as_expression(as_expression);
}

void InsertParenthesesVisitor::visit_interpolation(Interpolation& interpolation) {
    DepthFirstAstVisitor::visit_interpolation(interpolation);
    if (interpolation_needs_parenthesis(&interpolation)) parenthesize(interpolation.expression());
}

void InsertParenthesesVisitor::visit_conditional_expression(ConditionalExpression& conditional_expression) {
    if (is<Interpolation>(conditional_expression.parent())) {
        // Ternary Operators inside an interpolation need parentheses
        parenthesize(&conditional_expression);
    }
    // Associativity here is a bit tricky: (a ? b : c ? d : e) == (a ? b : (c ? d : e))
    if (insert_parentheses_for_readability && !is_conditional_ref_expression(&conditional_expression)) {
        // Precedence of ?: can be confusing; so always put parentheses in nested cases.
        parenthesize_if_required(conditional_expression.condition(), PrecedenceLevel::NullableRewrap);
        parenthesize_if_required(conditional_expression.true_expression(), PrecedenceLevel::NullableRewrap);
        parenthesize_if_required(conditional_expression.false_expression(), PrecedenceLevel::NullableRewrap);
    } else {
        parenthesize_if_required(conditional_expression.condition(), PrecedenceLevel::Conditional + 1);
        parenthesize_if_required(conditional_expression.true_expression(), PrecedenceLevel::Conditional);
        parenthesize_if_required(conditional_expression.false_expression(), PrecedenceLevel::Conditional);
    }
    DepthFirstAstVisitor::visit_conditional_expression(conditional_expression);
}

bool InsertParenthesesVisitor::is_conditional_ref_expression(ConditionalExpression* conditional_expression) {
    return is<DirectionExpression>(conditional_expression->true_expression()) || is<DirectionExpression>(conditional_expression->false_expression());
}

void InsertParenthesesVisitor::visit_assignment_expression(AssignmentExpression& assignment_expression) {
    // assignment is right-associative
    if (is<ArrayInitializerExpression>(assignment_expression.parent()) && !is<IndexerExpression>(assignment_expression.left())) {
        parenthesize(&assignment_expression);
    }
    parenthesize_if_required(assignment_expression.left(), PrecedenceLevel::Assignment + 1);
    handle_assignment_rhs(assignment_expression.right());
    DepthFirstAstVisitor::visit_assignment_expression(assignment_expression);
}

void InsertParenthesesVisitor::handle_assignment_rhs(Expression* right) {
    if (insert_parentheses_for_readability && !is<DirectionExpression>(right)) {
        parenthesize_if_required(right, PrecedenceLevel::Conditional + 1);
    } else {
        parenthesize_if_required(right, PrecedenceLevel::Assignment);
    }
}

void InsertParenthesesVisitor::visit_variable_initializer(VariableInitializer& variable_initializer) {
    if (variable_initializer.initializer() != nullptr) handle_assignment_rhs(variable_initializer.initializer());
    DepthFirstAstVisitor::visit_variable_initializer(variable_initializer);
}

void InsertParenthesesVisitor::visit_query_expression(QueryExpression& query_expression) {
    handle_lambda_or_query(&query_expression);
    DepthFirstAstVisitor::visit_query_expression(query_expression);
}

void InsertParenthesesVisitor::visit_lambda_expression(LambdaExpression& lambda_expression) {
    handle_lambda_or_query(&lambda_expression);
    DepthFirstAstVisitor::visit_lambda_expression(lambda_expression);
}

void InsertParenthesesVisitor::handle_lambda_or_query(Expression* expr) {
    const CSharpSlotInfo* slot = expr->slot();
    if (slot != nullptr && slot->kind() == &Slots::left) parenthesize(expr);
    if (is<IsExpression>(expr->parent()) || is<AsExpression>(expr->parent())) parenthesize(expr);
    if (insert_parentheses_for_readability) {
        if (is<UnaryOperatorExpression>(expr->parent()) || is<BinaryOperatorExpression>(expr->parent())) parenthesize(expr);
    }
}

void InsertParenthesesVisitor::visit_named_expression(NamedExpression& named_expression) {
    if (insert_parentheses_for_readability) {
        parenthesize_if_required(named_expression.expression(), PrecedenceLevel::RelationalAndTypeTesting + 1);
    }
    DepthFirstAstVisitor::visit_named_expression(named_expression);
}

void InsertParenthesesVisitor::visit_switch_expression(SwitchExpression& switch_expression) {
    parenthesize_if_required(switch_expression.expression(), PrecedenceLevel::Switch + 1);
    DepthFirstAstVisitor::visit_switch_expression(switch_expression);
}

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
