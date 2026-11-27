// dotnet/decompiler/csharp/syntax/namespace_declaration.hpp -- NamespaceDeclaration: `namespace A.B { members }`.
//
//     auto* ns = make<NamespaceDeclaration>("A.B");
//     ns->add_member(type_declaration);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: NamespaceDeclaration, from GeneralScope/NamespaceDeclaration.cs with the members
// ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class NamespaceDeclaration final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::NamespaceDeclaration;
    static constexpr CSharpSlotInfoT<AstType> namespace_name_slot{"NamespaceName", false, &Slots::namespace_name, false};
    static constexpr CSharpSlotInfoT<AstNode> members_slot{"Members", true, &Slots::member, true};

    explicit NamespaceDeclaration(const std::string& name_value);
    NamespaceDeclaration();
    explicit NamespaceDeclaration(AstType* namespace_name_value);
    NamespaceDeclaration(AstType* namespace_name_value, const std::vector<AstNode*>& members_value);

    AstType* namespace_name() const { return namespace_name_; }
    void set_namespace_name(AstType* value);
    AstNodeCollection<AstNode>& members() const { return members_; }
    bool is_file_scoped() const { return is_file_scoped_; }
    void set_is_file_scoped(bool value) { is_file_scoped_ = value; }

    // The namespace's dotted name (written from namespace_name), and the same with the enclosing namespaces' names in front.
    std::string name() const;
    void set_name(const std::string& value);
    std::string full_name() const;
    // The names between the dots, outermost first.
    std::vector<std::string> identifiers() const;

    // `name1.name2`, or the one that is not empty.
    static std::string build_qualified_name(const std::string& name1, const std::string& name2);

    // Adds `child` to the members.
    void add_member(AstNode* child);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_namespace_declaration(*this); }
    AstNode* shallow_clone() const override { return make<NamespaceDeclaration>(*this); }

private:
    ChildPtr<AstType> namespace_name_;
    mutable AstNodeCollection<AstNode> members_{this, Slots::member, 1, true};
    bool is_file_scoped_{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
