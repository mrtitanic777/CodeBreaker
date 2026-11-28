// dotnet/decompiler/csharp/syntax/binary_operator_expression.hpp -- a binary operator application, `left + right`.
//
//     auto* e = make<BinaryOperatorExpression>(left, BinaryOperatorType::Add, right);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: BinaryOperatorExpression, from Expressions/BinaryOperatorExpression.cs), Copyright AlphaSierraPapa for the
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
#include "dotnet/decompiler/csharp/syntax/is_expression.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class BinaryOperatorExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::BinaryOperatorExpression;

    static constexpr std::string_view bitwise_and_token = "&";
    static constexpr std::string_view bitwise_or_token = "|";
    static constexpr std::string_view conditional_and_token = "&&";
    static constexpr std::string_view conditional_or_token = "||";
    static constexpr std::string_view exclusive_or_token = "^";
    static constexpr std::string_view greater_than_token = ">";
    static constexpr std::string_view greater_than_or_equal_token = ">=";
    static constexpr std::string_view equality_token = "==";
    static constexpr std::string_view in_equality_token = "!=";
    static constexpr std::string_view less_than_token = "<";
    static constexpr std::string_view less_than_or_equal_token = "<=";
    static constexpr std::string_view add_token = "+";
    static constexpr std::string_view subtract_token = "-";
    static constexpr std::string_view multiply_token = "*";
    static constexpr std::string_view divide_token = "/";
    static constexpr std::string_view modulus_token = "%";
    static constexpr std::string_view shift_left_token = "<<";
    static constexpr std::string_view shift_right_token = ">>";
    static constexpr std::string_view unsigned_shift_right_token = ">>>";
    static constexpr std::string_view null_coalescing_token = "??";
    static constexpr std::string_view range_token = "..";
    static constexpr std::string_view is_keyword = "is";
    static constexpr std::string_view pattern_or_keyword = "or";   // (not upstream: BinaryOperatorType::PatternOr)

    static constexpr CSharpSlotInfoT<Expression> left_slot{"Left", false, &Slots::left, true};
    static constexpr CSharpSlotInfoT<Expression> right_slot{"Right", false, &Slots::right, true};

    BinaryOperatorExpression() = default;
    BinaryOperatorExpression(Expression* left, BinaryOperatorType op);
    BinaryOperatorExpression(Expression* left, BinaryOperatorType op, Expression* right);

    Expression* left() const { return left_; }
    void set_left(Expression* value);
    Expression* right() const { return right_; }
    void set_right(Expression* value);
    BinaryOperatorType operator_() const { return operator_value_; }
    void set_operator_(BinaryOperatorType value) { operator_value_ = value; }

    // The token written for an operator.
    static std::string_view get_operator_token(BinaryOperatorType op);
    static il::ExpressionType get_linq_node_type(BinaryOperatorType op, bool check_for_overflow) { return syntax::get_linq_node_type(op, check_for_overflow); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_binary_operator_expression(*this); }
    AstNode* shallow_clone() const override { return make<BinaryOperatorExpression>(*this); }

private:
    ChildPtr<Expression> left_;
    ChildPtr<Expression> right_;
    BinaryOperatorType operator_value_ = BinaryOperatorType{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
