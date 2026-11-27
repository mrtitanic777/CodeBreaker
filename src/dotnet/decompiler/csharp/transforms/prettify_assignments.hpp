// dotnet/decompiler/csharp/transforms/prettify_assignments.hpp -- PrettifyAssignments: simplifies "x = x op y" into "x op= y" where
// possible, and "x += 1" into "x++" (or "++x" when the value is used).
//
//     PrettifyAssignments().run(syntax_tree, context);
//     AssignmentOperatorType op = PrettifyAssignments::get_assignment_operator_for_binary_operator(BinaryOperatorType::Add);   // Add
//
// Because the two "x" in "x = x op y" may refer to different ILVariables, this transform must run after DeclareVariables. It must also run
// after ReplaceMethodCallsWithOperators (so that it works for custom operators, too) and after AddCheckedBlocks (because
// "for (;; x = unchecked(x op y))" cannot be transformed into "x += y").
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: PrettifyAssignments, from PrettifyAssignments.cs), Copyright AlphaSierraPapa
// for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.hpp"
#include "dotnet/decompiler/csharp/syntax/operator_type.hpp"
#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

class PrettifyAssignments final : public syntax::DepthFirstAstVisitor, public IAstTransform {
public:
    void visit_assignment_expression(syntax::AssignmentExpression& assignment) override;

    // The compound assignment operator of a binary operator (Assign when it has none).
    static syntax::AssignmentOperatorType get_assignment_operator_for_binary_operator(syntax::BinaryOperatorType bop);

    void run(syntax::AstNode* node, TransformContext& context) override;

private:
    static bool can_convert_to_compound_assignment(syntax::Expression* left);
    static bool is_without_side_effects(syntax::Expression* left);

    TransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
