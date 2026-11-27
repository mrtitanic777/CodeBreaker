// dotnet/decompiler/csharp/syntax/unchecked_statement.hpp -- UncheckedStatement: `unchecked { ... }`.
//
//     Statement* stmt = make<UncheckedStatement>(make<BlockStatement>());
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: UncheckedStatement, from Statements/UncheckedStatement.cs with the members ILSpy
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

class UncheckedStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::UncheckedStatement;
    static constexpr std::string_view unchecked_keyword = "unchecked";

    static constexpr CSharpSlotInfoT<BlockStatement> body_slot{"Body", false, &Slots::body, false};

    UncheckedStatement();
    explicit UncheckedStatement(BlockStatement* body_value);

    BlockStatement* body() const { return body_; }
    void set_body(BlockStatement* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_unchecked_statement(*this); }
    AstNode* shallow_clone() const override { return make<UncheckedStatement>(*this); }

private:
    ChildPtr<BlockStatement> body_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
