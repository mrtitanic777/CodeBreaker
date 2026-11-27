// dotnet/decompiler/csharp/syntax/extern_alias_declaration.hpp -- ExternAliasDeclaration: `extern alias name;`.
//
//     auto* d = make<ExternAliasDeclaration>();
//     d->set_name("Lib");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ExternAliasDeclaration, from GeneralScope/ExternAliasDeclaration.cs with the
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

class ExternAliasDeclaration final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ExternAliasDeclaration;
    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, false};

    ExternAliasDeclaration();
    explicit ExternAliasDeclaration(const std::string& name_value);

    Identifier* name_token() const { return name_token_; }
    void set_name_token(Identifier* value);
    std::string name() const { return name_token_->name(); }
    void set_name(const std::string& value) { set_name_token(Identifier::create(value)); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_extern_alias_declaration(*this); }
    AstNode* shallow_clone() const override { return make<ExternAliasDeclaration>(*this); }

private:
    ChildPtr<Identifier> name_token_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
