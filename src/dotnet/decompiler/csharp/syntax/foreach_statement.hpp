// dotnet/decompiler/csharp/syntax/foreach_statement.hpp -- ForeachStatement: `[await] foreach (type designation in expression) embedded`.
//
//     Statement* stmt = make<ForeachStatement>(type, designation, collection, body);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ForeachStatement, from Statements/ForeachStatement.cs with the members ILSpy
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

class ForeachStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ForeachStatement;
    static constexpr std::string_view await_keyword = "await";
    static constexpr std::string_view foreach_keyword = "foreach";
    static constexpr std::string_view in_keyword = "in";

    static constexpr CSharpSlotInfoT<AstType> variable_type_slot{"VariableType", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<VariableDesignation> variable_designation_slot{"VariableDesignation", false, &Slots::variable_designation, false};
    static constexpr CSharpSlotInfoT<Expression> in_expression_slot{"InExpression", false, &Slots::expression, false};
    static constexpr CSharpSlotInfoT<Statement> embedded_statement_slot{"EmbeddedStatement", false, &Slots::embedded_statement, false};

    ForeachStatement();
    ForeachStatement(AstType* variable_type_value, VariableDesignation* variable_designation_value, Expression* in_expression_value, Statement* embedded_statement_value);

    AstType* variable_type() const { return variable_type_; }
    void set_variable_type(AstType* value);
    VariableDesignation* variable_designation() const { return variable_designation_; }
    void set_variable_designation(VariableDesignation* value);
    Expression* in_expression() const { return in_expression_; }
    void set_in_expression(Expression* value);
    Statement* embedded_statement() const { return embedded_statement_; }
    void set_embedded_statement(Statement* value);
    bool is_async() const { return is_async_; }
    void set_is_async(bool value) { is_async_ = value; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_foreach_statement(*this); }
    AstNode* shallow_clone() const override { return make<ForeachStatement>(*this); }

private:
    ChildPtr<AstType> variable_type_;
    ChildPtr<VariableDesignation> variable_designation_;
    ChildPtr<Expression> in_expression_;
    ChildPtr<Statement> embedded_statement_;
    bool is_async_{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
