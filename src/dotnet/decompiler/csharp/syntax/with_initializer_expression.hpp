// dotnet/decompiler/csharp/syntax/with_initializer_expression.hpp -- a `with` expression, `expr with { A = 1 }`.
//
//     auto* w = make<WithInitializerExpression>(expr, initializer);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: WithInitializerExpression, from Expressions/WithInitializerExpression.cs), Copyright AlphaSierraPapa for the
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

class WithInitializerExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::WithInitializerExpression;

    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};
    static constexpr CSharpSlotInfoT<ArrayInitializerExpression> initializer_slot{"Initializer", false, &Slots::initializer, false};

    WithInitializerExpression() = default;
    WithInitializerExpression(Expression* expression, ArrayInitializerExpression* initializer);

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);
    ArrayInitializerExpression* initializer() const { return initializer_; }
    void set_initializer(ArrayInitializerExpression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_with_initializer_expression(*this); }
    AstNode* shallow_clone() const override { return make<WithInitializerExpression>(*this); }

private:
    ChildPtr<Expression> expression_;
    ChildPtr<ArrayInitializerExpression> initializer_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
