// dotnet/decompiler/csharp/syntax/conditional_expression.hpp -- the conditional operator, `cond ? a : b`.
//
//     auto* c = make<ConditionalExpression>(cond, a, b);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ConditionalExpression, from Expressions/ConditionalExpression.cs), Copyright AlphaSierraPapa for the
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

class ConditionalExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ConditionalExpression;

    static constexpr std::string_view question_mark_token = "?";
    static constexpr std::string_view colon_token = ":";

    static constexpr CSharpSlotInfoT<Expression> condition_slot{"Condition", false, &Slots::condition, false};
    static constexpr CSharpSlotInfoT<Expression> true_expression_slot{"TrueExpression", false, &Slots::true_, false};
    static constexpr CSharpSlotInfoT<Expression> false_expression_slot{"FalseExpression", false, &Slots::false_, false};

    ConditionalExpression() = default;
    ConditionalExpression(Expression* condition, Expression* true_expression, Expression* false_expression);

    Expression* condition() const { return condition_; }
    void set_condition(Expression* value);
    Expression* true_expression() const { return true_expression_; }
    void set_true_expression(Expression* value);
    Expression* false_expression() const { return false_expression_; }
    void set_false_expression(Expression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_conditional_expression(*this); }
    AstNode* shallow_clone() const override { return make<ConditionalExpression>(*this); }

private:
    ChildPtr<Expression> condition_;
    ChildPtr<Expression> true_expression_;
    ChildPtr<Expression> false_expression_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
