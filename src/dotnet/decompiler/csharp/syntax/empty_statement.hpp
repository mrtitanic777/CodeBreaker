// dotnet/decompiler/csharp/syntax/empty_statement.hpp -- EmptyStatement: a lone `;`.
//
//     auto* stmt = make<EmptyStatement>();
//     stmt->set_location(TextLocation(3, 5));
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: EmptyStatement, from Statements/EmptyStatement.cs with the members ILSpy 11's
// source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class EmptyStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::EmptyStatement;

    TextLocation location() const { return location_; }
    void set_location(TextLocation value) { location_ = value; }

    // The statement is the one `;`: it ends one column after where it starts.
    TextLocation start_location() const override { return location_; }
    TextLocation end_location() const override { return TextLocation(location_.line(), location_.column() + 1); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_empty_statement(*this); }
    AstNode* shallow_clone() const override { return make<EmptyStatement>(*this); }

private:
    TextLocation location_{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
