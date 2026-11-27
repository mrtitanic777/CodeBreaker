// dotnet/decompiler/csharp/syntax/using_statement.hpp -- UsingStatement: `[await] using (resource) embedded` (or the enhanced `using var x
// = ...;` form).
//
//     Statement* stmt = make<UsingStatement>(resource, body);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: UsingStatement, from Statements/UsingStatement.cs with the members ILSpy 11's
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

class UsingStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::UsingStatement;
    static constexpr std::string_view using_keyword = "using";
    static constexpr std::string_view await_keyword = "await";

    static constexpr CSharpSlotInfoT<AstNode> resource_acquisition_slot{"ResourceAcquisition", false, &Slots::resource_acquisition, false};
    static constexpr CSharpSlotInfoT<Statement> embedded_statement_slot{"EmbeddedStatement", false, &Slots::embedded_statement, false};

    UsingStatement();
    UsingStatement(AstNode* resource_acquisition_value, Statement* embedded_statement_value);

    AstNode* resource_acquisition() const { return resource_acquisition_; }
    void set_resource_acquisition(AstNode* value);
    Statement* embedded_statement() const { return embedded_statement_; }
    void set_embedded_statement(Statement* value);
    bool is_async() const { return is_async_; }
    void set_is_async(bool value) { is_async_ = value; }
    bool is_enhanced() const { return is_enhanced_; }
    void set_is_enhanced(bool value) { is_enhanced_ = value; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_using_statement(*this); }
    AstNode* shallow_clone() const override { return make<UsingStatement>(*this); }

private:
    ChildPtr<AstNode> resource_acquisition_;
    ChildPtr<Statement> embedded_statement_;
    bool is_async_{};
    bool is_enhanced_{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
