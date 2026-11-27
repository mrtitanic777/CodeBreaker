// dotnet/decompiler/csharp/transforms/transform_field_and_constructor_initializers.hpp -- TransformFieldAndConstructorInitializers: moves
// the field initializers at the start of constructors to their field, property and event declarations, turns this-/base-constructor
// calls into constructor initializers, makes a lone constructor a primary constructor where C# 12 allows, and removes the constructors
// that are only what the compiler writes anyway (the implicit one, an empty static one).
//
//     TransformFieldAndConstructorInitializers transform;
//     transform.run(root_node, context);
//     bool generated = TransformFieldAndConstructorInitializers::is_generated_primary_constructor_backing_field(field);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: TransformFieldAndConstructorInitializers, from
// TransformFieldAndConstructorInitializers.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

// This transform moves field initializers at the start of constructors to their respective field declarations and transforms this-/base-ctor
// calls in constructors to constructor initializers.
class TransformFieldAndConstructorInitializers final : public IAstTransform {
public:
    // Pattern for reference types: this..ctor(...);
    static syntax::AstNode* this_call_class_pattern();

    // Pattern for value types: this = new TSelf(...);
    static syntax::AstNode* this_call_struct_pattern();

    static bool is_generated_primary_constructor_backing_field(const typesystem::IField* field);

    void run(syntax::AstNode* node, TransformContext& context) override;

private:
    bool transform_declaration(const typesystem::ITypeDefinition* current_type_definition, syntax::AstNode* node,
                               const std::vector<syntax::EntityDeclaration*>& members);

    TransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
