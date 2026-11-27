// dotnet/decompiler/csharp/syntax/tuple_expression.hpp -- a tuple expression, `(a, b)`.
//
//     auto* t = make<TupleExpression>(std::vector<Expression*>{a, b});
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: TupleExpression, from Expressions/TupleExpression.cs), Copyright AlphaSierraPapa for the
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

class TupleExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::TupleExpression;

    static constexpr CSharpSlotInfoT<Expression> elements_slot{"Elements", true, &Slots::expression, true};

    TupleExpression() = default;
    explicit TupleExpression(const std::vector<Expression*>& elements);
    explicit TupleExpression(std::initializer_list<Expression*> elements);

    AstNodeCollection<Expression>& elements() const { return elements_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_tuple_expression(*this); }
    AstNode* shallow_clone() const override { return make<TupleExpression>(*this); }

private:
    mutable AstNodeCollection<Expression> elements_{this, Slots::expression, 0, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
