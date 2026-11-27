// dotnet/decompiler/csharp/syntax/if_else_statement.hpp -- IfElseStatement: `if (condition) true_statement [else false_statement]`.
//
//     Statement* stmt = make<IfElseStatement>(condition, then_statement, else_statement);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: IfElseStatement, from Statements/IfElseStatement.cs with the members ILSpy 11's
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

class IfElseStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::IfElseStatement;
    static constexpr std::string_view if_keyword = "if";
    static constexpr std::string_view else_keyword = "else";

    static constexpr CSharpSlotInfoT<Expression> condition_slot{"Condition", false, &Slots::condition, false};
    static constexpr CSharpSlotInfoT<Statement> true_statement_slot{"TrueStatement", false, &Slots::true_statement, false};
    static constexpr CSharpSlotInfoT<Statement> false_statement_slot{"FalseStatement", false, &Slots::false_statement, true};

    IfElseStatement();
    IfElseStatement(Expression* condition_value, Statement* true_statement_value);
    IfElseStatement(Expression* condition_value, Statement* true_statement_value, Statement* false_statement_value);

    Expression* condition() const { return condition_; }
    void set_condition(Expression* value);
    Statement* true_statement() const { return true_statement_; }
    void set_true_statement(Statement* value);
    Statement* false_statement() const { return false_statement_; }
    void set_false_statement(Statement* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_if_else_statement(*this); }
    AstNode* shallow_clone() const override { return make<IfElseStatement>(*this); }

private:
    ChildPtr<Expression> condition_;
    ChildPtr<Statement> true_statement_;
    ChildPtr<Statement> false_statement_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
