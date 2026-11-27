// dotnet/decompiler/csharp/syntax/expression_statement.hpp -- ExpressionStatement: an expression used as a statement (`call();`).
//
//     Statement* stmt = make<ExpressionStatement>(call);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ExpressionStatement, from Statements/ExpressionStatement.cs with the members
// ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class ExpressionStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ExpressionStatement;
    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};

    ExpressionStatement();
    explicit ExpressionStatement(Expression* expression_value);

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_expression_statement(*this); }
    AstNode* shallow_clone() const override { return make<ExpressionStatement>(*this); }

private:
    ChildPtr<Expression> expression_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
