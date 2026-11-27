// dotnet/decompiler/csharp/syntax/throw_expression.hpp -- a throw expression, `throw expr`.
//
//     auto* t = make<ThrowExpression>(expr);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ThrowExpression, from Expressions/ThrowExpression.cs), Copyright AlphaSierraPapa for the
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

class ThrowExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ThrowExpression;

    static constexpr std::string_view throw_keyword = "throw";

    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};

    ThrowExpression() = default;
    explicit ThrowExpression(Expression* expression);

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_throw_expression(*this); }
    AstNode* shallow_clone() const override { return make<ThrowExpression>(*this); }

private:
    ChildPtr<Expression> expression_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
