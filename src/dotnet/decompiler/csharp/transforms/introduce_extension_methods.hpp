// dotnet/decompiler/csharp/transforms/introduce_extension_methods.hpp -- IntroduceExtensionMethods: converts calls of extension methods
// written as static calls, Enumerable.Select(list, f), into the infix form, list.Select(f), where the resolver confirms the infix call
// binds to the same method.
//
//     IntroduceExtensionMethods().run(syntax_tree, context);
//
// The resolver needs the using scope IntroduceUsingDeclarations annotated the syntax tree with, so this transform runs after it.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: IntroduceExtensionMethods, from IntroduceExtensionMethods.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <vector>

#include "dotnet/decompiler/csharp/resolver/csharp_resolver.hpp"
#include "dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.hpp"
#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

class IntroduceExtensionMethods final : public syntax::DepthFirstAstVisitor, public IAstTransform {
public:
    void run(syntax::AstNode* root_node, TransformContext& context) override;

    void visit_namespace_declaration(syntax::NamespaceDeclaration& namespace_declaration) override;
    void visit_type_declaration(syntax::TypeDeclaration& type_declaration) override;
    void visit_invocation_expression(syntax::InvocationExpression& invocation_expression) override;

private:
    void initialize_context(const type_system::UsingScope* using_scope);
    // Whether the invocation of an extension method as a static method can be written infix; on success gives the member reference
    // (nullptr when the target is a simple name), the resolve result of the receiver and the first argument.
    bool can_transform_to_extension_method_call(syntax::InvocationExpression* invocation_expression, syntax::MemberReferenceExpression*& member_ref_expr,
                                                semantics::ResolveResult*& target, syntax::Expression*& first_argument);

    TransformContext* context_ = nullptr;
    resolver::CSharpResolver* resolver_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
