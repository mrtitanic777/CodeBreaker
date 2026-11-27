// dotnet/decompiler/csharp/syntax/indexer_expression.hpp -- an element access, `target[a, b]`.
//
//     auto* i = make<IndexerExpression>(target, std::vector<Expression*>{a});
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: IndexerExpression, from Expressions/IndexerExpression.cs), Copyright AlphaSierraPapa for the
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

class IndexerExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::IndexerExpression;

    static constexpr CSharpSlotInfoT<Expression> target_slot{"Target", false, &Slots::target_expression, true};
    static constexpr CSharpSlotInfoT<Expression> arguments_slot{"Arguments", true, &Slots::argument, true};

    IndexerExpression() = default;
    IndexerExpression(Expression* target, const std::vector<Expression*>& arguments);
    IndexerExpression(Expression* target, std::initializer_list<Expression*> arguments);

    Expression* target() const { return target_; }
    void set_target(Expression* value);
    AstNodeCollection<Expression>& arguments() const { return arguments_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_indexer_expression(*this); }
    AstNode* shallow_clone() const override { return make<IndexerExpression>(*this); }

private:
    ChildPtr<Expression> target_;
    mutable AstNodeCollection<Expression> arguments_{this, Slots::argument, 1, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
