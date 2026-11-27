// dotnet/decompiler/csharp/syntax/using_declaration.hpp -- UsingDeclaration: `using Namespace;`.
//
//     auto* u = make<UsingDeclaration>("System.Collections");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: UsingDeclaration, from GeneralScope/UsingDeclaration.cs with the members ILSpy
// 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class UsingDeclaration final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::UsingDeclaration;
    static constexpr std::string_view using_keyword = "using";

    static constexpr CSharpSlotInfoT<AstType> import_slot{"Import", false, &Slots::import, false};

    explicit UsingDeclaration(const std::string& name_space);
    UsingDeclaration();
    explicit UsingDeclaration(AstType* import_value);

    AstType* import() const { return import_; }
    void set_import(AstType* value);

    // The imported namespace as dotted text.
    std::string namespace_name_text() const { return construct_namespace(import()); }

    // The dotted text of a namespace type: `A.B` for MemberType(A, B), with `::` where an extern alias is meant.
    static std::string construct_namespace(AstType* type);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_using_declaration(*this); }
    AstNode* shallow_clone() const override { return make<UsingDeclaration>(*this); }

private:
    ChildPtr<AstType> import_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
