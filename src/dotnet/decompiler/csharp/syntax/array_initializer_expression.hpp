// dotnet/decompiler/csharp/syntax/array_initializer_expression.hpp -- an array or collection initializer, `{ 1, 2, 3 }`.
//
//     auto* init = make<ArrayInitializerExpression>(std::vector<Expression*>{a, b});
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ArrayInitializerExpression, from Expressions/ArrayInitializerExpression.cs), Copyright AlphaSierraPapa for the
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

class ArrayInitializerExpression : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ArrayInitializerExpression;

    static constexpr CSharpSlotInfoT<Expression> elements_slot{"Elements", true, &Slots::expression, true};

    ArrayInitializerExpression() = default;
    explicit ArrayInitializerExpression(const std::vector<Expression*>& elements);
    explicit ArrayInitializerExpression(std::initializer_list<Expression*> elements);

    AstNodeCollection<Expression>& elements() const { return elements_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

    static ArrayInitializerExpression* from_pattern(pattern_matching::Pattern* pattern);

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_array_initializer_expression(*this); }
    AstNode* shallow_clone() const override { return make<ArrayInitializerExpression>(*this); }

private:
    mutable AstNodeCollection<Expression> elements_{this, Slots::expression, 0, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
