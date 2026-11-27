// dotnet/decompiler/csharp/syntax/while_statement.hpp -- WhileStatement: `while (condition) embedded`.
//
//     Statement* stmt = make<WhileStatement>(condition, body);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: WhileStatement, from Statements/WhileStatement.cs with the members ILSpy 11's
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

class WhileStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::WhileStatement;
    static constexpr std::string_view while_keyword = "while";

    static constexpr CSharpSlotInfoT<Expression> condition_slot{"Condition", false, &Slots::condition, false};
    static constexpr CSharpSlotInfoT<Statement> embedded_statement_slot{"EmbeddedStatement", false, &Slots::embedded_statement, false};

    WhileStatement();
    WhileStatement(Expression* condition_value, Statement* embedded_statement_value);

    Expression* condition() const { return condition_; }
    void set_condition(Expression* value);
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
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_while_statement(*this); }
    AstNode* shallow_clone() const override { return make<WhileStatement>(*this); }

private:
    ChildPtr<Expression> condition_;
    ChildPtr<Statement> embedded_statement_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
