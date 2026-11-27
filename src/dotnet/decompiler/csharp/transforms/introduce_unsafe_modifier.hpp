// dotnet/decompiler/csharp/transforms/introduce_unsafe_modifier.hpp -- IntroduceUnsafeModifier: adds the unsafe modifier to every member
// that uses pointers (pointer types, dereference, address-of, sizeof, fixed, stackalloc, calls with pointer parameters), and rewrites
// "*(ptr + i)" as "ptr[i]" and "(*ptr).member" as "ptr->member".
//
//     IntroduceUnsafeModifier().run(syntax_tree, context);
//     bool unsafe = IntroduceUnsafeModifier::is_unsafe(expression);      // also rewrites what it finds, as upstream's does
//
// The visitor returns whether the node it visited needs an unsafe context; a member declaration that has such a child gets the modifier
// (and answers false: its parent need not be unsafe).
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: IntroduceUnsafeModifier, from IntroduceUnsafeModifier.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.hpp"
#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

class IntroduceUnsafeModifier final : public syntax::DepthFirstAstVisitorT<bool>, public IAstTransform {
public:
    void run(syntax::AstNode* compilation_unit, TransformContext& context) override;

    // Whether the node (or something inside it) needs an unsafe context. Rewrites it as the transform does, with no context.
    static bool is_unsafe(syntax::AstNode* node);

    bool visit_pointer_reference_expression(syntax::PointerReferenceExpression& pointer_reference_expression) override;
    bool visit_size_of_expression(syntax::SizeOfExpression& size_of_expression) override;
    bool visit_composed_type(syntax::ComposedType& composed_type) override;
    bool visit_function_pointer_type(syntax::FunctionPointerAstType& function_pointer_type) override;
    bool visit_unary_operator_expression(syntax::UnaryOperatorExpression& unary_operator_expression) override;
    bool visit_member_reference_expression(syntax::MemberReferenceExpression& member_reference_expression) override;
    bool visit_identifier_expression(syntax::IdentifierExpression& identifier_expression) override;
    bool visit_stack_alloc_expression(syntax::StackAllocExpression& stack_alloc_expression) override;
    bool visit_invocation_expression(syntax::InvocationExpression& invocation_expression) override;
    bool visit_object_create_expression(syntax::ObjectCreateExpression& object_create_expression) override;
    bool visit_fixed_variable_initializer(syntax::FixedVariableInitializer& fixed_variable_initializer) override;

protected:
    bool visit_children(syntax::AstNode& node) override;

private:
    // The step log; the static is_unsafe entry point drives this visitor with no context, so there may be none.
    void step(const std::string& description, syntax::AstNode* node);
    void end_step(syntax::AstNode* node);
    static bool has_unsafe_resolve_result(const syntax::AstNode* node);
    static bool is_unsafe_type(const typesystem::IType* type);

    TransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
