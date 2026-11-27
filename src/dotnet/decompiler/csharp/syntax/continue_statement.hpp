// dotnet/decompiler/csharp/syntax/continue_statement.hpp -- ContinueStatement: `continue;`.
//
//     Statement* stmt = make<ContinueStatement>();
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ContinueStatement, from Statements/ContinueStatement.cs with the members ILSpy
// 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class ContinueStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ContinueStatement;


    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_continue_statement(*this); }
    AstNode* shallow_clone() const override { return make<ContinueStatement>(*this); }

private:
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
