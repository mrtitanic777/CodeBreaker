// dotnet/decompiler/csharp/syntax/is_expression.hpp -- an `is` type test, `expr is T`.
//
//     auto* e = make<IsExpression>(expr, type);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: IsExpression, from Expressions/IsExpression.cs), Copyright AlphaSierraPapa for the
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

class IsExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::IsExpression;

    static constexpr std::string_view is_keyword = "is";

    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};
    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, false};

    IsExpression() = default;
    IsExpression(Expression* expression, AstType* type);

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);
    AstType* type() const { return type_; }
    void set_type(AstType* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_is_expression(*this); }
    AstNode* shallow_clone() const override { return make<IsExpression>(*this); }

private:
    ChildPtr<Expression> expression_;
    ChildPtr<AstType> type_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
