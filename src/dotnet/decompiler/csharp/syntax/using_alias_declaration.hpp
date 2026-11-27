// dotnet/decompiler/csharp/syntax/using_alias_declaration.hpp -- UsingAliasDeclaration: `using Alias = Namespace.Type;`.
//
//     auto* u = make<UsingAliasDeclaration>("Sys", "System");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: UsingAliasDeclaration, from GeneralScope/UsingAliasDeclaration.cs with the
// members ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class UsingAliasDeclaration final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::UsingAliasDeclaration;
    static constexpr std::string_view using_keyword = "using";

    static constexpr CSharpSlotInfoT<Identifier> alias_token_slot{"AliasToken", false, &Slots::alias, false};
    static constexpr CSharpSlotInfoT<AstType> import_slot{"Import", false, &Slots::import, false};

    UsingAliasDeclaration(const std::string& alias_value, const std::string& name_space);
    UsingAliasDeclaration();
    UsingAliasDeclaration(const std::string& alias_value, AstType* import_value);

    Identifier* alias_token() const { return alias_token_; }
    void set_alias_token(Identifier* value);
    std::string alias() const { return alias_token_->name(); }
    void set_alias(const std::string& value) { set_alias_token(Identifier::create(value)); }
    AstType* import() const { return import_; }
    void set_import(AstType* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_using_alias_declaration(*this); }
    AstNode* shallow_clone() const override { return make<UsingAliasDeclaration>(*this); }

private:
    ChildPtr<Identifier> alias_token_;
    ChildPtr<AstType> import_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
