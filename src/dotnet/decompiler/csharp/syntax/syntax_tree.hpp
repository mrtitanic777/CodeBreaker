// dotnet/decompiler/csharp/syntax/syntax_tree.hpp -- SyntaxTree, the root of a C# file's syntax tree: its members (usings, namespaces,
// types ...) in order.
//
//     auto* tree = make<SyntaxTree>();
//     tree->members().add(namespace_declaration);
//     for (EntityDeclaration* type : tree->get_types(true)) ...      // every type and delegate declaration, inner ones too
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: SyntaxTree, from SyntaxTree.cs and the members generated for it), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class SyntaxTree final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::SyntaxTree;

    static constexpr CSharpSlotInfoT<AstNode> members_slot{"Members", true, &Slots::member, true};

    SyntaxTree() = default;
    explicit SyntaxTree(const std::vector<AstNode*>& member_list) { members_.add_range(member_list); }

    AstNodeCollection<AstNode>& members() const { return members_; }

    // The type and delegate declarations in the tree, in document order; those nested in other types only when `include_inner_types`.
    // (Statements and expressions are not searched.)
    std::vector<EntityDeclaration*> get_types(bool include_inner_types = false) const;

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override { return members_.count(); }
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_syntax_tree(*this); }
    AstNode* shallow_clone() const override { return make<SyntaxTree>(*this); }

private:
    mutable AstNodeCollection<AstNode> members_{this, Slots::member, 0, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
