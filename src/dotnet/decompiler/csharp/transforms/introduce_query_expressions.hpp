// dotnet/decompiler/csharp/transforms/introduce_query_expressions.hpp -- IntroduceQueryExpressions: decompiles the calls of the LINQ query
// methods (Select, Where, OrderBy, GroupBy, SelectMany, Join ...) back into query expressions ("from x in e where c select x.y").
//
//     IntroduceQueryExpressions().run(syntax_tree, context);
//
// Based on the translation of query expressions in the C# 4.0 specification (7.16.2). A query that does not end in a select or group
// clause (a degenerate one) is completed with a select, or merged with the query it is the source of.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: IntroduceQueryExpressions, from IntroduceQueryExpressions.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.hpp"
#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"
#include "dotnet/decompiler/csharp/transforms/combine_query_expressions.hpp"
#include "dotnet/decompiler/il/il_variable.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

class IntroduceQueryExpressions final : public IAstTransform {
public:
    void run(syntax::AstNode* root_node, TransformContext& context) override;

private:
    // Marks the identifiers of a name with a LetIdentifierAnnotation (not used by the transform).
    class ApplyAnnotationVisitor final : public syntax::DepthFirstAstVisitorT<syntax::AstNode*> {
    public:
        ApplyAnnotationVisitor(LetIdentifierAnnotation* annotation, std::string identifier) : annotation_(annotation), identifier_(std::move(identifier)) {}
        syntax::AstNode* visit_identifier(syntax::Identifier& identifier) override;

    private:
        LetIdentifierAnnotation* annotation_;
        std::string identifier_;
    };

    void combine_range_variables(syntax::QueryClause* clause, il::ILVariable* old_variable, il::ILVariable* new_variable);
    static bool is_degenerate_query(syntax::QueryExpression* query);
    void decompile_queries(syntax::AstNode* node);
    bool can_use_discard_assignment() const;
    syntax::QueryExpression* decompile_query(syntax::InvocationExpression* invocation);
    static bool is_complex_query(syntax::MemberReferenceExpression* mre);
    static syntax::QueryFromClause* make_from_clause(syntax::ParameterDeclaration* parameter, syntax::Expression* body);
    static bool is_null_conditional(syntax::Expression* target);
    // Wraps the expression in parentheses when it is a plain identifier matching the parameter name (the Select call was explicit).
    static syntax::Expression* wrap_expression_in_parentheses_if_necessary(syntax::Expression* expression, const std::string& parameter_name);
    // Ensures that all ThenBy's are correct, and that the list of ThenBy's is terminated by an 'OrderBy' invocation.
    static bool validate_then_by_chain(syntax::InvocationExpression* invocation, const std::string& expected_parameter_name);
    // Matches simple lambdas of the form "a => b".
    static bool match_simple_lambda(syntax::Expression* expr, syntax::ParameterDeclaration*& parameter, syntax::Expression*& body);
    static bool validate_parameter(syntax::ParameterDeclaration* p);

    TransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
