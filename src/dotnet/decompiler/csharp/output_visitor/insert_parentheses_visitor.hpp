// dotnet/decompiler/csharp/output_visitor/insert_parentheses_visitor.hpp -- InsertParenthesesVisitor: wraps the sub-expressions of a syntax
// tree in parentheses where C#'s operator precedence would otherwise change their meaning (and, with insert_parentheses_for_readability,
// where a reader would want them). A tree built from the decompiler's output has no parenthesized expressions of its own: this is what
// puts them in before the tree is written.
//
//     InsertParenthesesVisitor visitor;
//     visitor.insert_parentheses_for_readability = true;
//     syntax_tree->accept_visitor(visitor);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: InsertParenthesesVisitor), Copyright AlphaSierraPapa for the ILSpy team,
// MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

class InsertParenthesesVisitor : public syntax::DepthFirstAstVisitor {
public:
    // Whether to parenthesize more than C# needs, for a reader.
    bool insert_parentheses_for_readability = false;

    void visit_member_reference_expression(syntax::MemberReferenceExpression& member_reference_expression) override;
    void visit_pointer_reference_expression(syntax::PointerReferenceExpression& pointer_reference_expression) override;
    void visit_invocation_expression(syntax::InvocationExpression& invocation_expression) override;
    void visit_indexer_expression(syntax::IndexerExpression& indexer_expression) override;
    void visit_unary_operator_expression(syntax::UnaryOperatorExpression& unary_operator_expression) override;
    void visit_cast_expression(syntax::CastExpression& cast_expression) override;
    void visit_binary_operator_expression(syntax::BinaryOperatorExpression& binary_operator_expression) override;
    void visit_is_expression(syntax::IsExpression& is_expression) override;
    void visit_as_expression(syntax::AsExpression& as_expression) override;
    void visit_interpolation(syntax::Interpolation& interpolation) override;
    void visit_conditional_expression(syntax::ConditionalExpression& conditional_expression) override;
    void visit_assignment_expression(syntax::AssignmentExpression& assignment_expression) override;
    void visit_variable_initializer(syntax::VariableInitializer& variable_initializer) override;
    void visit_query_expression(syntax::QueryExpression& query_expression) override;
    void visit_lambda_expression(syntax::LambdaExpression& lambda_expression) override;
    void visit_named_expression(syntax::NamedExpression& named_expression) override;
    void visit_switch_expression(syntax::SwitchExpression& switch_expression) override;

private:
    void handle_assignment_rhs(syntax::Expression* right);
    void handle_lambda_or_query(syntax::Expression* expr);
    bool is_conditional_ref_expression(syntax::ConditionalExpression* conditional_expression);
};

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
