// dotnet/decompiler/csharp/syntax/lock_statement.hpp -- LockStatement: `lock (expression) embedded`.
//
//     Statement* stmt = make<LockStatement>(monitor, body);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: LockStatement, from Statements/LockStatement.cs with the members ILSpy 11's
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

class LockStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::LockStatement;
    static constexpr std::string_view lock_keyword = "lock";

    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};
    static constexpr CSharpSlotInfoT<Statement> embedded_statement_slot{"EmbeddedStatement", false, &Slots::embedded_statement, false};

    LockStatement();
    LockStatement(Expression* expression_value, Statement* embedded_statement_value);

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);
    Statement* embedded_statement() const { return embedded_statement_; }
    void set_embedded_statement(Statement* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_lock_statement(*this); }
    AstNode* shallow_clone() const override { return make<LockStatement>(*this); }

private:
    ChildPtr<Expression> expression_;
    ChildPtr<Statement> embedded_statement_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
