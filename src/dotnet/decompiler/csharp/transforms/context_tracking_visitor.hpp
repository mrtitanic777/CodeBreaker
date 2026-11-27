// dotnet/decompiler/csharp/transforms/context_tracking_visitor.hpp -- ContextTrackingVisitor, the base of AST visitors that need to know
// the type and method they are inside of: initialize(context) sets them from the TransformContext, and visiting a type, method,
// constructor, destructor, operator or accessor declaration sets them for the length of that visit.
//
//     class ReplaceX final : public ContextTrackingVisitor<AstNode*>, public IAstTransform {
//         void run(AstNode* root, TransformContext& context) override { initialize(context); root->accept_visitor(*this); uninitialize(); }
//         AstNode* visit_invocation_expression(InvocationExpression& e) override { ... current_method_ ... return visit_children(e); }
//     };
//
// A visitor that has no use for a result derives from ContextTrackingVisitor<AstNode*> and returns nullptr (upstream's TResult is `object`
// in practice); the visit methods take their node by reference, as in syntax/depth_first_ast_visitor.hpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: ContextTrackingVisitor<TResult>, from ContextTrackingVisitor.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

// Base class for AST visitors that need the current type/method context info.
template <class TResult>
class ContextTrackingVisitor : public syntax::DepthFirstAstVisitorT<TResult> {
protected:
    using Base = syntax::DepthFirstAstVisitorT<TResult>;

    const typesystem::ITypeDefinition* current_type_definition_ = nullptr;
    const typesystem::IMethod* current_method_ = nullptr;

    void initialize(const TransformContext& context) {
        current_type_definition_ = context.current_type_definition();
        current_method_ = dynamic_cast<const typesystem::IMethod*>(context.current_member());
    }

    void uninitialize() {
        current_type_definition_ = nullptr;
        current_method_ = nullptr;
    }

public:
    TResult visit_type_declaration(syntax::TypeDeclaration& type_declaration) override {
        Restore<const typesystem::ITypeDefinition*> restore(current_type_definition_);
        current_type_definition_ = dynamic_cast<const typesystem::ITypeDefinition*>(get_symbol(&type_declaration));
        return Base::visit_type_declaration(type_declaration);
    }

    TResult visit_method_declaration(syntax::MethodDeclaration& method_declaration) override {
        Restore<const typesystem::IMethod*> restore(current_method_);
        current_method_ = dynamic_cast<const typesystem::IMethod*>(get_symbol(&method_declaration));
        return Base::visit_method_declaration(method_declaration);
    }

    TResult visit_constructor_declaration(syntax::ConstructorDeclaration& constructor_declaration) override {
        Restore<const typesystem::IMethod*> restore(current_method_);
        current_method_ = dynamic_cast<const typesystem::IMethod*>(get_symbol(&constructor_declaration));
        return Base::visit_constructor_declaration(constructor_declaration);
    }

    TResult visit_destructor_declaration(syntax::DestructorDeclaration& destructor_declaration) override {
        Restore<const typesystem::IMethod*> restore(current_method_);
        current_method_ = dynamic_cast<const typesystem::IMethod*>(get_symbol(&destructor_declaration));
        return Base::visit_destructor_declaration(destructor_declaration);
    }

    TResult visit_operator_declaration(syntax::OperatorDeclaration& operator_declaration) override {
        Restore<const typesystem::IMethod*> restore(current_method_);
        current_method_ = dynamic_cast<const typesystem::IMethod*>(get_symbol(&operator_declaration));
        return Base::visit_operator_declaration(operator_declaration);
    }

    TResult visit_accessor(syntax::Accessor& accessor) override {
        Restore<const typesystem::IMethod*> restore(current_method_);
        current_method_ = dynamic_cast<const typesystem::IMethod*>(get_symbol(&accessor));
        return Base::visit_accessor(accessor);
    }

private:
    // Puts the old value of a field back when the visit ends (upstream's try/finally).
    template <class T>
    struct Restore {
        explicit Restore(T& field_) : field(field_), old_value(field_) {}
        ~Restore() { field = old_value; }
        Restore(const Restore&) = delete;
        Restore& operator=(const Restore&) = delete;
        T& field;
        T old_value;
    };
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
