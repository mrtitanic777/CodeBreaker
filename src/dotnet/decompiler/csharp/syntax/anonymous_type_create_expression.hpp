// dotnet/decompiler/csharp/syntax/anonymous_type_create_expression.hpp -- an anonymous object creation, `new { A = 1, B = 2 }`.
//
//     auto* n = make<AnonymousTypeCreateExpression>(std::vector<Expression*>{a, b});
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: AnonymousTypeCreateExpression, from Expressions/AnonymousTypeCreateExpression.cs), Copyright AlphaSierraPapa for the
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

class AnonymousTypeCreateExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::AnonymousTypeCreateExpression;

    static constexpr std::string_view new_keyword = "new";

    static constexpr CSharpSlotInfoT<Expression> initializers_slot{"Initializers", true, &Slots::expression, true};

    AnonymousTypeCreateExpression() = default;
    explicit AnonymousTypeCreateExpression(const std::vector<Expression*>& initializers);
    explicit AnonymousTypeCreateExpression(std::initializer_list<Expression*> initializers);

    AstNodeCollection<Expression>& initializers() const { return initializers_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_anonymous_type_create_expression(*this); }
    AstNode* shallow_clone() const override { return make<AnonymousTypeCreateExpression>(*this); }

private:
    mutable AstNodeCollection<Expression> initializers_{this, Slots::expression, 0, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
