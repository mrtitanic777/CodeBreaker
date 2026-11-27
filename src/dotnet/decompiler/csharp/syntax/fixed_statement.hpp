// dotnet/decompiler/csharp/syntax/fixed_statement.hpp -- FixedStatement: `fixed (type variables) embedded`.
//
//     Statement* stmt = make<FixedStatement>(type, variables, body);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: FixedStatement, from Statements/FixedStatement.cs with the members ILSpy 11's
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

class FixedStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::FixedStatement;
    static constexpr std::string_view fixed_keyword = "fixed";

    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<VariableInitializer> variables_slot{"Variables", true, &Slots::variable, true};
    static constexpr CSharpSlotInfoT<Statement> embedded_statement_slot{"EmbeddedStatement", false, &Slots::embedded_statement, false};

    FixedStatement();
    FixedStatement(AstType* type_value, const std::vector<VariableInitializer*>& variables_value, Statement* embedded_statement_value);

    AstType* type() const { return type_; }
    void set_type(AstType* value);
    AstNodeCollection<VariableInitializer>& variables() const { return variables_; }
    Statement* embedded_statement() const { return embedded_statement_; }
    void set_embedded_statement(Statement* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_fixed_statement(*this); }
    AstNode* shallow_clone() const override { return make<FixedStatement>(*this); }

private:
    ChildPtr<AstType> type_;
    mutable AstNodeCollection<VariableInitializer> variables_{this, Slots::variable, 1, false};
    ChildPtr<Statement> embedded_statement_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
