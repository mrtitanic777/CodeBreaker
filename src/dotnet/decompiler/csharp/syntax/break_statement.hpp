// dotnet/decompiler/csharp/syntax/break_statement.hpp -- BreakStatement: `break;`.
//
//     Statement* stmt = make<BreakStatement>();
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: BreakStatement, from Statements/BreakStatement.cs with the members ILSpy 11's
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

class BreakStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::BreakStatement;
    static constexpr std::string_view break_keyword = "break";



    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_break_statement(*this); }
    AstNode* shallow_clone() const override { return make<BreakStatement>(*this); }

private:
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
