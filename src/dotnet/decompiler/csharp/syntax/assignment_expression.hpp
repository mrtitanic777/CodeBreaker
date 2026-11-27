// dotnet/decompiler/csharp/syntax/assignment_expression.hpp -- an assignment, `left = right` or a compound one such as `left += right`.
//
//     auto* a = make<AssignmentExpression>(left, AssignmentOperatorType::Add, right);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: AssignmentExpression, from Expressions/AssignmentExpression.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/operator_type.hpp"
#include "dotnet/decompiler/csharp/syntax/tokens.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class AssignmentExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::AssignmentExpression;

    static constexpr std::string_view assign_token = "=";
    static constexpr std::string_view add_token = "+=";
    static constexpr std::string_view subtract_token = "-=";
    static constexpr std::string_view multiply_token = "*=";
    static constexpr std::string_view divide_token = "/=";
    static constexpr std::string_view modulus_token = "%=";
    static constexpr std::string_view shift_left_token = "<<=";
    static constexpr std::string_view shift_right_token = ">>=";
    static constexpr std::string_view unsigned_shift_right_token = ">>>=";
    static constexpr std::string_view bitwise_and_token = "&=";
    static constexpr std::string_view bitwise_or_token = "|=";
    static constexpr std::string_view exclusive_or_token = "^=";

    static constexpr CSharpSlotInfoT<Expression> left_slot{"Left", false, &Slots::left, false};
    static constexpr CSharpSlotInfoT<Expression> right_slot{"Right", false, &Slots::right, false};

    AssignmentExpression() = default;
    AssignmentExpression(Expression* left, Expression* right);
    AssignmentExpression(Expression* left, AssignmentOperatorType op, Expression* right);

    Expression* left() const { return left_; }
    void set_left(Expression* value);
    Expression* right() const { return right_; }
    void set_right(Expression* value);
    AssignmentOperatorType operator_() const { return operator_value_; }
    void set_operator_(AssignmentOperatorType value) { operator_value_ = value; }

    // The token written for an operator.
    static std::string_view get_operator_token(AssignmentOperatorType op);
    static il::ExpressionType get_linq_node_type(AssignmentOperatorType op, bool check_for_overflow) { return syntax::get_linq_node_type(op, check_for_overflow); }
    // The binary operator an assignment operator applies (none for plain assignment).
    static std::optional<BinaryOperatorType> get_corresponding_binary_operator(AssignmentOperatorType op) {
        return syntax::get_corresponding_binary_operator(op);
    }
    // The assignment operator of a LINQ expression type, if it has one.
    static std::optional<AssignmentOperatorType> get_assignment_operator_type_from_expression_type(il::ExpressionType expression_type);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_assignment_expression(*this); }
    AstNode* shallow_clone() const override { return make<AssignmentExpression>(*this); }

private:
    ChildPtr<Expression> left_;
    ChildPtr<Expression> right_;
    AssignmentOperatorType operator_value_ = AssignmentOperatorType{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
