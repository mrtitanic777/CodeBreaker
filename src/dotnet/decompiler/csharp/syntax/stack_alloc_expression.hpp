// dotnet/decompiler/csharp/syntax/stack_alloc_expression.hpp -- `stackalloc T[count] { ... }`.
//
//     auto* s = make<StackAllocExpression>(type, count, initializer);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: StackAllocExpression, from Expressions/StackAllocExpression.cs), Copyright AlphaSierraPapa for the
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

class StackAllocExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::StackAllocExpression;

    static constexpr std::string_view stackalloc_keyword = "stackalloc";

    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, true};
    static constexpr CSharpSlotInfoT<Expression> count_expression_slot{"CountExpression", false, &Slots::expression, true};
    static constexpr CSharpSlotInfoT<ArrayInitializerExpression> initializer_slot{"Initializer", false, &Slots::initializer, true};

    StackAllocExpression() = default;
    StackAllocExpression(AstType* type, Expression* count_expression, ArrayInitializerExpression* initializer);

    AstType* type() const { return type_; }
    void set_type(AstType* value);
    Expression* count_expression() const { return count_expression_; }
    void set_count_expression(Expression* value);
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
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_stack_alloc_expression(*this); }
    AstNode* shallow_clone() const override { return make<StackAllocExpression>(*this); }

private:
    ChildPtr<AstType> type_;
    ChildPtr<Expression> count_expression_;
    ChildPtr<ArrayInitializerExpression> initializer_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
