// dotnet/decompiler/csharp/syntax/do_while_statement.hpp -- DoWhileStatement: `do embedded while (condition);`.
//
//     Statement* stmt = make<DoWhileStatement>(body, condition);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: DoWhileStatement, from Statements/DoWhileStatement.cs with the members ILSpy
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

class DoWhileStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::DoWhileStatement;
    static constexpr std::string_view do_keyword = "do";
    static constexpr std::string_view while_keyword = "while";

    static constexpr CSharpSlotInfoT<Statement> embedded_statement_slot{"EmbeddedStatement", false, &Slots::embedded_statement, false};
    static constexpr CSharpSlotInfoT<Expression> condition_slot{"Condition", false, &Slots::condition, false};

    DoWhileStatement(Expression* condition_value, Statement* embedded_statement_value);
    DoWhileStatement();
    DoWhileStatement(Statement* embedded_statement_value, Expression* condition_value);

    Statement* embedded_statement() const { return embedded_statement_; }
    void set_embedded_statement(Statement* value);
    Expression* condition() const { return condition_; }
    void set_condition(Expression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_do_while_statement(*this); }
    AstNode* shallow_clone() const override { return make<DoWhileStatement>(*this); }

private:
    ChildPtr<Statement> embedded_statement_;
    ChildPtr<Expression> condition_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
