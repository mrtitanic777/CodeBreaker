// dotnet/decompiler/csharp/syntax/unary_operator_expression.hpp -- a unary operator application, `!x`, `x++`, `await x`, `x!`.
//
//     auto* e = make<UnaryOperatorExpression>(UnaryOperatorType::Not, expr);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: UnaryOperatorExpression, from Expressions/UnaryOperatorExpression.cs), Copyright AlphaSierraPapa for the
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

class UnaryOperatorExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::UnaryOperatorExpression;

    static constexpr std::string_view not_token = "!";
    static constexpr std::string_view bit_not_token = "~";
    static constexpr std::string_view minus_token = "-";
    static constexpr std::string_view plus_token = "+";
    static constexpr std::string_view increment_token = "++";
    static constexpr std::string_view decrement_token = "--";
    static constexpr std::string_view dereference_token = "*";
    static constexpr std::string_view address_of_token = "&";
    static constexpr std::string_view await_keyword = "await";
    static constexpr std::string_view null_conditional_token = "?";
    static constexpr std::string_view suppress_nullable_warning_token = "!";
    static constexpr std::string_view index_from_end_token = "^";
    static constexpr std::string_view pattern_not_keyword = "not";

    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};

    UnaryOperatorExpression() = default;
    UnaryOperatorExpression(UnaryOperatorType op, Expression* expression);

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);
    UnaryOperatorType operator_() const { return operator_value_; }
    void set_operator_(UnaryOperatorType value) { operator_value_ = value; }

    // The token written for an operator (none for the operators that have no syntax).
    static std::optional<std::string_view> get_operator_token(UnaryOperatorType op);
    static il::ExpressionType get_linq_node_type(UnaryOperatorType op, bool check_for_overflow) { return syntax::get_linq_node_type(op, check_for_overflow); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_unary_operator_expression(*this); }
    AstNode* shallow_clone() const override { return make<UnaryOperatorExpression>(*this); }

private:
    ChildPtr<Expression> expression_;
    UnaryOperatorType operator_value_ = UnaryOperatorType{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
